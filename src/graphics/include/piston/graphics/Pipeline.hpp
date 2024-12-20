//
// Created by cleme on 20/12/2024.
//

#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "piston/graphics/Device.hpp"

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace piston {
    struct PipelineConfigInfo
    {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline
    {
    public:
        Pipeline(Device& a_device, const std::string& a_vertFilePath, const std::string& a_fragFilePath,
                 const PipelineConfigInfo& a_configInfo);
        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        Pipeline operator=(const Pipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);

        static void defaultPipeLineConfigInfo(PipelineConfigInfo& configInfo);

    private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath,
                                    const PipelineConfigInfo& a_configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        Device& device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}


#endif //PIPELINE_HPP
