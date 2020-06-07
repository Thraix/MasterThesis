#pragma once

#include <common/Types.h>
#include <utils/Utils.h>

#define GREET_ERROR_IMAGE_FORMAT	BIT(0)
#define GREET_ERROR_IMAGE_READ		BIT(1)
#define GREET_ERROR_IMAGE_BPP		BIT(2)
#define GREET_ERROR_IMAGE_CROP		BIT(3)
#define GREET_ERROR_GOBJ_FORMAT		BIT(4)
#define GREET_ERROR_GOBJ_READ		BIT(5)
#define GREET_ERROR_SHADER_VERTEX	BIT(6)
#define GREET_ERROR_SHADER_FRAGMENT	BIT(7)
#define GREET_ERROR_SHADER_GEOMETRY	BIT(8)
#define GREET_ERROR_MESH_LOCATION	BIT(8)
#define GREET_ERROR_MANAGER_ADD		BIT(9)
#define GREET_ERROR_MANAGER_GET		BIT(10)
#define GREET_ERROR_MANAGER_REMOVE	BIT(11)

namespace Greet {
  struct ErrorHandle
  {
    private:
      static ulong errorCode;
    public:
      static void SetErrorCode(ulong errorCode) { ErrorHandle::errorCode |= errorCode; };
      static ulong GetErrorCode() { return errorCode; }
      static void ResetErrorCode() { ErrorHandle::errorCode = 0; }
  };
}
