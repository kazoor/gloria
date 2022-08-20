use anyhow::{anyhow, Result};
use::winit::window::{Window, WindowBuilder};
use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::window as vk_window;
use vulkanalia::prelude::v1_0::*;
use std::collections::HashSet;
use std::ffi::CStr;
use std::os::raw::c_void;
use log::*;
use vulkanalia::vk::ExtDebugUtilsExtension;

const VALIDATION_ENABLED: bool = cfg!(debug_assertions);
const VALIDATION_LAYER: vk::ExtensionName = vk::ExtensionName::from_bytes(b"VK_LAYER_KHRONOS_validation");

#[derive(Clone, Debug)]
pub struct Application {
    entry: Entry,
    instance: Instance,
    data: ApplicationData,
}

impl Application {
    /// Creates our Vulkan app.
    pub unsafe fn create(window: &Window) -> Result<Self> {
        let loader = LibloadingLoader::new(LIBRARY)?;
        let entry = Entry::new(loader).map_err(|b| anyhow!("{}", b))?;
        let mut data = ApplicationData::default();
        let instance = create_instance(window, &entry, &mut data)?;

        Ok(Self {entry, instance, data})
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
    }
}

/// The Vulkan handles and associated properties used by our Vulkan app.
#[derive(Clone, Debug, Default)]
pub struct ApplicationData {
    messenger: vk::DebugUtilsMessengerEXT,
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
        .user_callback(Some(debug_callback));

    if VALIDATION_ENABLED {
        info = info.push_next(&mut debug_info);
    }

    let instance = entry.create_instance(&info, None)?;

    if VALIDATION_ENABLED {
        data.messenger = instance.create_debug_utils_messenger_ext(&debug_info, None)?;
    }

    Ok(instance)
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
    } else {
        trace!("({:?}) {}", type_, message);
    }

    vk::FALSE
}