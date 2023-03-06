#pragma once
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
#include "FrameWork/Wrapper/Device.h"

namespace MoChengEngine::FrameWork::Wrapper {
template <typename T, typename K> class Component : public WrapperBase<T, K> {
protected:
  Device::Ptr m_device;

public:
  Component<T, K>(Device::Ptr device) : m_device{device} {}
  [[nodiscard]] auto  Get_device() { return m_device; }
};
} // namespace MoChengEngine::FrameWork::Wrapper