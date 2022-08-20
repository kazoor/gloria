use anyhow::{anyhow, Result, Ok};
use::winit::window::{Window, WindowBuilder};
use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::window as vk_window;
use vulkanalia::prelude::v1_0::*;
use std::collections::HashSet;
use std::ffi::CStr;
use std::os::raw::c_void;
use thiserror::Error;
use log::*;
use vulkanalia::vk::ExtDebugUtilsExtension;

const VALIDATION_ENABLED: bool = cfg!(debug_assertions);
const VALIDATION_LAYER: vk::ExtensionName = vk::ExtensionName::from_bytes(b"VK_LAYER_KHRONOS_validation");

/// The Vulkan handles and associated properties used by our Vulkan app.
#[derive(Clone, Debug, Default)]
pub struct ApplicationData {
    messenger: vk::DebugUtilsMessengerEXT,
    physical_device: vk::PhysicalDevice,
    graphics_queue: vk::Queue,
}

#[derive(Debug, Error)]
#[error("Missing {0}.")]
pub struct SuitabilityError(pub &'static str);

#[derive(Copy, Clone, Debug)]
struct QueueFamilyIndices {
    graphics: u32,
}

impl QueueFamilyIndices {
    unsafe fn get(instance: &Instance, data: &ApplicationData, physical_device: vk::PhysicalDevice) -> Result<Self> {
        let properties = instance.get_physical_device_queue_family_properties(physical_device);

        let graphics = properties
        .iter()
        .position(|p| p.queue_flags.contains(vk::QueueFlags::GRAPHICS))
        .map(|i| i as u32);

        if let Some(graphics) = graphics {
            Ok(Self{graphics})
        } else {
            Err(anyhow!(SuitabilityError("Missing require queue families.")))
        }
    }
}

#[derive(Clone, Debug)]
pub struct Application {
    entry: Entry,
    instance: Instance,
    data: ApplicationData,
    device: Device,
}

impl Application {
    /// Creates our Vulkan app.
    pub unsafe fn create(window: &Window) -> Result<Self> {
        let loader = LibloadingLoader::new(LIBRARY)?;
        let entry = Entry::new(loader).map_err(|b| anyhow!("{}", b))?;
        let mut data = ApplicationData::default();
        let instance = Self::create_instance(window, &entry, &mut data)?;
        let device = Self::create_logical_device(&instance, &mut data)?;

        Self::get_physical_device(&instance, &mut data);
        Ok(Self {entry, instance, data, device})
    }

    /// Renders a frame for our Vulkan app.
    pub unsafe fn render(&mut self, window: &Window) -> Result<()> {
        Ok(())
    }

    /// Destroys our Vulkan app.
    pub unsafe fn destroy(&mut self) {
        if VALIDATION_ENABLED {
            self.instance.destroy_debug_utils_messenger_ext(self.data.messenger, None);
        }
        
        self.instance.destroy_instance(None);
        self.device.destroy_device(None);
    }
    
    unsafe fn create_instance(window: &Window, entry: &Entry, data: &mut ApplicationData) -> Result<Instance> {
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

    unsafe fn get_physical_device(instance: &Instance, data: &mut ApplicationData) -> Result<()> {
        for physical_device in instance.enumerate_physical_devices()? {
            let properties = instance.get_physical_device_properties(physical_device);

            if let Err(error) = Self::check_physical_devices(instance, data, physical_device) {
                warn!("Skipping physical device: {}: {}", properties.device_name, error);
            } else {
                info!("Selected physical device: {}.", properties.device_name);
                data.physical_device = physical_device;
                return Ok(());
            }
        }

        Err(anyhow!("Failed to find suitabled GPU."))
    }

    unsafe fn check_physical_devices(instance: &Instance, data: &ApplicationData, physical_device: vk::PhysicalDevice) -> Result<()> {
        let properties = instance.get_physical_device_properties(physical_device);
        if properties.device_type != vk::PhysicalDeviceType::DISCRETE_GPU {
            return Err(anyhow!(SuitabilityError("Application only accpets discrete GPUs are supported. (imagine using integrated graphics lmao).")));
        }
        
        let features = instance.get_physical_device_features(physical_device);
        if features.geometry_shader != vk::TRUE {
            return Err(anyhow!(SuitabilityError("Missing geometry shader support.")));
        }

        QueueFamilyIndices::get(instance, data, physical_device)?;
        Ok(())
    }

    unsafe fn create_logical_device(instance: &Instance, data: &mut ApplicationData) -> Result<Device> {
        let indices = QueueFamilyIndices::get(instance, data, data.physical_device)?;

        let queue_properties = &[1.0];
        let queue_info = vk::DeviceQueueCreateInfo::builder()
            .queue_family_index(indices.graphics)
            .queue_priorities(queue_properties);

        let layers = if VALIDATION_ENABLED {
            vec![VALIDATION_LAYER.as_ptr()]
        } else {
            vec![]
        };

        let features = vk::PhysicalDeviceFeatures::builder();

        let queue_infos = &[queue_info];
        let info = vk::DeviceCreateInfo::builder()
            .queue_create_infos(queue_infos)
            .enabled_layer_names(&layers)
            .enabled_features(&features);
        
        let device = instance.create_device(data.physical_device, &info, None)?;

        data.graphics_queue = device.get_device_queue(indices.graphics, 0);

        Ok(device)
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