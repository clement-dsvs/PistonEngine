#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <vulkan/vulkan.hpp>

namespace piston {


    struct QueueFamilyIndices {
        uint32_t m_graphicsFamily;
        uint32_t m_presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    class Device {

    public:
        static Device &os_getInstance();

    private:
        Device();

        ~Device();

    public:
        // Not copyable or movable
        Device(const Device &) = delete;

        Device operator=(const Device &) = delete;

        Device(Device &&) = delete;

        Device &operator=(Device &&) = delete;

        VkCommandPool getCommandPool() const { return m_commandPool; }
        VkDevice device() const { return m_device; }
        VkInstance& o_getVkInstance() { return m_instance; }
        VkQueue graphicsQueue() const { return m_graphicsQueue; }
        VkQueue presentQueue() const { return m_presentQueue; }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        QueueFamilyIndices findPhysicalQueueFamilies() const { return findQueueFamilies(m_physicalDevice); }

        VkFormat findSupportedFormat(
            const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        // Buffer Helper Functions
        void createBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer &buffer,
            VkDeviceMemory &bufferMemory);

        VkCommandBuffer beginSingleTimeCommands();

        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void copyBufferToImage(
            VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

        void createImageWithInfo(
            const VkImageCreateInfo &imageInfo,
            VkMemoryPropertyFlags properties,
            VkImage &image,
            VkDeviceMemory &imageMemory);

        VkPhysicalDeviceProperties m_properties;

    private:
        void createInstance();

        void setupDebugMessenger();

        void pickPhysicalDevice();

        void createLogicalDevice();

        void createCommandPool();

        // helper functions
        bool isDeviceSuitable(VkPhysicalDevice device);

        std::vector<const char *> getRequiredExtensions() const;

        bool checkValidationLayerSupport();

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        void hasGflwRequiredInstanceExtensions() const;

        bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;

        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    public:
        [[nodiscard]] VkPhysicalDevice o_physicalDevice() const {
            return m_physicalDevice;
        }

    private:
        VkCommandPool m_commandPool;

        VkDevice m_device;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;

        const std::vector<const char *> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    };
}

#endif //DEVICE_HPP
