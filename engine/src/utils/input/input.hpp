#pragma once
#include <utility>
#include "../base.hpp"
#include "../base.hpp"

namespace gloria::util {
    class Input;
    static Input* inputInstancePtr = nullptr;

    class Input {
    public:
        Input() { inputInstancePtr = this; }

        static bool isKeyPressed(int KeyCode) { return inputInstancePtr->isKeyPressedImpl(KeyCode); }

        static bool isMouseKeyPressed(int KeyCode) { return inputInstancePtr->isMouseKeyPressedImpl(KeyCode); }

        static std::pair<double, double> getMousePosition() { return inputInstancePtr->getMousePositionImpl(); }

        static double getMouseX() { return inputInstancePtr->getMouseXImpl(); }

        static double getMouseY() { return inputInstancePtr->getMouseYImpl(); }

        static std::pair<double, double> getScrollInput() { return inputInstancePtr->getScrollInputImpl(); }

        static void gesetScroll() { inputInstancePtr->resetScrollImpl(); };

        static Input& get() {
            return *inputInstancePtr;
        }

    protected:
        virtual bool isKeyPressedImpl(int KeyCode) = 0;

        virtual bool isMouseKeyPressedImpl(int KeyCode) = 0;

        virtual std::pair<double, double> getMousePositionImpl() = 0;

        virtual double getMouseXImpl() = 0;

        virtual double getMouseYImpl() = 0;

        virtual double getScrollXImpl() = 0;

        virtual double getScrollYImpl() = 0;

        virtual std::pair<double, double> getScrollInputImpl() = 0;

        virtual void resetScrollImpl() = 0;
    };
}

static gloria::Shared<gloria::util::Input> gInputInstance = nullptr;