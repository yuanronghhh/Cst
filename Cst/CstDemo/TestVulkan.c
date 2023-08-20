#include <CstDemo/TestVulkan.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define IDX_IS_COMPLETE(indices) (indices.graphicsFamily >= 0 && indices.presentFamily >= 0)
#define SWAPCHAIN_HAS_VALUE(swap) (swap.formatCount >= 0 && swap.presentModes >= 0)

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

typedef struct _QueueFamilyIndices QueueFamilyIndices;
typedef struct _SwapChainSupportDetails SwapChainSupportDetails;
typedef struct _Application Application;
typedef struct _Vertex Vertex;
typedef struct _UniformBufferObject UniformBufferObject;

struct _Application {
  VkExtensionProperties* instExtProps;
  uint32_t instExtCount;

  GLFWwindow* window;
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice;

  VkDevice device;
  VkQueue graphicsQueue;

  // window surface
  VkSurfaceKHR surface;
  VkQueue presentQueue;

  // swap chain
  VkSwapchainKHR swapChain;
  VkImage* swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  // imageView
  uint32_t imageCount;
  VkImageView* swapChainImageViews;

  // Pipeline
  VkPipelineLayout pipelineLayout;

  // RenderPass
  VkRenderPass renderPass;

  VkDescriptorSetLayout descriptorSetLayout;

  // graphics pipleline
  VkPipeline graphicsPipeline;

  // framebuffer
  VkFramebuffer *swapChainFramebuffers;

  // Command buffer
  VkCommandPool commandPool;
  VkCommandBuffer* commandBuffers;
  VkSemaphore* imageAvailableSemaphores;
  VkSemaphore* renderFinishedSemaphores;
  VkFence* inFlightFences;
  uint32_t currentFrame;

  // vertex buffer
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;

  // index buffer
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;

  VkBuffer* uniformBuffers;
  VkDeviceMemory* uniformBuffersMemory;

  VkDescriptorPool descriptorPool;
  VkDescriptorSet* descriptorSets;

  bool framebufferResized;

  // Hello Triangle
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
  VkFence inFlightFence;
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

struct _Vertex {
  vec2 pos;
  vec3 color;
};

#pragma pack(16)
struct  _UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
};

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
static SysInt64 startTime = 0;

const int MAX_FRAMES_IN_FLIGHT = 2;

static const char* validationLayers[] = {
  "VK_LAYER_KHRONOS_validation",
  // "VK_LAYER_RENDERDOC_Capture"
};

static const char* deviceExtensions[] = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const Vertex vertices[] = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const uint16_t vertIndices[] = {
  0, 1, 2, 2, 3, 0
};

static Application app;

void cst_vk_check_result(VkResult f) {
  VkResult res = (f);
  if (res != VK_SUCCESS) {
    sys_assert(res == VK_SUCCESS);
  }
}

static void getBindingDescription(VkVertexInputBindingDescription *bindingDescription) {
  bindingDescription->binding = 0;
  bindingDescription->stride = sizeof(Vertex);
  bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

static void getAttributeDescriptions(VkVertexInputAttributeDescription attributeDescriptions[], size_t len) {
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
  const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
  PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != NULL) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
  PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != NULL) {
    func(instance, debugMessenger, pAllocator);
  }
}

int array_find_string(char *array[], int size, char *name) {
  for (int i = 0; i < size; i++) {
    char *e = array[i];

    if (strcmp(e, name) == 0) {
      return i;
    }
  }

  return -1;
}

bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  
  VkLayerProperties *availableLayers = sys_new0_N(VkLayerProperties, layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  for (int i = 0; i < (int)ARRAY_SIZE(validationLayers); i++) {
    const char * layerName = validationLayers[i];

    bool layerFound = false;

    for (int i = 0; i < (int)layerCount; i++) {
      VkLayerProperties layerProperties = availableLayers[i];

      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  Application *app = (Application *)(glfwGetWindowUserPointer(window));
  app->framebufferResized = true;
}

void initWindow() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  app.window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
  
  glfwSetWindowUserPointer(app.window, &app);
  glfwSetFramebufferSizeCallback(app.window, framebufferResizeCallback);
}

static void extension_support_init() {
  cst_vk_check_result(vkEnumerateInstanceExtensionProperties(NULL, &app.instExtCount, NULL));
  app.instExtProps = sys_new0_N(VkExtensionProperties, app.instExtCount);
  cst_vk_check_result(vkEnumerateInstanceExtensionProperties(NULL, &app.instExtCount, app.instExtProps));

#if 0
  for(int i = 0; i < (int)instExtCount; i++) {
    VkExtensionProperties prop = instExtProps[i];

    g_print("%s\n", prop.extensionName);
  };
#endif
}

static bool extension_support_check(char *name) {
  for (uint32_t i = 0; i < app.instExtCount; i++) {
    if (!strcmp(app.instExtProps[i].extensionName, name))
      return true;
  }
  return false;
}

void getRequiredExtensions(char ***extensions, SysInt *count) {
  uint32_t ec = 0;
  const char** gexts;
  char **nexts = NULL;
  gexts = glfwGetRequiredInstanceExtensions(&ec);

  nexts = sys_new0_N(char *, ec + 1);
  memcpy(nexts, gexts, sizeof(char *) * ec);

  if (enableValidationLayers) {
    if (extension_support_check(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
      nexts[ec++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }
  }

  *count = ec;
  *extensions = nexts;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL cst_vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  if (!(messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)) {
    return VK_TRUE;
  }

  sys_error_N("validation layer: %s", pCallbackData->pMessage);

  return VK_FALSE;
}

void cst_vk_populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *createInfo) {
  createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo->pfnUserCallback = cst_vk_debug_callback;
}

void createInstance() {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    sys_error_N("%s", "validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo = { 0 };
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = { 0 };
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  const char * const* extensions = NULL;
  SysInt extCount = 0;

  getRequiredExtensions((char ***)&extensions, &extCount);

  createInfo.enabledExtensionCount = extCount;
  createInfo.ppEnabledExtensionNames = extensions;

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = { 0 };

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = ARRAY_SIZE(validationLayers);
    createInfo.ppEnabledLayerNames = validationLayers;

    cst_vk_populate_debug_messenger_create_info(&debugCreateInfo);
    createInfo.pNext = (void *)&debugCreateInfo;

  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = NULL;
  }

  if (vkCreateInstance(&createInfo, NULL, &app.instance) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create instance!");
  }
}

void setupDebugMessenger() {
  if (!enableValidationLayers) return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = { 0 };
  cst_vk_populate_debug_messenger_create_info(&createInfo);

  if (CreateDebugUtilsMessengerEXT(app.instance, &createInfo, NULL, &app.debugMessenger) != VK_SUCCESS) {
    sys_error_N("%s", "failed to set up debug messenger!");
  }
}

QueueFamilyIndices cst_vk_find_queue_families(VkPhysicalDevice device) {
  QueueFamilyIndices indices = { -1, -1 };

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
  VkQueueFamilyProperties *queueFamilies = sys_new0_N(VkQueueFamilyProperties, queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

  for(int i = 0; i < (int)queueFamilyCount; i++) {
    VkQueueFamilyProperties queueFamily = queueFamilies[i];

    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, app.surface, &presentSupport);

    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (IDX_IS_COMPLETE(indices)) {
      break;
    }
  }

  return indices;
}

bool cst_vk_check_device_extension_support(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
  VkExtensionProperties *availableExtensions = sys_new0_N(VkExtensionProperties, extensionCount);
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

  bool support = true;

  for (int i = 0; i < (int)ARRAY_SIZE(deviceExtensions); i++) {
    const char * de = deviceExtensions[i];

    bool exist = false;
    for (int i = 0; i < (int)extensionCount; i++) {
      VkExtensionProperties extension = availableExtensions[i];

      if (strcmp(extension.extensionName, de) == 0) {
        exist = true;
        break;
      }
    }

    if (exist == false) {
      support = false;
      break;
    }
  }

  return support;
}

void cst_vk_query_swap_chain_support(VkPhysicalDevice device, SwapChainSupportDetails *details) {
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, app.surface, &details->capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, app.surface, &formatCount, NULL);

  if (formatCount != 0) {
    details->formats = sys_new0_N(VkSurfaceFormatKHR, formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, app.surface, &formatCount, details->formats);
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, app.surface, &presentModeCount, NULL);

  if (presentModeCount != 0) {
    details->presentModes = sys_new0_N(VkPresentModeKHR, presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, app.surface, &presentModeCount, details->presentModes);
  }

  details->formatCount = formatCount;
  details->presentModeCount = presentModeCount;
}

bool isDeviceSuitable(VkPhysicalDevice device) {
  QueueFamilyIndices indices = cst_vk_find_queue_families(device);

  bool extensionsSupported = cst_vk_check_device_extension_support(device);
  SwapChainSupportDetails swapChainSupport = { 0 };
  swapChainSupport.formatCount = -1;
  swapChainSupport.presentModeCount = -1;

  if (extensionsSupported) {
    cst_vk_query_swap_chain_support(device, &swapChainSupport);
  }

  return IDX_IS_COMPLETE(indices) && extensionsSupported && SWAPCHAIN_HAS_VALUE(swapChainSupport);
}

void pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(app.instance, &deviceCount, NULL);

  if (deviceCount == 0) {
    sys_error_N("%s", "failed to find GPUs with Vulkan support!");
  }

  VkPhysicalDevice *devices = sys_new0_N(VkPhysicalDevice, deviceCount);
  vkEnumeratePhysicalDevices(app.instance, &deviceCount, devices);

  for(int i = 0; i < (int)deviceCount; i++) {
    VkPhysicalDevice device = devices[i];

    if (isDeviceSuitable(device)) {
      app.physicalDevice = device;
      break;
    }
  }

  if (app.physicalDevice == VK_NULL_HANDLE) {
    sys_error_N("%s", "failed to find a suitable GPU!");
  }
}

void createLogicalDevice() {
  QueueFamilyIndices indices = cst_vk_find_queue_families(app.physicalDevice);

  VkDeviceQueueCreateInfo queueCreateInfo = { 0 };
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
  queueCreateInfo.queueCount = 1;

  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures = { 0 };

  VkDeviceCreateInfo createInfo = { 0 };
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = ARRAY_SIZE(deviceExtensions);
  createInfo.ppEnabledExtensionNames = deviceExtensions;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = ARRAY_SIZE(validationLayers);
    createInfo.ppEnabledLayerNames = validationLayers;
  }
  else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(app.physicalDevice, &createInfo, NULL, &app.device) != VK_SUCCESS) {
    sys_error_N("%s","failed to create logical device!");
  }

  vkGetDeviceQueue(app.device, indices.graphicsFamily, 0, &app.graphicsQueue);
  vkGetDeviceQueue(app.device, indices.presentFamily, 0, &app.presentQueue);
}

void createSurface() {
  if (glfwCreateWindowSurface(app.instance, app.window, NULL, &app.surface) != VK_SUCCESS) {
    sys_error_N("%s","failed to create window surface!");
  }
}

VkSurfaceFormatKHR cst_vk_choose_swap_surface_format(const VkSurfaceFormatKHR *availableFormats, uint32_t formatCount) {
  for(int i = 0; i < (int)formatCount; i++) {
    VkSurfaceFormatKHR availableFormat = availableFormats[i];

    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

    return availableFormats[0];
}

VkPresentModeKHR cst_vk_choose_swap_present_mode(const VkPresentModeKHR *availablePresentModes, uint32_t presentModeCount) {
  for(int i = 0; i < (int)presentModeCount; i++) {
    VkPresentModeKHR availablePresentMode = availablePresentModes[i];

    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D cst_vk_choose_swap_extent(const VkSurfaceCapabilitiesKHR *capabilities) {
  if (capabilities->currentExtent.width != UINT32_MAX) {
    return capabilities->currentExtent;
  }
  else {
    int width, height;
    glfwGetFramebufferSize(app.window, &width, &height);

    VkExtent2D actualExtent = { width, height };

    actualExtent.width = CLAMP(actualExtent.width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
    actualExtent.height = CLAMP(actualExtent.height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);

    return actualExtent;
  }
}

void createSwapChain() {
  SwapChainSupportDetails swapChainSupport = { 0 };
  swapChainSupport.formatCount = -1;
  swapChainSupport.presentModeCount = -1;

  cst_vk_query_swap_chain_support(app.physicalDevice, &swapChainSupport);

  VkSurfaceFormatKHR surfaceFormat = cst_vk_choose_swap_surface_format(swapChainSupport.formats, swapChainSupport.formatCount);
  VkPresentModeKHR presentMode = cst_vk_choose_swap_present_mode(swapChainSupport.presentModes, swapChainSupport.presentModeCount);
  VkExtent2D extent = cst_vk_choose_swap_extent(&swapChainSupport.capabilities);

  app.imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 && app.imageCount > swapChainSupport.capabilities.maxImageCount) {
    app.imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = { 0 };
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = app.surface;

  createInfo.minImageCount = app.imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = cst_vk_find_queue_families(app.physicalDevice);
  uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;

  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  // createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(app.device, &createInfo, NULL, &app.swapChain) != VK_SUCCESS) {
    sys_error_N("%s","failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(app.device, app.swapChain, &app.imageCount, NULL);
  app.swapChainImages = sys_new0_N(VkImage, app.imageCount);
  vkGetSwapchainImagesKHR(app.device, app.swapChain, &app.imageCount, app.swapChainImages);

  app.swapChainImageFormat = surfaceFormat.format;
  app.swapChainExtent = extent;
}

void createImageViews() {
  app.swapChainImageViews = sys_new0_N(VkImageView, app.imageCount);

  for (size_t i = 0; i < app.imageCount; i++) {
    VkImageViewCreateInfo createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = app.swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = app.swapChainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(app.device, &createInfo, NULL, &app.swapChainImageViews[i]) != VK_SUCCESS) {
      sys_error_N("%s","failed to create image views!");
    }
  }
}

void readFile(char *filename, char **content, size_t* length) {
  SysError *error = NULL;
  if (!sys_file_get_contents(filename, content, length, &error)) {
    sys_error_N("%s", "failed to read sharder %s", filename);
  }
}

VkShaderModule createShaderModule(const char* code, size_t size) {
  VkShaderModuleCreateInfo createInfo = { 0 };
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = size;
  createInfo.pCode = (const uint32_t *)code;

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(app.device, &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create shader module!");
  }

  return shaderModule;
}

void createGraphicsPipeline() {
  char* vertShaderCode = NULL;
  char* fragShaderCode = NULL;
  size_t vertShaderCodeLen = 0;
  size_t fragShaderCodeLen = 0;

  readFile(PROJECT_DIR"/shaders/22_shader_ubo_vert.spv", &vertShaderCode, &vertShaderCodeLen);
  readFile(PROJECT_DIR"/shaders/22_shader_ubo_frag.spv", &fragShaderCode, &fragShaderCodeLen);

  VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, vertShaderCodeLen);
  VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, fragShaderCodeLen);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = { 0 };
  vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";
  vertShaderStageInfo.pSpecializationInfo = NULL;

  VkPipelineShaderStageCreateInfo fragShaderStageInfo = { 0 };
  fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";
  fragShaderStageInfo.pSpecializationInfo = NULL;

  VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = { 0 };
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  VkVertexInputBindingDescription bindingDescription = { 0 };
  VkVertexInputAttributeDescription attributeDescriptions[2] = { 0 };

  getAttributeDescriptions(attributeDescriptions, 2);
  getBindingDescription(&bindingDescription);

  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount = ARRAY_SIZE(attributeDescriptions);
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = { 0 };
  inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport = { 0 };
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)app.swapChainExtent.width;
  viewport.height = (float)app.swapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = { 0 };
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  scissor.extent = app.swapChainExtent;

  VkPipelineViewportStateCreateInfo viewportState = { 0 };
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer = { 0 };
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling = { 0 };
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState colorBlendAttachment = { 0 };
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending = { 0 };
  colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = { 0 };
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &app.descriptorSetLayout;

  if (vkCreatePipelineLayout(app.device, &pipelineLayoutInfo, NULL, &app.pipelineLayout) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create pipeline layout!");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = { 0 };
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = app.pipelineLayout;
  pipelineInfo.renderPass = app.renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(app.device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &app.graphicsPipeline) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create graphics pipeline!");
  }

  vkDestroyShaderModule(app.device, fragShaderModule, NULL);
  vkDestroyShaderModule(app.device, vertShaderModule, NULL);
}

void createRenderPass() {
  VkAttachmentDescription colorAttachment = { 0 };
  colorAttachment.format = app.swapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = { 0 };
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = { 0 };
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkSubpassDependency dependency = { 0 };
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo = { 0 };
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(app.device, &renderPassInfo, NULL, &app.renderPass) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create render pass!");
  }
}

void createFramebuffers() {
  app.swapChainFramebuffers = sys_new0_N(VkFramebuffer, app.imageCount);

  for (size_t i = 0; i < app.imageCount; i++) {
    VkImageView attachments[] = {
      app.swapChainImageViews[i]
    };

    VkFramebufferCreateInfo framebufferInfo = { 0 };
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = app.renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = app.swapChainExtent.width;
    framebufferInfo.height = app.swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(app.device, &framebufferInfo, NULL, &app.swapChainFramebuffers[i]) != VK_SUCCESS) {
      sys_error_N("%s", "failed to create framebuffer!");
    }
  }
}

void createCommandPool() {
  QueueFamilyIndices queueFamilyIndices = cst_vk_find_queue_families(app.physicalDevice);

  VkCommandPoolCreateInfo poolInfo = { 0 };
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

  if (vkCreateCommandPool(app.device, &poolInfo, NULL, &app.commandPool) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create graphics command pool!");
  }
}

void createCommandBuffers() {
  app.commandBuffers = sys_new0_N(VkCommandBuffer, MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo = { 0 };
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = app.commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)MAX_FRAMES_IN_FLIGHT; //  commandBuffers size

  if (vkAllocateCommandBuffers(app.device, &allocInfo, app.commandBuffers) != VK_SUCCESS) {
    sys_error_N("%s", "failed to allocate command buffers!");
  }
}

void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo = { 0 };
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    sys_error_N("%s", "failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo = { 0 };
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = app.renderPass;
  renderPassInfo.framebuffer = app.swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset.x = 0;
  renderPassInfo.renderArea.offset.y = 0;
  renderPassInfo.renderArea.extent = app.swapChainExtent;

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, app.graphicsPipeline);
    VkBuffer vertexBuffers[] = {app.vertexBuffer};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, app.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, app.pipelineLayout, 0, 1, &app.descriptorSets[app.currentFrame], 0, NULL);
    vkCmdDrawIndexed(commandBuffer, (uint32_t)ARRAY_SIZE(vertIndices), 1, 0, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    sys_error_N("%s", "failed to record command buffer!");
  }
}

void createSyncObjects() {
  app.imageAvailableSemaphores = sys_new0_N(VkSemaphore, MAX_FRAMES_IN_FLIGHT);
  app.renderFinishedSemaphores = sys_new0_N(VkSemaphore, MAX_FRAMES_IN_FLIGHT);
  app.inFlightFences = sys_new0_N(VkFence, MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo = { 0 };
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo = { 0 };
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(app.device, &semaphoreInfo, NULL, &app.imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(app.device, &semaphoreInfo, NULL, &app.renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(app.device, &fenceInfo, NULL, &app.inFlightFences[i]) != VK_SUCCESS) {

      sys_error_N("%s", "failed to create synchronization objects for a frame!");
    }
  }
}

void cleanupSwapChain() {
  for(int i = 0; i < (int)app.imageCount; i++) {
    VkFramebuffer framebuffer = app.swapChainFramebuffers[i];

    vkDestroyFramebuffer(app.device, framebuffer, NULL);
  }

  vkDestroyPipeline(app.device, app.graphicsPipeline, NULL);
  vkDestroyPipelineLayout(app.device, app.pipelineLayout, NULL);
  vkDestroyRenderPass(app.device, app.renderPass, NULL);

  for(int i = 0; i < (int)app.imageCount; i++) {
    VkImageView imageView = app.swapChainImageViews[i];

    vkDestroyImageView(app.device, imageView, NULL);
  }

  vkDestroySwapchainKHR(app.device, app.swapChain, NULL);
}

void recreateSwapChain() {
  int width = 0, height = 0;
  glfwGetFramebufferSize(app.window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(app.window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(app.device);

  cleanupSwapChain();

  createSwapChain();
  createImageViews();
  createRenderPass();
  createGraphicsPipeline();
  createFramebuffers();
}

void updateUniformBuffer(uint32_t currentImage) {
  if (startTime == 0) {
    startTime = g_get_monotonic_time();
  }

  gint64 currentTime = g_get_monotonic_time();
  float time = (currentTime - startTime) / 1000000.0f;

  UniformBufferObject ubo = { 0 };

  glm_rotate(ubo.model, time * glm_rad(90.0f), (vec3) { 0.0f, 0.0f, 1.0f });
  glm_look((vec3) { 2.0f, 2.0f, 2.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 1.0f }, ubo.view);
  glm_perspective(glm_rad(45.0f), app.swapChainExtent.width / (float)app.swapChainExtent.height, 0.1f, 10.0f, ubo.proj);
  ubo.proj[1][1] *= -1;

#if 0
    UniformBufferObject ubo{};
    glm_rotate(ubo.model, time * glm_rad(90.0f), vec3{ 0.0f, 0.0f, 1.0f });
    glm_look(vec3{ 2.0f, 2.0f, 2.0f }, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 0.0f, 0.0f, 1.0f }, ubo.view);
    glm_perspective(glm_rad(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f, ubo.proj);
    ubo.proj[1][1] *= -1;
#endif

  void* data;
  vkMapMemory(app.device, app.uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(app.device, app.uniformBuffersMemory[currentImage]);
}

void drawFrame() {
  vkWaitForFences(app.device, 1, &app.inFlightFences[app.currentFrame], VK_TRUE, UINT64_MAX);

  uint32_t imageIndex;

  VkResult result = vkAcquireNextImageKHR(app.device, app.swapChain, UINT64_MAX, 
    app.imageAvailableSemaphores[app.currentFrame], VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    sys_error_N("%s", "failed to acquire swap chain image!");
  }

  updateUniformBuffer(app.currentFrame);

  vkResetFences(app.device, 1, &app.inFlightFences[app.currentFrame]);

  vkResetCommandBuffer(app.commandBuffers[app.currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
  recordCommandBuffer(app.commandBuffers[app.currentFrame], imageIndex);
  
  VkSubmitInfo submitInfo = { 0 };
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { app.imageAvailableSemaphores[app.currentFrame]};
  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submitInfo.waitSemaphoreCount = 1;

  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &app.commandBuffers[app.currentFrame];

  VkSemaphore signalSemaphores[] = { app.renderFinishedSemaphores[app.currentFrame] };
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(app.graphicsQueue, 1, &submitInfo, app.inFlightFences[app.currentFrame]) != VK_SUCCESS) {
    sys_error_N("%s", "failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo = { 0 };
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = { app.swapChain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(app.presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || app.framebufferResized) {
    app.framebufferResized = false;
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    sys_error_N("%s", "failed to present swap chain image!");
  }

  app.currentFrame = (app.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void initApp () {
  extension_support_init();
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(app.physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  sys_error_N("%s", "failed to find suitable memory type!");
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *bufferMemory) {
  VkBufferCreateInfo bufferInfo = { 0 };
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(app.device, &bufferInfo, NULL, buffer) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(app.device, *buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo = { 0 };
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(app.device, &allocInfo, NULL, bufferMemory) != VK_SUCCESS) {
    sys_error_N("%s", "failed to allocate buffer memory!");
  }

  vkBindBufferMemory(app.device, *buffer, *bufferMemory, 0);
}

void createUniformBuffers() {
  VkDeviceSize bufferSize = sizeof(UniformBufferObject);

  app.uniformBuffers = sys_new0_N(VkBuffer, MAX_FRAMES_IN_FLIGHT);
  app.uniformBuffersMemory = sys_new0_N(VkDeviceMemory, MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
      &app.uniformBuffers[i], &app.uniformBuffersMemory[i]);
  }
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
  VkCommandBufferAllocateInfo allocInfo = { 0 };
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = app.commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(app.device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo = { 0 };
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion = { 0 };
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo = { 0 };
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(app.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(app.graphicsQueue);

  vkFreeCommandBuffers(app.device, app.commandPool, 1, &commandBuffer);
}

void createVertexBuffer() {
  VkDeviceSize bufferSize = sizeof(Vertex) * ARRAY_SIZE(vertices);

  VkBuffer stagingBuffer = NULL;
  VkDeviceMemory stagingBufferMemory = NULL;
  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
    &stagingBuffer, &stagingBufferMemory);

  void* data;
  vkMapMemory(app.device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices, (size_t) bufferSize);
  vkUnmapMemory(app.device, stagingBufferMemory);

  createBuffer(bufferSize, 
    VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
    &app.vertexBuffer, &app.vertexBufferMemory);

  copyBuffer(stagingBuffer, app.vertexBuffer, bufferSize);

  vkDestroyBuffer(app.device, stagingBuffer, NULL);
  vkFreeMemory(app.device, stagingBufferMemory, NULL);
}

void createIndexBuffer() {
  VkDeviceSize bufferSize = sizeof(Vertex) * ARRAY_SIZE(vertIndices);

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
      &stagingBuffer, &stagingBufferMemory);

  void* data;
  vkMapMemory(app.device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertIndices, (size_t) bufferSize);
  vkUnmapMemory(app.device, stagingBufferMemory);

  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
      &app.indexBuffer, &app.indexBufferMemory);

  copyBuffer(stagingBuffer, app.indexBuffer, bufferSize);

  vkDestroyBuffer(app.device, stagingBuffer, NULL);
  vkFreeMemory(app.device, stagingBufferMemory, NULL);
}

void createDescriptorSetLayout() {
  VkDescriptorSetLayoutBinding uboLayoutBinding = { 0 };
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.pImmutableSamplers = NULL;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutCreateInfo layoutInfo = { 0 };
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = 1;
  layoutInfo.pBindings = &uboLayoutBinding;

  if (vkCreateDescriptorSetLayout(app.device, &layoutInfo, NULL, &app.descriptorSetLayout) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create descriptor set layout!");
  }
}

void createDescriptorPool() {
  VkDescriptorPoolSize poolSize = {0};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = (uint32_t)(MAX_FRAMES_IN_FLIGHT);

  VkDescriptorPoolCreateInfo poolInfo = {0};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.maxSets = (uint32_t)(MAX_FRAMES_IN_FLIGHT);

  if (vkCreateDescriptorPool(app.device, &poolInfo, NULL, &app.descriptorPool) != VK_SUCCESS) {
    sys_error_N("%s", "failed to create descriptor pool!");
  }
}

void createDescriptorSets() {
  VkDescriptorSetLayout *layouts = sys_new0_N(VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    layouts[i] = app.descriptorSetLayout;
  }

  VkDescriptorSetAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = app.descriptorPool;
  allocInfo.descriptorSetCount = (uint32_t)(MAX_FRAMES_IN_FLIGHT);
  allocInfo.pSetLayouts = layouts;

  app.descriptorSets = sys_new0_N(VkDescriptorSet, MAX_FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(app.device, &allocInfo, app.descriptorSets) != VK_SUCCESS) {
    sys_error_N("%s", "failed to allocate descriptor sets!");
  }

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    VkDescriptorBufferInfo bufferInfo = {0};
    bufferInfo.buffer = app.uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite = {0};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = app.descriptorSets[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(app.device, 1, &descriptorWrite, 0, NULL);
  }
}

void initVulkan() {
  initApp();

  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createRenderPass();
  createDescriptorSetLayout();
  createGraphicsPipeline();
  createFramebuffers();
  createCommandPool();
  createVertexBuffer();
  createIndexBuffer();
  createUniformBuffers();
  createDescriptorPool();
  createDescriptorSets();
  createCommandBuffers();
  createSyncObjects();
}

void mainLoop() {
  while (!glfwWindowShouldClose(app.window)) {

    glfwWaitEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(app.device);
}

void cleanup() {
  cleanupSwapChain();

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroyBuffer(app.device, app.uniformBuffers[i], NULL);
    vkFreeMemory(app.device, app.uniformBuffersMemory[i], NULL);
  }

  vkDestroyDescriptorPool(app.device, app.descriptorPool, NULL);

  vkDestroyDescriptorSetLayout(app.device, app.descriptorSetLayout, NULL);

  vkDestroyBuffer(app.device, app.indexBuffer, NULL);
  vkFreeMemory(app.device, app.indexBufferMemory, NULL);

  vkDestroyBuffer(app.device, app.vertexBuffer, NULL);
  vkFreeMemory(app.device, app.vertexBufferMemory, NULL);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(app.device, app.renderFinishedSemaphores[i], NULL);
    vkDestroySemaphore(app.device, app.imageAvailableSemaphores[i], NULL);
    vkDestroyFence(app.device, app.inFlightFences[i], NULL);
  }

  vkDestroyCommandPool(app.device, app.commandPool, NULL);

  vkDestroyDevice(app.device, NULL);

  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(app.instance, app.debugMessenger, NULL);
  }

  vkDestroySurfaceKHR(app.instance, app.surface, NULL);
  vkDestroyInstance(app.instance, NULL);

  glfwDestroyWindow(app.window);
  glfwTerminate();
}

static void run(void) {
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

void test_vulkan_run(void) {
  UNITY_BEGIN();
  {
    RUN_TEST(run);
  }
  UNITY_END();
}
