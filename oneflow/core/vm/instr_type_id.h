#ifndef ONEFLOW_CORE_VM_INSTRUCTION_ID_H_
#define ONEFLOW_CORE_VM_INSTRUCTION_ID_H_

#include <typeindex>
#include "oneflow/core/common/flat_msg.h"
#include "oneflow/core/common/layout_standardize.h"
#include "oneflow/core/vm/stream_desc.msg.h"
#include "oneflow/core/vm/vm_type.h"

namespace oneflow {
namespace vm {

class InstructionType;

class InstrTypeId final {
 public:
  InstrTypeId() : instruction_type_(nullptr) { __Init__(); }
  InstrTypeId(const InstrTypeId& rhs) : instruction_type_(nullptr) {
    __Init__();
    CopyFrom(rhs);
  }

  ~InstrTypeId() = default;

  void __Init__() {
    std::memset(this, 0, sizeof(InstrTypeId));
    mutable_stream_type_id()->__Init__();
  }
  void __Init__(const StreamType* stream_type, const InstructionType* instruction_type,
                InterpretType interpret_type, VmType type) {
    __Init__();
    mutable_stream_type_id()->__Init__(stream_type, interpret_type);
    instruction_type_ = instruction_type;
    set_type(type);
  }
  void clear() {
    stream_type_id_.clear();
    instruction_type_ = nullptr;
    type_ = VmType::kInvalidVmType;
  }
  void CopyFrom(const InstrTypeId& rhs) {
    stream_type_id_.CopyFrom(rhs.stream_type_id_);
    instruction_type_ = &rhs.instruction_type();
    type_ = rhs.type_;
  }
  // Getters
  const StreamTypeId& stream_type_id() const { return stream_type_id_; }
  const InstructionType& instruction_type() const { return *instruction_type_; }
  VmType type() const { return type_; }

  // Setters
  StreamTypeId* mut_stream_type_id() { return &stream_type_id_; }
  StreamTypeId* mutable_stream_type_id() { return &stream_type_id_; }
  void set_type(VmType val) { type_ = val; }

  bool operator==(const InstrTypeId& rhs) const {
    return stream_type_id_ == rhs.stream_type_id_ && instruction_type_ == rhs.instruction_type_
           && type_ == rhs.type_;
  }
  bool operator<(const InstrTypeId& rhs) const {
    if (!(stream_type_id_ == rhs.stream_type_id_)) { return stream_type_id_ < rhs.stream_type_id_; }
    if (!(instruction_type_ == rhs.instruction_type_)) {
      return instruction_type_ < rhs.instruction_type_;
    }
    return type_ < rhs.type_;
  }
  bool operator<=(const InstrTypeId& rhs) const { return *this < rhs || *this == rhs; }

 private:
  const InstructionType* instruction_type_;
  StreamTypeId stream_type_id_;
  VmType type_;
};

}  // namespace vm
}  // namespace oneflow

#endif  // ONEFLOW_CORE_VM_INSTRUCTION_ID_H_
