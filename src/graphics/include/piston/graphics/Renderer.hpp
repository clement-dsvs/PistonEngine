#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "piston/window/Window.hpp"
#include "piston/graphics/Device.hpp"
#include "piston/graphics/SwapChain.hpp"

namespace piston {
    class Renderer
    {
    public:

        Renderer(Window& a_window, Device& a_device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer &operator=(const Renderer&) = delete;

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer a_commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer a_commandBuffer);

        VkRenderPass getSwapChainRenderPass() const { return m_swapChain->getRenderPass(); }

        float getAspectRatio() const { return m_swapChain->extentAspectRatio(); }

        bool isFrameInProgress() const { return isFrameStarted; }
        VkCommandBuffer o_getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
            return m_commandBuffers[currentFrameIndex];
        }

        int o_getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
            return currentFrameIndex;
        }

    private:
        void o_createCommandBuffers();
        void o_freeCommandBuffers();
        void o_recreateSwapChain();

        Window& m_window;
        Device& m_device;
        std::unique_ptr<SwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;

        uint32_t currentImageIndex = 0;
        int currentFrameIndex = 0;
        bool isFrameStarted = false;
    };
}

#endif //RENDERER_HPP
