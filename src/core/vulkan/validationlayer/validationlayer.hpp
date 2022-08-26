#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "../../defines.hpp"

class ValidationLayer {
public:
	ValidationLayer();

	bool checkValidationLayerSupport();

	std::vector<const char*> getRequiredExtensions();

	bool isEnabled();

private:
#ifdef DEBUG
	const bool mEnabled = true;
#else
	const bool mEnabled = false;
#endif
};