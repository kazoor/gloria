use anyhow::{anyhow, Ok, Result};
use log::*;
use std::collections::HashSet;
use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::prelude::v1_0::*;
use vulkanalia::vk::ExtDebugUtilsExtension;
use vulkanalia::vk::KhrSurfaceExtension;
use vulkanalia::vk::KhrSwapchainExtension;

use crate::Application::appcfg::*;
use crate::Application::applicationdata::ApplicationData;
use crate::Application::queuefamilyindices::QueueFamilyIndices;
use crate::Application::suitabilityerror::SuitabilityError;
use crate::Graphics::swapchain::SwapchainSupport;

#[derive(Default, Clone, Debug)]
pub struct PhysicalDevice {}

impl PhysicalDevice {
    pub unsafe fn get_physical_device(
        instance: &Instance,
        data: &mut ApplicationData,
    ) -> Result<()> {
        for physical_device in instance.enumerate_physical_devices()? {
            let properties = instance.get_physical_device_properties(physical_device);

            if let Err(error) = Self::check_physical_devices(instance, data, physical_device) {
                warn!(
                    "Skipping physical device: {}: {}",
                    properties.device_name, error
                );
            } else {
                info!("Selected physical device: {}.", properties.device_name);
                data.physical_device = physical_device;
                return Ok(());
            }
        }

        Err(anyhow!("Failed to find suitabled GPU."))
    }

    pub unsafe fn check_physical_devices(
        instance: &Instance,
        data: &ApplicationData,
        physical_device: vk::PhysicalDevice,
    ) -> Result<()> {
        let properties = instance.get_physical_device_properties(physical_device);
        if properties.device_type != vk::PhysicalDeviceType::DISCRETE_GPU {
            return Err(anyhow!(SuitabilityError("Application only accpets discrete GPUs are supported. (imagine using integrated graphics lmao).")));
        }

        let features = instance.get_physical_device_features(physical_device);
        if features.geometry_shader != vk::TRUE {
            return Err(anyhow!(SuitabilityError(
                "Missing geometry shader support."
            )));
        }

        QueueFamilyIndices::get(instance, data, physical_device)?;
        Self::check_physical_device_extensions(instance, physical_device);

        let support = SwapchainSupport::get(instance, data, physical_device)?;
        if support.formats.is_empty() || support.present_modes.is_empty() {
            return Err(anyhow!(SuitabilityError("Insufficient swapchain support.")));
        }

        Ok(())
    }

    pub unsafe fn check_physical_device_extensions(
        instance: &Instance,
        physical_device: vk::PhysicalDevice,
    ) -> Result<()> {
        let extensions = instance
            .enumerate_device_extension_properties(physical_device, None)?
            .iter()
            .map(|e| e.extension_name)
            .collect::<HashSet<_>>();

        if DEVICE_EXTENSIONS.iter().all(|e| extensions.contains(e)) {
            Ok(())
        } else {
            Err(anyhow!(SuitabilityError(
                "Missing required device extensions."
            )))
        }
    }

    pub unsafe fn create_logical_device(
        instance: &Instance,
        data: &mut ApplicationData,
    ) -> Result<Device> {
        let indices = QueueFamilyIndices::get(instance, data, data.physical_device)?;

        let mut unique_indices: HashSet<u32> = HashSet::new();
        unique_indices.insert(indices.graphics);
        unique_indices.insert(indices.present);

        let queue_priorities = &[1.0];
        let queue_infos = unique_indices
            .iter()
            .map(|i| {
                vk::DeviceQueueCreateInfo::builder()
                    .queue_family_index(*i)
                    .queue_priorities(queue_priorities)
            })
            .collect::<Vec<_>>();

        let layers = if VALIDATION_ENABLED {
            vec![VALIDATION_LAYER.as_ptr()]
        } else {
            vec![]
        };

        let features = vk::PhysicalDeviceFeatures::builder();

        let extensions = DEVICE_EXTENSIONS
            .iter()
            .map(|n| n.as_ptr())
            .collect::<Vec<_>>();

        let info = vk::DeviceCreateInfo::builder()
            .queue_create_infos(&queue_infos)
            .enabled_layer_names(&layers)
            .enabled_extension_names(&extensions)
            .enabled_features(&features);

        let device = instance.create_device(data.physical_device, &info, None)?;

        data.graphics_queue = device.get_device_queue(indices.graphics, 0);
        data.present_queue = device.get_device_queue(indices.present, 0);

        Ok(device)
    }
}
