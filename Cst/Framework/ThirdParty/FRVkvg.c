#include <Framework/Device/FRWindow.h>
#include <Framework/FRVkvg.h>
#include <Framework/FRRenderPrivate.h>

#define IDX_IS_COMPLETE(indices) (indices->graphicsFamily >= 0 && indices->presentFamily >= 0)
#define SWAPCHAIN_HAS_VALUE(swap) (swap->formatCount >= 0 && swap->presentModeCount >= 0)

typedef struct _QueueFamilyIndices QueueFamilyIndices;
typedef struct _SwapChainSupportDetails SwapChainSupportDetails;
typedef struct _Vertex Vertex;
typedef struct _UniformBufferObject UniformBufferObject;
typedef struct _FRVkvgInstance FRVkvgInstance;
typedef FRVkvgInstance* FRVkvgInstanceT;

struct _FRVkvgInstance {
  VkInstance instance;
  SysChar *appName;
  SysChar *engineName;

  bool enableLayer;
  VkDebugUtilsMessengerEXT debugMessenger;

  uint32_t eCount;
  uint32_t layerCount;
  SysChar* ppExtensions[10];
  SysChar* ppLayers[10];

  // Physic device
  VkPhysicalDevice physicalDevice;
  SwapChainSupportDetails *chainSupport;
  QueueFamilyIndices *indices;
  VkDevice logicDevice;

  VkQueue graphicsQueue;

  // window surface
  VkSurfaceKHR vkSurface;

  // present
  VkQueue presentQueue;
  VkSemaphore presentSemaphore;
  VkSemaphore drawSemaphore;

  uint32_t currentFrame;
  uint32_t drawFenceCount;
  VkFence drawFence;

  VkSwapchainKHR swapChain;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  SysInt imageCount;
  VkImageView *swapChainImageViews;
  VkImage *swapChainImages;

  VkCommandPool commandPool;
  VkCommandBuffer *commandBuffers;

  // vkvg env
  FRWindow *window;
  uint32_t width;
  uint32_t height;
  VkvgSurface vkvgSurface;
  VkvgDevice vkvgDevice;
  VkvgContext vkvgContext;
};

struct _QueueFamilyIndices {
  int graphicsFamily;
  int presentFamily;
};

struct _SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  int formatCount;
  VkSurfaceFormatKHR *formats;

  int presentModeCount;
  VkPresentModeKHR *presentModes;
};

static FRRenderInstanceT fr_vkvg_create_instance(FRWindow *window);
static void fr_vkvg_free_instance(FRVkvgInstanceT vinst);
static void fr_vkvg_present(FRRenderInstanceT inst);
static FRSurfaceT fr_vkvg_create_surface(FRRenderInstanceT inst);
static FRRenderContextT fr_vkvg_create_context(FRRenderInstanceT inst);

static FRRenderBackend backend = {
  .type = FR_CANVAS_VKVG,
  .create_instance = &fr_vkvg_create_instance,
  .create_surface = &fr_vkvg_create_surface,
  .create_context = &fr_vkvg_create_context,
  .present = &fr_vkvg_present,

  .destroy_surface = (SysPointer)&vkvg_surface_destroy,
  .destroy_context = (SysPointer)&vkvg_destroy,
  .destroy_instance = (SysPointer)&fr_vkvg_free_instance,

  .paint = (SysPointer)&vkvg_paint,
  .set_source_rgba = (SysPointer)&vkvg_set_source_rgba,
  .move_to = (SysPointer)&vkvg_move_to,
  .rel_line_to = (SysPointer)&vkvg_rel_line_to,
  .arc = (SysPointer)&vkvg_arc,
  .new_path = (SysPointer)&vkvg_new_path,
  .close_path = (SysPointer)&vkvg_close_path,
  .stroke = (SysPointer)&vkvg_stroke,
  .show_text = (SysPointer)&vkvg_show_text,
  .flush = (SysPointer)&vkvg_flush,
  .clear = (SysPointer)&vkvg_clear,
  .surface_clear = (SysPointer)&vkvg_surface_clear,
  .rectangle = (SysPointer)&vkvg_rectangle,
  .rounded_rectangle = (SysPointer)&vkvg_rounded_rectangle,
  .rounded_rectangle2 = (SysPointer)&vkvg_rounded_rectangle2,
};

static const SysChar* const validationLayers[] = {
  "VK_LAYER_KHRONOS_validation",
#if 0
  "VK_LAYER_RENDERDOC_Capture",
#endif
};

static const char* const deviceExtensions[] = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

/*-------------------------------------------- vk part --------------------------------------------*/
static void fr_vk_check_result(VkResult f) {
  VkResult res = (f);
  if (res != VK_SUCCESS) {
    sys_abort_N("%s: %d", SYS_("vk check error."), res);
  }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL fr_vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  sys_error_N("validation: %s", pCallbackData->pMessage);

  UNUSED(messageType);
  UNUSED(pUserData);

  if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    sys_abort_N("%s", "messageSeverity");
  }
  return VK_FALSE;
}

static void fr_vk_populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *createInfo) {
  createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo->messageSeverity = 
    // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

  createInfo->messageType = 
    // VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo->pfnUserCallback = fr_vk_debug_callback;
}

static void fr_vk_get_required_extension(const SysChar* ppExtensions[], uint32_t *count) {
  uint32_t ec = 0;
  uint32_t extCount = 0;
  const char** gexts;

  vkvg_get_required_instance_extensions((const char **)ppExtensions, &extCount);
  gexts = glfwGetRequiredInstanceExtensions(&ec);

  sys_array_foreach(const char *, node, gexts, ec) {
    ppExtensions[i + extCount] = node;
  }

  *count = ec + extCount;
}

static void fr_vk_get_layer_extension(SysChar* ppExtensions[], uint32_t *count) {
  uint32_t eCount = ARRAY_SIZE(validationLayers);

  memcpy(ppExtensions, validationLayers, sizeof(char *) * eCount);

  *count = eCount;
}

static QueueFamilyIndices *fr_vk_find_queue_families(VkSurfaceKHR surface, VkPhysicalDevice device) {
  QueueFamilyIndices *indices = sys_new0_N(QueueFamilyIndices, 1);

  indices->graphicsFamily = -1;
  indices->presentFamily = -1;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
  VkQueueFamilyProperties *queueFamilies = sys_new0_N(VkQueueFamilyProperties, queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

  sys_array_foreach(VkQueueFamilyProperties, queueFamily, queueFamilies, queueFamilyCount) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices->graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport) {
      indices->presentFamily = i;
    }

    if (IDX_IS_COMPLETE(indices)) {
      break;
    }
  }

  return indices;
}

static SysBool fr_vk_check_device_extension_support(VkPhysicalDevice device, SysChar **extNames, SysInt extCount) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
  VkExtensionProperties *availableExtensions = sys_new0_N(VkExtensionProperties, extensionCount);
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

  sys_array_foreach(SysChar *, extName, extNames, extCount) {
    SysBool exists = false;

    sys_array_foreach(VkExtensionProperties, extension, availableExtensions, extensionCount) {
      if (strcmp(extension.extensionName, extName) == 0) {
        exists = true;
        break;
      }
    }

    if (!exists) {
      sys_error_N(SYS_("%s is unsupport device extensions."), extName);
      return false;
    }
  }

  return true;
}

static SwapChainSupportDetails *fr_vk_query_swap_chain_support(VkSurfaceKHR surface, VkPhysicalDevice device) {
  SwapChainSupportDetails *details = sys_new0_N(SwapChainSupportDetails, 1);

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details->capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);

  if (formatCount != 0) {
    details->formats = sys_new0_N(VkSurfaceFormatKHR, formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details->formats);
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, NULL);

  if (presentModeCount != 0) {
    details->presentModes = sys_new0_N(VkPresentModeKHR, presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details->presentModes);
  }

  details->formatCount = formatCount;
  details->presentModeCount = presentModeCount;

  return details;
}

static bool fr_vk_is_device_suitable(VkPhysicalDevice device) {
  if (!fr_vk_check_device_extension_support(device, (SysChar **)deviceExtensions, ARRAY_SIZE(deviceExtensions))) {
    return false;
  }

  return true;
}

static VkPhysicalDevice fr_vk_pick_physical_device(VkInstance instance) {
  uint32_t deviceCount = 0;
  VkPhysicalDevice phyDevice = NULL;

  vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

  if (deviceCount == 0) {
    sys_abort_N("%s", SYS_("failed to find GPUs with Vulkan support!"));
  }

  VkPhysicalDevice *devices = sys_new0_N(VkPhysicalDevice, deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

  sys_array_foreach(VkPhysicalDevice, device, devices, deviceCount) {
    if (fr_vk_is_device_suitable(device)) {
      phyDevice = device;
      break;
    }
  }

  if (phyDevice == VK_NULL_HANDLE) {
    sys_abort_N("%s", SYS_("failed to find a suitable GPU!"));
  }

  return phyDevice;
}

static VkSurfaceFormatKHR fr_vk_choose_swap_surface_format(const VkSurfaceFormatKHR *availableFormats, uint32_t formatCount) {
  sys_array_foreach(VkSurfaceFormatKHR, availableFormat, availableFormats, formatCount) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

static VkPresentModeKHR fr_vk_choose_swap_present_mode(const VkPresentModeKHR *availablePresentModes, uint32_t presentModeCount) {
  sys_array_foreach(VkPresentModeKHR, availablePresentMode, availablePresentModes, presentModeCount) {
    if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
      return availablePresentMode;
    }

    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D fr_vk_choose_swap_extent(const VkSurfaceCapabilitiesKHR *capabilities, uint32_t width, uint32_t height) {
  if (capabilities->currentExtent.width != UINT32_MAX) {
    return capabilities->currentExtent;
  }
  else {
    VkExtent2D actualExtent = { width, height };

    actualExtent.width = CLAMP(actualExtent.width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
    actualExtent.height = CLAMP(actualExtent.height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);

    return actualExtent;
  }
}

static void fr_vk_create_swap_chain(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice logicDevice,
  int imageCount,
  VkSurfaceFormatKHR surfaceFormat,
  VkExtent2D extent,
  VkSurfaceTransformFlagBitsKHR preTransform,
  VkPresentModeKHR presentMode,
  VkSwapchainKHR oldSwapChain,
  VkSwapchainKHR *swapChain) {

  VkSwapchainCreateInfoKHR createInfo = { 0 };
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.pNext = NULL;
  createInfo.flags = 0;
  createInfo.surface = surface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  createInfo.queueFamilyIndexCount = 0;
  createInfo.pQueueFamilyIndices = NULL;
  createInfo.preTransform = preTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = oldSwapChain;

  QueueFamilyIndices *indices = fr_vk_find_queue_families(surface, physicalDevice);
  uint32_t queueFamilyIndices[] = { (uint32_t)indices->graphicsFamily, (uint32_t)indices->presentFamily };
  
  if (indices->graphicsFamily != indices->presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  if (vkCreateSwapchainKHR(logicDevice, &createInfo, NULL, swapChain) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to create swap chain!"));
  }
}

static void fr_vk_create_command_buffer(VkDevice logicDevice,
  VkCommandPool commandPool,
  VkCommandBufferLevel level,
  VkCommandBuffer *buffer) {

  VkCommandBufferAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                    .pNext = NULL,
                    .commandPool = commandPool,
                    .level = level,
                    .commandBufferCount = 1 };

  if (vkAllocateCommandBuffers(logicDevice, &allocInfo, buffer) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to allocate command buffer!"));
  }
}

#if 0
static uint32_t fr_vk_find_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  sys_error_N("%s", SYS_("failed to find suitable memory type!"));
  return 0;
}

static void fr_vk_create_image(VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
  uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
  VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
  VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory) {

  VkImageCreateInfo imageInfo = { 0 };
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = mipLevels;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = numSamples;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(logicalDevice, &imageInfo, NULL, image) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to create image!"));
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(logicalDevice, *image, &memRequirements);

  VkMemoryAllocateInfo allocInfo = { 0 };
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = fr_vk_find_memory_type(physicalDevice, memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(logicalDevice, &allocInfo, NULL, imageMemory) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to allocate image memory!"));
  }

  vkBindImageMemory(logicalDevice, *image, *imageMemory, 0);
}
#endif

static void fr_vk_create_image_view(VkDevice logicalDevice,
  VkImage image,
  VkFormat format,
  VkImageAspectFlags aspectFlags,
  uint32_t mipLevels,
  VkImageView *imageView) {

  VkImageViewCreateInfo viewInfo = { 0 };
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
  viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
  viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
  viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;

  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = mipLevels;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(logicalDevice, &viewInfo, NULL, imageView) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to create texture image view!"));
  }
}

#if 0
static void fr_vk_create_bind_buffer(
  VkPhysicalDevice physicalDevice,
  VkDevice logicDevice, VkDeviceSize size,
  VkBufferUsageFlags usage,
  VkMemoryPropertyFlags properties,
  VkBuffer *buffer,
  VkDeviceMemory *bufferMemory) {
  VkBufferCreateInfo bufferInfo = { 0 };

  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(logicDevice, &bufferInfo, NULL, buffer) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to create buffer!"));
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(logicDevice, *buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo = { 0 };
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = fr_vk_find_memory_type(physicalDevice, memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(logicDevice, &allocInfo, NULL, bufferMemory) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to allocate buffer memory!"));
  }

  vkBindBufferMemory(logicDevice, *buffer, *bufferMemory, 0);
}
#endif

static void fr_vk_command_begin(VkCommandBuffer buffer, VkCommandBufferUsageFlags flags) {
  VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                      .pNext = NULL,
                      .flags = flags,
                      .pInheritanceInfo = NULL };

  fr_vk_check_result(vkBeginCommandBuffer(buffer, &beginInfo));
}

static void fr_vk_command_end(VkCommandBuffer buffer) {
  fr_vk_check_result(vkEndCommandBuffer(buffer));
}

static void fr_vk_set_image_layout(VkCommandBuffer cmdBuff, VkImage image,
  VkImageAspectFlags aspect_mask,
  VkImageLayout old_image_layout, VkImageLayout new_image_layout,
  VkPipelineStageFlags src_stages, VkPipelineStageFlags dest_stages) {

  VkImageMemoryBarrier barrier = { 0 };

  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = old_image_layout;
  barrier.newLayout = new_image_layout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;

  barrier.subresourceRange.aspectMask = aspect_mask;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  switch (old_image_layout) {
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      break;

    default:
      break;
  }

  switch (new_image_layout) {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;

    default:
      break;
  }

  vkCmdPipelineBarrier(cmdBuff,
      src_stages, dest_stages,
      0, 
      0, NULL, 
      0, NULL, 
      1, &barrier);
}

/*-------------------------------------------- vkvg part --------------------------------------------*/
static void fr_vkvg_create_swap_chain(FRVkvgInstanceT vinst) {
  uint32_t imageCount = 0;
  SwapChainSupportDetails *chainSupport = vinst->chainSupport;

  VkSurfaceFormatKHR surfaceFormat = fr_vk_choose_swap_surface_format(chainSupport->formats, chainSupport->formatCount);
  VkPresentModeKHR presentMode = fr_vk_choose_swap_present_mode(chainSupport->presentModes, chainSupport->presentModeCount);
  VkExtent2D extent = fr_vk_choose_swap_extent(&chainSupport->capabilities, vinst->width, vinst->height);

  imageCount = chainSupport->capabilities.minImageCount + 1;

  if (chainSupport->capabilities.maxImageCount > 0 && imageCount > chainSupport->capabilities.maxImageCount) {
    imageCount = chainSupport->capabilities.maxImageCount;
  }

  vkDeviceWaitIdle(vinst->logicDevice);

  fr_vk_create_swap_chain(vinst->vkSurface, vinst->physicalDevice, vinst->logicDevice,
    imageCount,
    surfaceFormat,
    extent,
    chainSupport->capabilities.currentTransform,
    presentMode,
    vinst->swapChain,
    &vinst->swapChain);

  fr_vk_check_result(vkGetSwapchainImagesKHR(vinst->logicDevice, vinst->swapChain, &imageCount, NULL));
  vinst->swapChainImages = sys_new0_N(VkImage, imageCount);
  fr_vk_check_result(vkGetSwapchainImagesKHR(vinst->logicDevice, vinst->swapChain, &imageCount, vinst->swapChainImages));

  vinst->imageCount = imageCount;
  vinst->swapChainImageFormat = surfaceFormat.format;
  vinst->swapChainExtent = extent;
  vinst->swapChainImageViews = sys_new0_N(VkImageView, imageCount);

  // swap chain buffers
  vinst->commandBuffers = sys_new0_N(VkCommandBuffer, imageCount);
  sys_array_foreach(VkImage, image, vinst->swapChainImages, imageCount) {

    fr_vk_create_image_view(vinst->logicDevice,
      image,
      vinst->swapChainImageFormat,
      VK_IMAGE_ASPECT_COLOR_BIT, 1,
      &vinst->swapChainImageViews[i]);

  fr_vk_create_command_buffer(vinst->logicDevice,
    vinst->commandPool,
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    &vinst->commandBuffers[i]);

  }
}

static void fr_vkvg_create_command_pool(FRVkvgInstanceT vinst) {
  VkCommandPool commandPool = NULL;

  VkCommandPoolCreateInfo poolInfo = { 0 };
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = (uint32_t)vinst->indices->graphicsFamily;

  if (vkCreateCommandPool(vinst->logicDevice, &poolInfo, NULL, &commandPool) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to create graphics command pool!"));
  }

  vinst->commandPool = commandPool;
}

static void fr_vkvg_create_semaphore_and_fence(FRVkvgInstanceT vinst) {
  VkSemaphoreCreateInfo info = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                   .pNext = NULL,
                   .flags = 0 };

  if (vkCreateSemaphore(vinst->logicDevice, &info, NULL, &vinst->presentSemaphore) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to create presentSemaphore"));
  }

  if (vkCreateSemaphore(vinst->logicDevice, &info, NULL, &vinst->drawSemaphore) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to create drawSemaphore!"));
  }

  VkFenceCreateInfo fenceInfo = { .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .pNext = NULL,
                .flags = VK_FENCE_CREATE_SIGNALED_BIT };
  if (vkCreateFence(vinst->logicDevice, &fenceInfo, NULL, &vinst->drawFence) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to create drawFence!"));
  }
}

static void fr_vkvg_create_logical_device(FRVkvgInstanceT vinst) {
  VkDeviceQueueCreateInfo queueCreateInfo = { 0 };
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = vinst->indices->graphicsFamily;
  queueCreateInfo.queueCount = 1;

  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures = { 0 };
  const SysChar* deviceExts[10] = { 0 };
  uint32_t deviceCount = 0;

  vkvg_get_required_device_extensions(vinst->physicalDevice, deviceExts, &deviceCount);
  const void *pNext = vkvg_get_device_requirements(&deviceFeatures);

  sys_array_foreach(const char * const, de, deviceExtensions, ARRAY_SIZE(deviceExtensions)) {
    deviceExts[deviceCount + i] = (SysChar *)de;
    deviceCount++;
  }

  VkDeviceCreateInfo createInfo = { 0 };
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.flags = 0;
  createInfo.pNext = pNext;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;

  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = deviceCount;
  createInfo.ppEnabledExtensionNames = deviceExts;


  if (vinst->enableLayer) {
    createInfo.enabledLayerCount = ARRAY_SIZE(validationLayers);
    createInfo.ppEnabledLayerNames = validationLayers;
  }
  else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(vinst->physicalDevice, &createInfo, NULL, &vinst->logicDevice) != VK_SUCCESS) {
    sys_abort_N("%s", SYS_("failed to create logical device!"));
  }

  vkGetDeviceQueue(vinst->logicDevice, vinst->indices->graphicsFamily, 0, &vinst->graphicsQueue);
  vkGetDeviceQueue(vinst->logicDevice, vinst->indices->presentFamily, 0, &vinst->presentQueue);

}

static void fr_vkvg_init_device_support(FRVkvgInstanceT vinst) {
  vinst->indices = fr_vk_find_queue_families(vinst->vkSurface, vinst->physicalDevice);
  vinst->chainSupport = fr_vk_query_swap_chain_support(vinst->vkSurface, vinst->physicalDevice);
}

static void fr_vkvg_pick_physical_device(FRVkvgInstanceT vinst) {
  vinst->physicalDevice = fr_vk_pick_physical_device(vinst->instance);

  fr_vkvg_init_device_support(vinst);
}

static VkResult fr_vk_create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
  const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

  PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != NULL) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

static void fr_vk_destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger, const VkAllocationCallbacks* pAllocator) {
  PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != NULL) {
    func(instance, pDebugMessenger, pAllocator);
  } else {
    return;
  }
}

static void fr_vkvg_setup_debug_messenger(FRVkvgInstanceT vinst) {
  if (!vinst->enableLayer) return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = { 0 };
  fr_vk_populate_debug_messenger_create_info(&createInfo);

  if (fr_vk_create_debug_utils_messenger_ext(vinst->instance, &createInfo, NULL, &vinst->debugMessenger) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to set up debug messenger!"));
  }
}

static void fr_vkvg_create_vk_instance(FRVkvgInstanceT vinst) {
  VkInstance vkinst = NULL;

  fr_vk_get_required_extension((const SysChar**)vinst->ppExtensions, &vinst->eCount);
  fr_vk_get_layer_extension(vinst->ppLayers, &vinst->layerCount);

  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = vinst->appName,
    .applicationVersion = 1,
    .pEngineName = vinst->engineName,
    .engineVersion = 1,
#ifdef VK_VERSION_1_2
    .apiVersion = VK_MAKE_API_VERSION(0, 1, 2, 0)
#else
    .apiVersion = VK_MAKE_VERSION(1, 0, 0) };
#endif
  };

  VkInstanceCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledExtensionCount = vinst->eCount,
    .ppEnabledExtensionNames = (const char *const *)vinst->ppExtensions,
    .enabledLayerCount = vinst->layerCount,
    .ppEnabledLayerNames = (const char *const *)vinst->ppLayers
  };

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = { 0 };
  fr_vk_populate_debug_messenger_create_info(&debugCreateInfo);
  create_info.pNext = (void *)&debugCreateInfo;

  fr_vk_check_result(vkCreateInstance(&create_info, NULL, &vkinst));

  vinst->instance = vkinst;
}

static void fr_vkvg_create_vk_surface(FRVkvgInstanceT vinst) {
  VkSurfaceKHR surfacekhr = NULL;
  fr_window_create_vk_surface(vinst->window, vinst->instance, &surfacekhr);
  vinst->vkSurface = surfacekhr;
}

static void fr_vkvg_v_record_command(FRVkvgInstanceT vinst) {
  VkImage src = vkvg_surface_get_vk_image(vinst->vkvgSurface);

  for (int i = 0; i < vinst->imageCount; i++) {
    VkImage dst = vinst->swapChainImages[i];
    VkCommandBuffer buffer = vinst->commandBuffers[i];

    fr_vk_command_begin(buffer, 0);

    fr_vk_set_image_layout(buffer, dst, VK_IMAGE_ASPECT_COLOR_BIT,
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    fr_vk_set_image_layout(buffer, src, VK_IMAGE_ASPECT_COLOR_BIT,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    VkImageBlit bregion = {
      .srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
      .dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
      .srcOffsets[0] = {0, 0, 0},
      .srcOffsets[1] = {vinst->width, vinst->height, 1},
      .dstOffsets[0] = {0, 0, 0},
      .dstOffsets[1] = {vinst->width, vinst->height, 1}
    };

    vkCmdBlitImage(buffer, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bregion, VK_FILTER_NEAREST);

    fr_vk_set_image_layout(buffer, dst, VK_IMAGE_ASPECT_COLOR_BIT,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

    fr_vk_set_image_layout(buffer, src, VK_IMAGE_ASPECT_COLOR_BIT,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    fr_vk_command_end(buffer);
  }
}

static VkvgSurface fr_vkvg_v_create_surface(FRVkvgInstanceT vinst) {
  return vkvg_surface_create(vinst->vkvgDevice, vinst->width, vinst->height);
}

#if 0
static VkvgContext fr_vkvg_v_create_context(FRVkvgInstanceT vinst) {
  vinst->vkvgContext = vkvg_create(vinst->vkvgSurface);
  return vinst->vkvgContext;
}
#endif

static void fr_vkvg_v_create_logic_device(FRVkvgInstanceT vinst) {
  static VkSampleCountFlags samples = VK_SAMPLE_COUNT_1_BIT;
  bool deferredResolve = false;

  vinst->vkvgDevice = vkvg_device_create_from_vk_multisample(vinst->instance, vinst->physicalDevice,
    vinst->logicDevice, (uint32_t)vinst->indices->graphicsFamily, 0, samples, deferredResolve);

  vkvg_device_set_dpy(vinst->vkvgDevice, 96, 96);
}

static void fr_vkvg_create_device(FRVkvgInstanceT vinst) {
  fr_vkvg_pick_physical_device(vinst);
  fr_vkvg_create_logical_device(vinst);
}

static void fr_vkvg_create_grahpics(FRVkvgInstanceT vinst) {
  fr_vkvg_create_command_pool(vinst);
  fr_vkvg_create_semaphore_and_fence(vinst);
  fr_vkvg_create_swap_chain(vinst);
}

/*-------------------------------------------- API part --------------------------------------------*/
void fr_vkvg_present(FRRenderInstanceT inst) {
  FRVkvgInstanceT vinst = (FRVkvgInstanceT)inst;

  uint16_t fence_timeout = UINT16_MAX;
  VkResult result = vkAcquireNextImageKHR(vinst->logicDevice, vinst->swapChain, UINT64_MAX,
    vinst->presentSemaphore, VK_NULL_HANDLE, &vinst->currentFrame);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    sys_abort_N("%s", SYS_("failed to acquire swap chain image!"));
  }

  VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkSubmitInfo submit_info = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                 .commandBufferCount = 1,
                 .signalSemaphoreCount = 1,
                 .pSignalSemaphores = &vinst->drawSemaphore,
                 .waitSemaphoreCount = 1,
                 .pWaitSemaphores = &vinst->presentSemaphore,
                 .pWaitDstStageMask = &dstStageMask,
                 .pCommandBuffers = &vinst->commandBuffers[vinst->currentFrame] };

  vkWaitForFences(vinst->logicDevice, 1, &vinst->drawFence, VK_TRUE, fence_timeout);
  vkResetFences(vinst->logicDevice, 1, &vinst->drawFence);

  fr_vk_check_result(vkQueueSubmit(vinst->presentQueue, 1, &submit_info, vinst->drawFence));

  VkPresentInfoKHR present = { .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                 .swapchainCount = 1,
                 .pSwapchains = &vinst->swapChain,
                 .waitSemaphoreCount = 1,
                 .pWaitSemaphores = &vinst->drawSemaphore,
                 .pImageIndices = &vinst->currentFrame };

  vkQueuePresentKHR(vinst->presentQueue, &present);
}

static FRSurfaceT fr_vkvg_create_surface(FRRenderInstanceT inst) {
  FRVkvgInstanceT vinst = (FRVkvgInstanceT)inst;

  vinst->vkvgSurface = fr_vkvg_v_create_surface(vinst);
  fr_vkvg_v_record_command(vinst);

  return vinst->vkvgSurface;
}

static FRRenderContextT fr_vkvg_create_context(FRRenderInstanceT inst) {
  FRVkvgInstanceT vinst = (FRVkvgInstanceT)inst;

  vinst->vkvgContext = vkvg_create(vinst->vkvgSurface);

  return (FRRenderContextT)vinst->vkvgContext;
}

static void fr_vkvg_free_instance(FRVkvgInstanceT vinst) {
  vkvg_device_destroy(vinst->vkvgDevice);

  for (int i = 0; i < vinst->imageCount; i++) {
    // VkImage image = vinst->swapChainImages[i];
    VkImageView imageView = vinst->swapChainImageViews[i];

    vkDestroyImageView(vinst->logicDevice, imageView, NULL);
    // vkDestroyImage(vinst->logicDevice, image, NULL);
  }

  vkDestroySwapchainKHR(vinst->logicDevice, vinst->swapChain, NULL);
  vkFreeCommandBuffers(vinst->logicDevice, vinst->commandPool, vinst->imageCount, vinst->commandBuffers);
  vkDestroySemaphore(vinst->logicDevice, vinst->drawSemaphore, NULL);
  vkDestroySemaphore(vinst->logicDevice, vinst->presentSemaphore, NULL);
  vkDestroyFence(vinst->logicDevice, vinst->drawFence, NULL);

  fr_vk_destroy_debug_utils_messenger_ext(vinst->instance, vinst->debugMessenger, NULL);
  vkDestroySurfaceKHR(vinst->instance, vinst->vkSurface, NULL);

  vkDestroyCommandPool(vinst->logicDevice, vinst->commandPool, NULL);
  vkDestroyDevice(vinst->logicDevice, NULL);

  vkDestroyInstance(vinst->instance, NULL);
  sys_free_N(vinst);
}

static FRRenderInstanceT fr_vkvg_create_instance(FRWindow *window) {
  FRVkvgInstanceT vinst = sys_new0_N(FRVkvgInstance, 1);

  vinst->appName = "FRApp";
  vinst->engineName = "FREngine";
  vinst->enableLayer = true;
  vinst->window = window;

  fr_window_get_size(window, (SysInt *)&vinst->width, (SysInt *)&vinst->height);

  // basic init
  fr_vkvg_create_vk_instance(vinst);
  fr_vkvg_setup_debug_messenger(vinst);
  fr_vkvg_create_vk_surface(vinst);
  fr_vkvg_create_device(vinst);
  fr_vkvg_create_grahpics(vinst);
  fr_vkvg_v_create_logic_device(vinst);

  return (FRRenderInstanceT)vinst;
}

FRRenderBackend *fr_vkvg_get_backend(void) {
  return &backend;
}

