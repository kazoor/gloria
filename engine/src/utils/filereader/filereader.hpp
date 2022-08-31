#pragma once
#include "../../defines.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace gloria::util {
	class FileReader {
	public:
		FileReader() {}

		~FileReader() {}

		static std::vector<char> readFile(const std::string& filepath) {
			std::ifstream file(filepath, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				GL_CORE_ERROR("Failed to open file: {0}", filepath);
				throw std::runtime_error("readFile");
			}

			std::size_t fileSize = static_cast<std::size_t>(file.tellg());
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}
	};
}