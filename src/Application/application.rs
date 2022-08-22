use ::winit::window::{Window, WindowBuilder};
use anyhow::{anyhow, Ok, Result};
use log::*;
use std::collections::HashSet;
use std::ffi::CStr;
use std::os::raw::c_void;
use thiserror::Error;
use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::prelude::v1_0::*;
use vulkanalia::vk::ExtDebugUtilsExtension;
use vulkanalia::vk::KhrSurfaceExtension;
use vulkanalia::vk::KhrSwapchainExtension;
use vulkanalia::window as vk_window;

use crate::Application::appcfg::*;
use crate::Application::applicationdata::ApplicationData;
use crate::Application::physicaldevice::PhysicalDevice;
use crate::Application::queuefamilyindices::QueueFamilyIndices;
use crate::Application::suitabilityerror::SuitabilityError;
use crate::Graphics::framebuffer::Framebuffer;
use crate::Graphics::pipeline::GraphicsPipeline;
use crate::Graphics::swapchain::Swapchain;
use crate::Graphics::swapchain::SwapchainSupport;

#[derive(Clone, Debug)]
pub struct Application {
    pub entry: Entry,
    pub instance: Instance,
    pub data: ApplicationData,
    pub device: Device,
    pub frame: usize,
}

impl Application {
    /// Creates our Vulkan app.
    pub unsafe fn create(window: &Window) -> Result<Self> {
        let loader = LibloadingLoader::new(LIBRARY)?;
        let entry = Entry::new(loader).map_err(|b| anyhow!("{}", b))?;
        let mut data = ApplicationData::default();
        let instance = Self::create_instance(window, &entry, &mut data)?;
        data.surface = vk_window::create_surface(&instance, window)?;
        PhysicalDevice::get_physical_device(&instance, &mut data);
        let device = PhysicalDevice::create_logical_device(&instance, &mut data)?;
        Swapchain::create_swapchain(window, &instance, &device, &mut data)?;
        Swapchain::create_swapchain_image_views(&device, &mut data)?;
        GraphicsPipeline::create_render_pass(&instance, &device, &mut data)?;
        GraphicsPipeline::create_pipeline(&device, &mut data)?;
        Framebuffer::create_framebuffers(&device, &mut data)?;
        Self::create_command_pool(&instance, &device, &mut data)?;
        Self::create_command_buffers(&device, &mut data)?;
        Swapchain::create_sync_objects(&device, &mut data)?;

        Ok(Self {
            entry,
            instance,
            data,
            device,
            frame: 0,
        })
    }

    /// Renders a frame for our Vulkan app.
    pub unsafe fn render(&mut self, window: &Window) -> Result<()> {
        let in_flight_fence = self.data.in_flight_fences[self.frame];

        self.device
            .wait_for_fences(&[in_flight_fence], true, u64::max_value())?;

        let image_index = self
            .device
            .acquire_next_image_khr(
                self.data.swapchain,
                u64::max_value(),
                self.data.image_available_semaphores[self.frame],
                vk::Fence::null(),
            )?
            .0 as usize;

        let image_in_flight = self.data.images_in_flight[image_index];
        if !image_in_flight.is_null() {
            self.device
                .wait_for_fences(&[image_in_flight], true, u64::max_value())?;
        }

        self.data.images_in_flight[image_index] = in_flight_fence;

        let wait_semaphores = &[self.data.image_available_semaphores[self.frame]];
        let wait_stages = &[vk::PipelineStageFlags::COLOR_ATTACHMENT_OUTPUT];
        let command_buffers = &[self.data.command_buffers[image_index]];
        let signal_semaphores = &[self.data.render_finished_semaphores[self.frame]];
        let submit_info = vk::SubmitInfo::builder()
            .wait_semaphores(wait_semaphores)
            .wait_dst_stage_mask(wait_stages)
            .command_buffers(command_buffers)
            .signal_semaphores(signal_semaphores);

        self.device.reset_fences(&[in_flight_fence])?;

        self.device
            .queue_submit(self.data.graphics_queue, &[submit_info], in_flight_fence)?;

        let swapchains = &[self.data.swapchain];
        let image_indices = &[image_index as u32];
        let present_info = vk::PresentInfoKHR::builder()
            .wait_semaphores(signal_semaphores)
            .swapchains(swapchains)
            .image_indices(image_indices);

        self.device
            .queue_present_khr(self.data.present_queue, &present_info)?;

        self.frame = (self.frame + 1) % MAX_FRAMES_IN_FLIGHT;

        Ok(())
    }

    /// Destroys our Vulkan app.
    pub unsafe fn destroy(&mut self) {
        self.data
            .in_flight_fences
            .iter()
            .for_each(|f| self.device.destroy_fence(*f, None));
        self.data
            .render_finished_semaphores
            .iter()
            .for_each(|s| self.device.destroy_semaphore(*s, None));
        self.data
            .image_available_semaphores
            .iter()
            .for_each(|s| self.device.destroy_semaphore(*s, None));
        self.device
            .destroy_command_pool(self.data.command_pool, None);
        self.device.destroy_pipeline(self.data.pipeline, None);
        self.device
            .destroy_pipeline_layout(self.data.pipeline_layout, None);
        self.data
            .framebuffers
            .iter()
            .for_each(|f| self.device.destroy_framebuffer(*f, None));
        self.device.destroy_render_pass(self.data.render_pass, None);
        self.data
            .swapchain_image_views
            .iter()
            .for_each(|v| self.device.destroy_image_view(*v, None));
        self.device.destroy_swapchain_khr(self.data.swapchain, None);
        self.device.destroy_device(None);
        self.instance.destroy_surface_khr(self.data.surface, None);

        if VALIDATION_ENABLED {
            self.instance
                .destroy_debug_utils_messenger_ext(self.data.messenger, None);
        }

        self.instance.destroy_instance(None);
    }

    unsafe fn create_instance(
        window: &Window,
        entry: &Entry,
        data: &mut ApplicationData,
    ) -> Result<Instance> {
        let application_info = vk::ApplicationInfo::builder()
            .application_name(b"Gloria\0")
            .application_version(vk::make_version(1, 0, 0))
            .engine_name(b"Gloria\0")
            .engine_version(vk::make_version(1, 0, 0))
            .api_version(vk::make_version(1, 0, 0));

        let all_layers = entry
            .enumerate_instance_layer_properties()?
            .iter()
            .map(|l| l.layer_name)
            .collect::<HashSet<_>>();

        if VALIDATION_ENABLED && !all_layers.contains(&VALIDATION_LAYER) {
            return Err(anyhow!("Validation Layer requested but is not supported."));
        }

        let layers = if VALIDATION_ENABLED {
            vec![VALIDATION_LAYER.as_ptr()]
        } else {
            Vec::new()
        };

        let mut extensions = vk_window::get_required_instance_extensions(window)
            .iter()
            .map(|e| e.as_ptr())
            .collect::<Vec<_>>();

        if VALIDATION_ENABLED {
            extensions.push(vk::EXT_DEBUG_UTILS_EXTENSION.name.as_ptr());
        }

        let mut info = vk::InstanceCreateInfo::builder()
            .application_info(&application_info)
            .enabled_layer_names(&layers)
            .enabled_extension_names(&extensions);

        let mut debug_info = vk::DebugUtilsMessengerCreateInfoEXT::builder()
            .message_severity(vk::DebugUtilsMessageSeverityFlagsEXT::all())
            .message_type(vk::DebugUtilsMessageTypeFlagsEXT::all())
            .user_callback(Some(Self::debug_callback));

        if VALIDATION_ENABLED {
            info = info.push_next(&mut debug_info);
        }

        let instance = entry.create_instance(&info, None)?;

        if VALIDATION_ENABLED {
            data.messenger = instance.create_debug_utils_messenger_ext(&debug_info, None)?;
        }

        Ok(instance)
    }

    unsafe fn create_command_pool(
        instance: &Instance,
        device: &Device,
        data: &mut ApplicationData,
    ) -> Result<()> {
        let indices = QueueFamilyIndices::get(instance, data, data.physical_device)?;

        let info = vk::CommandPoolCreateInfo::builder()
            .flags(vk::CommandPoolCreateFlags::empty())
            .queue_family_index(indices.graphics);

        data.command_pool = device.create_command_pool(&info, None)?;
        Ok(())
    }

    unsafe fn create_command_buffers(device: &Device, data: &mut ApplicationData) -> Result<()> {
        let allocate_info = vk::CommandBufferAllocateInfo::builder()
            .command_pool(data.command_pool)
            .level(vk::CommandBufferLevel::PRIMARY)
            .command_buffer_count(data.framebuffers.len() as u32);

        data.command_buffers = device.allocate_command_buffers(&allocate_info)?;

        for (i, command_buffer) in data.command_buffers.iter().enumerate() {
            let inheritance = vk::CommandBufferInheritanceInfo::builder();

            let info = vk::CommandBufferBeginInfo::builder()
                .flags(vk::CommandBufferUsageFlags::empty())
                .inheritance_info(&inheritance);

            device.begin_command_buffer(*command_buffer, &info)?;

            let render_area = vk::Rect2D::builder()
                .offset(vk::Offset2D::default())
                .extent(data.swapchain_extent);

            let color_clear_value = vk::ClearValue {
                color: vk::ClearColorValue {
                    float32: [0.0, 0.0, 0.0, 1.0],
                },
            };

            let clear_values = &[color_clear_value];
            let info = vk::RenderPassBeginInfo::builder()
                .render_pass(data.render_pass)
                .framebuffer(data.framebuffers[i])
                .render_area(render_area)
                .clear_values(clear_values);

            device.cmd_begin_render_pass(*command_buffer, &info, vk::SubpassContents::INLINE);
            device.cmd_bind_pipeline(
                *command_buffer,
                vk::PipelineBindPoint::GRAPHICS,
                data.pipeline,
            );
            device.cmd_draw(*command_buffer, 3, 1, 0, 0);
            device.cmd_end_render_pass(*command_buffer);
            device.end_command_buffer(*command_buffer)?;
        }

        Ok(())
    }

    extern "system" fn debug_callback(
        severity: vk::DebugUtilsMessageSeverityFlagsEXT,
        type_: vk::DebugUtilsMessageTypeFlagsEXT,
        data: *const vk::DebugUtilsMessengerCallbackDataEXT,
        _: *mut c_void,
    ) -> vk::Bool32 {
        let data = unsafe { *data };
        let message = unsafe { CStr::from_ptr(data.message) }.to_string_lossy();

        if severity >= vk::DebugUtilsMessageSeverityFlagsEXT::ERROR {
            error!("({:?}) {}", type_, message);
        } else if severity >= vk::DebugUtilsMessageSeverityFlagsEXT::WARNING {
            warn!("({:?}) {}", type_, message);
        } else if severity >= vk::DebugUtilsMessageSeverityFlagsEXT::INFO {
            debug!("({:?}) {}", type_, message);
        } else if severity >= vk::DebugUtilsMessageSeverityFlagsEXT::VERBOSE {
            debug!("({:?}) {}", type_, message);
        } else {
            trace!("({:?}) {}", type_, message);
        }

        vk::FALSE
    }
}
