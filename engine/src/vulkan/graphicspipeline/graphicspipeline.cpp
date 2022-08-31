#include "graphicspipeline.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include "../../defines.hpp"
#include "../../utils/filereader/filereader.hpp"
#include "../../core/instance/instance.hpp"

namespace gloria::vk {
	GraphicsPipeline::GraphicsPipeline() {
	}

	GraphicsPipeline::~GraphicsPipeline() {
	}

	void GraphicsPipeline::init() {
		createGraphicsPipeline();
	}

	void GraphicsPipeline::createGraphicsPipeline() {
		auto vertShaderCode = util::FileReader::readFile("resources/shaders/base.vert.spv");
		auto fragShaderCode = util::FileReader::readFile("resources/shaders/base.frag.spv");

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vertShaderModule,
			.pName = "main"
		};

		VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = fragShaderModule,
			.pName = "main"
		};

		VkPipelineShaderStageCreateInfo shaderstages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = 0,
			.pVertexBindingDescriptions = nullptr,
			.vertexAttributeDescriptionCount = 0,
			.pVertexAttributeDescriptions = nullptr
		};

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE
		};

		VkViewport viewport = {
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(core::Instance::get().getVkInstance().getSwapchain().swapChainExtent.width),
			.height = static_cast<float>(core::Instance::get().getVkInstance().getSwapchain().swapChainExtent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = core::Instance::get().getVkInstance().getSwapchain().swapChainExtent
		};

		VkPipelineViewportStateCreateInfo viewportStateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.pViewports = &viewport,
			.scissorCount = 1,
			.pScissors = &scissor
		};

		VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f,
		};

		VkPipelineMultisampleStateCreateInfo multisamplingInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};

		VkPipelineColorBlendAttachmentState colorBlendattachmentState = {
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &colorBlendattachmentState,
			.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
		};

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast<std::uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data()
		};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};

		VK_VALIDATE(vkCreatePipelineLayout(core::Instance::get().getVkInstance().getLogicalDevice().get(), &pipelineLayoutInfo, nullptr, &mPipelineLayout), "Failed to create pipeline layout");

#ifdef DEBUG
		if (mPipelineLayout != VK_NULL_HANDLE)
			GL_CORE_INFO("Created pipeline layout");
#endif // DEBUG

		const VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
			.colorAttachmentCount = 1,
			.pColorAttachmentFormats = &core::Instance::get().getVkInstance().getSwapchain().swapChainImageFormat,
			// .depthAttachmentFormat = VulkanSwapchain::Get().FindDepthFormat(), // if not defined. comment this line.
		};

		VkGraphicsPipelineCreateInfo pipelineInfo = {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &pipelineRenderingCreateInfo,
			.stageCount = 2,
			.pStages = shaderstages,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = &inputAssemblyInfo,
			.pViewportState = &viewportStateInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisamplingInfo,
			.pDepthStencilState = nullptr,
			.pColorBlendState = &colorBlendingInfo,
			.pDynamicState = &dynamicStateInfo,
			.layout = mPipelineLayout,
			.renderPass = nullptr,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};

		VK_VALIDATE(vkCreateGraphicsPipelines(core::Instance::get().getVkInstance().getLogicalDevice().get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline), "Failed to create pipeline");

#ifdef DEBUG
		if (mPipeline != VK_NULL_HANDLE)
			GL_CORE_INFO("Created pipeline layout");
#endif // DEBUG

		vkDestroyShaderModule(core::Instance::get().getVkInstance().getLogicalDevice().get(), fragShaderModule, nullptr);
		vkDestroyShaderModule(core::Instance::get().getVkInstance().getLogicalDevice().get(), vertShaderModule, nullptr);
	}

	void GraphicsPipeline::destroy() {
		vkDestroyPipeline(core::Instance::get().getVkInstance().getLogicalDevice().get(), mPipeline, nullptr);
		vkDestroyPipelineLayout(core::Instance::get().getVkInstance().getLogicalDevice().get(), mPipelineLayout, nullptr);
	}

	VkPipeline& GraphicsPipeline::get() {
		return mPipeline;
	}

	VkShaderModule GraphicsPipeline::createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = code.size(),
			.pCode = reinterpret_cast<const std::uint32_t*>(code.data())
		};

		VkShaderModule shaderModule;
		VK_VALIDATE(vkCreateShaderModule(core::Instance::get().getVkInstance().getLogicalDevice().get(), &createInfo, nullptr, &shaderModule), "Failed to create shader module");

#ifdef DEBUG
		if (shaderModule != VK_NULL_HANDLE)
			GL_CORE_INFO("Shadermodule created");
#endif // DEBUG
		return shaderModule;
	}
}