use anyhow::{anyhow, Ok, Result};
use vulkanalia::loader::{LibloadingLoader, LIBRARY};
use vulkanalia::prelude::v1_0::*;

use crate::Application::applicationdata::ApplicationData;

pub struct Framebuffer {}

impl Framebuffer {
    pub unsafe fn create_framebuffers(device: &Device, data: &mut ApplicationData) -> Result<()> {
        data.framebuffers = data
            .swapchain_image_views
            .iter()
            .map(|i| {
                let attachments = &[*i];
                let create_info = vk::FramebufferCreateInfo::builder()
                    .render_pass(data.render_pass)
                    .attachments(attachments)
                    .width(data.swapchain_extent.width)
                    .height(data.swapchain_extent.height)
                    .layers(1);

                device.create_framebuffer(&create_info, None)
            })
            .collect::<Result<Vec<_>, _>>()?;

        Ok(())
    }
}
