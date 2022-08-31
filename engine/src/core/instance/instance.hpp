#pragma once
#include <iostream>
#include <cstdint>
#include "../../utils/base.hpp"
#include "../../defines.hpp"
#include "../window/window.hpp"
#include "../../utils/input/input.hpp"
#include "../../utils/input/inputhandler/inputhandler.hpp"
#include "../../vulkan/vulkaninstance/vulkaninstance.hpp"
#include "../../vulkan/vulkanrenderer/vulkanrenderer.hpp"

namespace gloria::core {
    class Instance;
    static Instance* instanceClassPtr = nullptr;

    class Instance
    {
    public:
        Instance() {
            instanceClassPtr = this;
            mWindow = std::make_shared<Window>();
            gInputInstance = std::make_shared<util::InputHandler>();
            mVulkanInstance = std::make_shared<vk::VulkanInstance>();
            mVulkanRenderer = std::make_shared<vk::VulkanRenderer>();
        }

        ~Instance() {

        }

        static Instance& get() {
            return *instanceClassPtr;
        }

        Window& getWindow() {
            return *mWindow;
        }

        vk::VulkanInstance& getVkInstance() {
            return *mVulkanInstance;
        }

        vk::VulkanRenderer& getRenderer() {
            return *mVulkanRenderer;
        }

        std::uint32_t currentFrame{ 0 };

        bool framebufferResized{ false };
    private:
        Shared<Window> mWindow{ nullptr };
        Shared<vk::VulkanInstance> mVulkanInstance{ nullptr };
        Shared<vk::VulkanRenderer> mVulkanRenderer{ nullptr };
    };
}

static gloria::Shared<gloria::core::Instance> gInstance = nullptr;