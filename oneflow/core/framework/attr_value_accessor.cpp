#include "oneflow/core/framework/attr_value_accessor.h"
#include "oneflow/core/common/util.h"
#include "oneflow/core/common/shape.h"
#include "oneflow/core/common/protobuf.h"

namespace oneflow {

namespace user_op {

// Basic and Enum Attr
#define BASIC_AND_ENUM_ATTR_SEQ_ENTRY(field, cpp_type, attr_type)                             \
  template<>                                                                                  \
  cpp_type AttrValAccessor<cpp_type>::GetAttr(const UserOpAttrVal& val) {                     \
    return val.field();                                                                       \
  }                                                                                           \
  template<>                                                                                  \
  void AttrValAccessor<cpp_type>::SetAttr(const cpp_type& cpp_val, UserOpAttrVal* attr_val) { \
    attr_val->set_##field(cpp_val);                                                           \
  }

#define BASIC_AND_ENUM_ATTR_SEQ \
  BASIC_ATTR_SEQ                \
  ENUM_ATTR_SEQ

OF_PP_FOR_EACH_TUPLE(BASIC_AND_ENUM_ATTR_SEQ_ENTRY, BASIC_AND_ENUM_ATTR_SEQ)

#undef BASIC_AND_ENUM_ATTR_SEQ
#undef BASIC_AND_ENUM_ATTR_SEQ_ENTRY

// Customized Message Attr
template<>
Shape AttrValAccessor<Shape>::GetAttr(const UserOpAttrVal& val) {
  return Shape(val.at_shape());
}
template<>
void AttrValAccessor<Shape>::SetAttr(const Shape& cpp_val, UserOpAttrVal* attr_val) {
  cpp_val.ToProto(attr_val->mutable_at_shape());
}

// List of Basic Attr
#define LIST_BASIC_ATTR_SEQ_ENTRY(field, cpp_type, attr_type)                                   \
  template<>                                                                                    \
  cpp_type AttrValAccessor<cpp_type>::GetAttr(const UserOpAttrVal& val) {                       \
    return PbRf2StdVec<cpp_type::value_type>(val.field().val());                                \
  }                                                                                             \
  template<>                                                                                    \
  void AttrValAccessor<cpp_type>::SetAttr(const cpp_type& cpp_val, UserOpAttrVal* attr_val) {   \
    *(attr_val->mutable_##field()->mutable_val()) = StdVec2PbRf<cpp_type::value_type>(cpp_val); \
  }

OF_PP_FOR_EACH_TUPLE(LIST_BASIC_ATTR_SEQ_ENTRY, LIST_BASIC_ATTR_SEQ)

#undef LIST_BASIC_ATTR_SEQ_ENTRY

// List of Enum Attr
#define LIST_ENUM_ATTR_SEQ_ENTRY(field, cpp_type, attr_type)                                   \
  template<>                                                                                   \
  cpp_type AttrValAccessor<cpp_type>::GetAttr(const UserOpAttrVal& val) {                      \
    std::vector<cpp_type::value_type> ret;                                                     \
    ret.reserve(val.field().val_size());                                                       \
    for (const auto& value : val.field().val()) {                                              \
      ret.emplace_back(static_cast<cpp_type::value_type>(value));                              \
    }                                                                                          \
    return ret;                                                                                \
  }                                                                                            \
  template<>                                                                                   \
  void AttrValAccessor<cpp_type>::SetAttr(const cpp_type& cpp_val, UserOpAttrVal* attr_val) {  \
    using proto_type = std::remove_reference_t<decltype(attr_val->field().val())>::value_type; \
    std::vector<proto_type> vec;                                                               \
    vec.reserve(cpp_val.size());                                                               \
    for (const auto& value : cpp_val) { vec.emplace_back(static_cast<proto_type>(value)); }    \
    *(attr_val->mutable_##field()->mutable_val()) = StdVec2PbRf<proto_type>(vec);              \
  }

OF_PP_FOR_EACH_TUPLE(LIST_ENUM_ATTR_SEQ_ENTRY, LIST_ENUM_ATTR_SEQ)

#undef LIST_ENUM_ATTR_SEQ_ENTRY

// List of Customized Message Attr
template<>
std::vector<Shape> AttrValAccessor<std::vector<Shape>>::GetAttr(const UserOpAttrVal& val) {
  std::vector<Shape> ret;
  ret.reserve(val.at_list_shape().val_size());
  for (const auto& value : val.at_list_shape().val()) { ret.emplace_back(value); }
  return ret;
}
template<>
void AttrValAccessor<std::vector<Shape>>::SetAttr(const std::vector<Shape>& cpp_val,
                                                  UserOpAttrVal* attr_val) {
  if (attr_val->at_list_shape().val_size() > 0) { attr_val->mutable_at_list_shape()->clear_val(); }
  FOR_RANGE(int32_t, i, 0, cpp_val.size()) {
    cpp_val.at(i).ToProto(attr_val->mutable_at_list_shape()->add_val());
  }
}

}  // namespace user_op

}  // namespace oneflow
