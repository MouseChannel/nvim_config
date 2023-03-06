/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-22 14:08:41
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-19 19:53:54
 * @FilePath: \MoChengEngine\G_Example\model.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Core/UniformManager/UniformManager.h"
#include "FrameWork/Wrapper/Buffer.h"
#include "FrameWork/Wrapper/Command/CommandBuffer.h"
#include "FrameWork/Wrapper/Device.h"
#include "tools/tiny_obj_loader.h"

namespace MoChengEngine::FrameWork {
struct Vertex {
  glm::vec3 mPosition;
  glm::vec3 mColor;
};

class Model {

private:
  std::vector<float> mPositions{};
  std::vector<float> mColors{};
  std::vector<unsigned int> mIndexDatas{};
  std::vector<float> mUVs{};
  Wrapper::Device::Ptr m_device{nullptr};
  Wrapper::Buffer::Ptr mPositionBuffer{nullptr};
  Wrapper::Buffer::Ptr mColorBuffer{nullptr};
  Wrapper::Buffer::Ptr mUVBuffer{nullptr};

  Wrapper::Buffer::Ptr mIndexBuffer{nullptr};
  ObjectUniform m_Uniform{};
  float mAngle{0.0f};

public:
  using Ptr = std::shared_ptr<Model>;
  static Ptr Create(const Wrapper::Device::Ptr device) {
    return std::make_shared<Model>(device);
  }
  Model(const Wrapper::Device::Ptr device) : m_device{device} {
    // m_Uniform.mModelMatrix = glm::rotate(glm::mat4(1.0f),
    // glm::radians(-90.0f),
    //                                      glm::vec3(1.0f, 0.0f, 0.0f));
  }

  std::vector<VkVertexInputBindingDescription>
  getVertexInputBindingDescriptions();
  std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

  [[nodiscard]] std::vector<VkBuffer> getVertexBuffers() const {
    return {mPositionBuffer->Get_handle(), mUVBuffer->Get_handle()};
  }

  [[nodiscard]] auto getIndexBuffer() const { return mIndexBuffer; }

  [[nodiscard]] auto getIndexCount() const { return mIndexDatas.size(); }
  [[nodiscard]] auto getUniform() const { return m_Uniform; }

  void setModelMatrix(const glm::mat4 matrix) {
    m_Uniform.mModelMatrix = matrix;
  }
  void update();

  void loadModel(const std::string &path, const Wrapper::Device::Ptr &device,
                 Wrapper::CommandBuffer::Ptr command);
};
} // namespace MoChengEngine::FrameWork