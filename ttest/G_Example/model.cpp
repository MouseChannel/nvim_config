/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-12-09 11:31:33
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 19:36:35
 * @FilePath: \MoChengEngine\G_Example\model.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "model.h"
#include "FrameWork/Wrapper/Buffer.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "vulkan/vulkan_core.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tools/tiny_obj_loader.h"

namespace MoChengEngine::FrameWork {

std::vector<VkVertexInputBindingDescription>
Model::getVertexInputBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDes{};

  bindingDes.resize(2);

  bindingDes[0].binding = 0;
  bindingDes[0].stride = sizeof(float) * 3;
  bindingDes[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  bindingDes[1].binding = 1;
  bindingDes[1].stride = sizeof(float) * 2;
  bindingDes[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDes;
}
std::vector<VkVertexInputAttributeDescription>
Model::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDes{};
  attributeDes.resize(2);

  attributeDes[0].binding = 0;
  attributeDes[0].location = 0;
  attributeDes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  // attributeDes[0].offset = offsetof(Vertex, mPosition);
  attributeDes[0].offset = 0;
  //
  attributeDes[1].binding = 1;
  attributeDes[1].location = 2;
  attributeDes[1].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDes[1].offset = 0;
  return attributeDes;
}

void Model::update() {
  glm::mat4 rotateMatrix = glm::mat4(1.0f);
  rotateMatrix = glm::rotate(rotateMatrix, glm::radians(mAngle),
                             glm::vec3(0.0f, 0.0f, 1.0f));
  m_Uniform.mModelMatrix = rotateMatrix;

  mAngle += 0.05f;
}
void Model::loadModel(const std::string &path,
                      const Wrapper::Device::Ptr &device,
                      Wrapper::CommandBuffer::Ptr command_buffer) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;
  std::string warn;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                        path.c_str())) {
    throw std::runtime_error("Error: failed to load model");
  }

  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {

      mPositions.push_back(attrib.vertices[3 * index.vertex_index + 0]);
      mPositions.push_back(attrib.vertices[3 * index.vertex_index + 1]);
      mPositions.push_back(attrib.vertices[3 * index.vertex_index + 2]);

      mUVs.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
      mUVs.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);

      mIndexDatas.push_back(mIndexDatas.size());
    }
  }
  std::cout << mPositions.size() << std::endl;

  mPositionBuffer = Wrapper::Buffer::Create_GPU_Only_Buffer(
      device, mPositions.data(), mPositions.size() * sizeof(float),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      command_buffer);

  //   mPositionBuffer = Wrapper::Buffer::CreateR(
  //       device, mPositions.size() * sizeof(float),
  //       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
  //       VMA_MEMORY_USAGE_GPU_ONLY, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  //   mPositionBuffer->Update(mPositions.data(), mPositions.size() *
  //   sizeof(float));
  mUVBuffer = Wrapper::Buffer::Create_GPU_Only_Buffer(
      device, mUVs.data(), mUVs.size() * sizeof(float),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      command_buffer);
  //   mUVBuffer = Wrapper::Buffer::CreateR(
  //       device, mUVs.size() * sizeof(float),
  //       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
  //       VMA_MEMORY_USAGE_GPU_ONLY, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  //   mUVBuffer->Update(mUVs.data(), mUVs.size() * sizeof(float));
  mIndexBuffer = Wrapper::Buffer::Create_GPU_Only_Buffer(
      device, mIndexDatas.data(), mIndexDatas.size() * sizeof(float),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      command_buffer);

  //   mIndexBuffer = Wrapper::Buffer::CreateR(
  //       device, mIndexDatas.size() * sizeof(float),
  //       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
  //       VMA_MEMORY_USAGE_GPU_ONLY, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  //   mIndexBuffer->Update(mIndexDatas.data(), mIndexDatas.size() *
  //   sizeof(float));
}

} // namespace MoChengEngine::FrameWork