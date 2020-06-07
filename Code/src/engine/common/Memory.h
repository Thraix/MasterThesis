#pragma once

#include <memory>

namespace Greet
{
  template <class T>
  using Ref = std::shared_ptr<T>;
}
