#![allow(
    dead_code,
    unused_variables,
    clippy::too_many_arguments,
    clippy::unnecessary_wraps
)]
use ::winit::window::{Window, WindowBuilder};
use anyhow::{anyhow, Result};
use vulkanalia::vk::DeviceV1_0;
use winit::dpi::LogicalSize;
use winit::event::{Event, WindowEvent};
use winit::event_loop::{ControlFlow, EventLoop};
mod Application;

fn main() -> Result<()> {
    pretty_env_logger::init();

    // Window
    let event_loop = EventLoop::new();
    let window = WindowBuilder::new()
        .with_title("Gloria")
        .with_inner_size(LogicalSize::new(1920, 1080))
        .build(&event_loop)?;

    // App
    let mut app = unsafe { Application::application::Application::create(&window)? };
    let mut destroying = false;
    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Poll;
        match event {
            // Render a frame if our Vulkan app is not being destroyed.
            Event::MainEventsCleared if !destroying => unsafe { app.render(&window) }.unwrap(),
            // Destroy our Vulkan app.
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                ..
            } => {
                destroying = true;
                *control_flow = ControlFlow::Exit;
                unsafe {
                    app.device.device_wait_idle().unwrap();
                }
                unsafe {
                    app.destroy();
                }
            }
            _ => {}
        }
    });
}
