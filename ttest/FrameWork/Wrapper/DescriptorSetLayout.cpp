#include "DescriptorSetLayout.h"
#include "FrameWork/Wrapper/DescriptorSetLayout.h"
#include "vulkan/vulkan_core.h"
namespace MoChengEngine::FrameWork::Wrapper {
DescriptorSetLayout::DescriptorSetLayout(Device::Ptr device)
    : Component<VkDescriptorSetLayout, DescriptorSetLayout>{device} {}
DescriptorSetLayout::~DescriptorSetLayout() {

  vkDestroyDescriptorSetLayout(m_device->Get_handle(), m_handle, nullptr);
}
void DescriptorSetLayout::Build(std::vector<UniformParameter::Ptr> params) {

  std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};

  for (const auto &param : params) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.descriptorType = param->mDescriptorType;
    layoutBinding.binding = param->mBinding;
    layoutBinding.stageFlags = param->mStage;
    layoutBinding.descriptorCount = param->mCount;
    layoutBindings.push_back(layoutBinding);
  }

  VkDescriptorSetLayoutCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  createInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
  createInfo.pBindings = layoutBindings.data();
  VK_CHECK_SUCCESS(vkCreateDescriptorSetLayout(m_device->Get_handle(),
                                               &createInfo, nullptr, &m_handle),
                   "Error: failed to create descriptor set layout");
}
} // namespace MoChengEngine::FrameWork::Wrapper
