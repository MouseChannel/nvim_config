/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-19 14:59:53
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-19 16:25:35
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\ShaderModule.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

#include "ShaderModule.h"
#include <fstream>
namespace MoChengEngine::FrameWork::Wrapper {

static std::vector<char> ReadBinary(const std::string &fileName) {
  std::ifstream file(fileName.c_str(),
                     std::ios::ate | std::ios::binary | std::ios::in);

  if (!file) {
    throw std::runtime_error("Error: failed to open shader file");
  }

  const size_t fileSize = file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}
ShaderModule::ShaderModule(Device::Ptr device, const std::string &fileName,
                           VkShaderStageFlagBits shaderStage,
                           const std::string &entryPoint)
    : Component<VkShaderModule, ShaderModule>{device},
      m_ShaderStage{shaderStage}, m_EntryPoint{entryPoint} {

  std::vector<char> codeBuffer = ReadBinary(fileName);

  VkShaderModuleCreateInfo shaderCreateInfo{};
  shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderCreateInfo.codeSize = codeBuffer.size();
  shaderCreateInfo.pCode =
      reinterpret_cast<const uint32_t *>(codeBuffer.data());

  VK_CHECK_SUCCESS(vkCreateShaderModule(m_device->Get_handle(),
                                        &shaderCreateInfo, nullptr, &m_handle),
                   "Error: failed to create shader");
}
ShaderModule::~ShaderModule() {
  vkDestroyShaderModule(m_device->Get_handle(), m_handle, nullptr);
}

VkPipelineShaderStageCreateInfo ShaderModule::Make_Createinfo_in_pipeline() {

  VkPipelineShaderStageCreateInfo shaderCreateInfo{};
  shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderCreateInfo.stage = m_ShaderStage;
  shaderCreateInfo.pName = m_EntryPoint.c_str();
  shaderCreateInfo.module = m_handle;
  return shaderCreateInfo;
}
} // namespace MoChengEngine::FrameWork::Wrapper