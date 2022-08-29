#include <iostream>
#include <format>
#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include <gloria.hpp>
#include <functional>

using namespace gloria::core;

class SandboxLayer : public Layer {
public:
	SandboxLayer() : Layer("SandboxLayer") {

	}

	~SandboxLayer() {

	}

	void destroy() {

	}

	void onUpdate(gloria::util::Time t) {
		GL_INFO("Time: {0}", t.minutes());
	}
};

class App : public Application {
public:
	App() {
		gloria::Shared<SandboxLayer> layer = std::make_shared<SandboxLayer>();
		pushLayer(layer);
	}
};

int main() {
	auto app = new App();
	app->run();

	return 0;
}