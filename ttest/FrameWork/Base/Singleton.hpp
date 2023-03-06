#pragma once
#include <functional>
#include <memory>
#include <utility>


template <typename T> class Singleton {
private:
  bool hasInit;

protected:
  void Init(auto args...);

public:
  using Ptr = std::shared_ptr<T>;
   
  static   Ptr Instance  ;
  [[nodiscard]] static Ptr GetInstance(){
    return Instance;
  }
  static Ptr SetInstance(auto &&...args) {

    static Ptr instance = std::make_shared<T>(args...);

    // static auto arg = std::forward(args...);
    Instance = std::move(instance);
    return Instance;
  }
  //   static Ptr Instance() { return SetInstance(nullptr); }
};
 
 