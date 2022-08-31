#pragma once
#include <utility>
#include "../input.hpp"

namespace gloria::util {
    class InputHandler : public Input {
    public:
        static Input& get() {
            return *inputInstancePtr;
        }

    protected:
        bool isKeyPressedImpl(int KeyCode) override;

        bool isMouseKeyPressedImpl(int KeyCode) override;

        std::pair<double, double> getMousePositionImpl() override;

        double getMouseXImpl() override;

        double getMouseYImpl() override;

        double getScrollXImpl() override;

        double getScrollYImpl() override;

        std::pair<double, double> getScrollInputImpl() override;

        void resetScrollImpl() override;
    };
}