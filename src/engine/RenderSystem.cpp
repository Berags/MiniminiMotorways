//
// Created by Jacopo Beragnoli on 13/04/22.
//

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <array>
#include <cassert>

// headers
#include "../../include/engine/RenderSystem.h"
#include "../../include/engine/exceptions/vulkan/FailedToCreatePipeLineLayoutException.h"

namespace Engine {

    struct SimplePushConstantData {
        glm::mat4 transform{1.f};
        glm::mat4 modelMatrix{1.f};
    };

    RenderSystem::RenderSystem(Engine::Device &device, VkRenderPass renderPass)
            : device(device) {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    RenderSystem::~RenderSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void RenderSystem::createPipelineLayout() {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw Engine::Exceptions::FailedToCreatePipeLineLayoutException();
        }
    }

    void RenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Engine::Pipeline::defaultConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Engine::Pipeline>(
                device,
                "../shaders/simple_shader.vert.spv",
                "../shaders/simple_shader.frag.spv",
                pipelineConfig);
    }

    void RenderSystem::renderGameObjects(
            VkCommandBuffer commandBuffer, std::vector<Engine::GameObject> &gameObjects, const Engine::Camera &camera) {
        pipeline->bind(commandBuffer);

        auto projectionView = camera.getProjection() * camera.getView();

        for (auto &obj: gameObjects) {
            SimplePushConstantData push{};
            auto modelMatrix = obj.transform.mat4();
            push.transform = projectionView * modelMatrix;
            push.modelMatrix = modelMatrix;

            vkCmdPushConstants(
                    commandBuffer,
                    pipelineLayout,
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                    0,
                    sizeof(SimplePushConstantData),
                    &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}