#include <iostream>
#include <format>
#include <string>
#include <GLFW/glfw3.h>
#include <gloria.hpp>

using namespace gloria::core;

class SandBox : public Application {
public:
	SandBox() {
		mInstance = getInstance();
		init();
	}

	void init() override {
		mInstance.getWindow().createWindow(1920, 1080, "Gloria");
		update();
		destroy();
	}

	void update() override {
		while (!glfwWindowShouldClose(mInstance.getWindow().getWindow())) {
			glfwPollEvents();
		}
	}

	void destroy() override {
		mInstance.getWindow().destroy();
	}

private:
	Instance mInstance;
};

int main() {
	SandBox sandBox;
	return 0;
}