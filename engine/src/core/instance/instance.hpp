#pragma once
#include <iostream>
#include "../../utils/base.hpp"
#include "../../defines.hpp"
#include "../window/window.hpp"

namespace gloria::core {
    class Instance;
    static Instance* instanceClassPtr = nullptr;

    class Instance
    {
    public:
        Instance() {
            instanceClassPtr = this;
            mWindow = std::make_shared<Window>();
        }

        static Instance& get() {
            return *instanceClassPtr;
        }

        Window& getWindow() {
            return *mWindow;
        }

    private:
        Shared<Window> mWindow = nullptr;
    };
}

static gloria::Shared<gloria::core::Instance> gInstance = nullptr;