#include <c10/core/DispatchKeySet.h>

namespace c10 {

// autograd_dispatch_keyset should include all runtime autograd keys.
// Alias key DispatchKey::Autograd maps to autograd_dispatch_keyset.
constexpr DispatchKeySet autograd_dispatch_keyset = DispatchKeySet({
  DispatchKey::AutogradCPU,
  DispatchKey::AutogradCUDA,
  DispatchKey::AutogradXLA,
  DispatchKey::AutogradPrivateUse1,
  DispatchKey::AutogradPrivateUse2,
  DispatchKey::AutogradPrivateUse3,
  DispatchKey::AutogradOther,
});

// backend_dispatch_keyset should include all runtime backend keys.
constexpr DispatchKeySet backend_dispatch_keyset = DispatchKeySet({
  DispatchKey::CPU,
  DispatchKey::CUDA,
  DispatchKey::HIP,
  DispatchKey::FPGA,
  DispatchKey::MSNPU,
  DispatchKey::XLA,
  DispatchKey::Vulkan,
  DispatchKey::MKLDNN,
  DispatchKey::OpenGL,
  DispatchKey::OpenCL,
  DispatchKey::IDEEP,
  DispatchKey::QuantizedCPU,
  DispatchKey::QuantizedCUDA,
  DispatchKey::ComplexCPU,
  DispatchKey::ComplexCUDA,
  DispatchKey::CustomRNGKeyId,
  DispatchKey::MkldnnCPU,
  DispatchKey::SparseCPU,
  DispatchKey::SparseCUDA,
  DispatchKey::SparseHIP,
  DispatchKey::PrivateUse1,
  DispatchKey::PrivateUse2,
  DispatchKey::PrivateUse3,
});

// math_dispatch_keyset contains all keys in backend_dispatch_keyset and autograd_dispatch_keyset
// Alias key DispatchKey::Math maps to math_dispatch_keyset.
constexpr DispatchKeySet math_dispatch_keyset = backend_dispatch_keyset | autograd_dispatch_keyset;

DispatchKeySet getRuntimeDispatchKeySet(DispatchKey t) {
  switch (t) {
    case DispatchKey::Autograd:
      return autograd_dispatch_keyset;
    case DispatchKey::Math:
      return math_dispatch_keyset;
   default:
     return DispatchKeySet(t);
  }
}

bool isIncludedInAlias(DispatchKey k, DispatchKey alias) {
  return k != DispatchKey::Undefined && getRuntimeDispatchKeySet(alias).has(k);
}

std::string toString(DispatchKeySet ts) {
  std::stringstream ss;
  ss << ts;
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, DispatchKeySet ts) {
  if (ts.empty()) {
    os << "DispatchKeySet()";
    return os;
  }
  os << "DispatchKeySet(";
  DispatchKey tid;
  bool first = true;
  while ((tid = ts.highestPriorityTypeId()) != DispatchKey::Undefined) {
    if (!first) {
      os << ", ";
    }
    os << tid;
    ts = ts.remove(tid);
    first = false;
  }
  os << ")";
  return os;
}

}
