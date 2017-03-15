#ifndef MACRO_UTILS
#define MACRO_UTILS

#include <stdexcept>
#include "Base/Log.h"

#define DEFINE_GETTER(FIELD_NAME, TYPE) \
  TYPE FIELD_NAME() const { return FIELD_NAME##_; } \

#define DEFINE_ACCESSOR(FIELD_NAME, TYPE) \
  TYPE FIELD_NAME() const { return FIELD_NAME##_; } \
  void set_##FIELD_NAME(TYPE FIELD_NAME) { FIELD_NAME##_ = FIELD_NAME; } \

#define DEFINE_INCREMENTOR_DECREMENTOR(FIELD_NAME, TYPE) \
  void increment_##FIELD_NAME(TYPE value) { FIELD_NAME##_ += value; } \
  void decrement_##FIELD_NAME(TYPE value) { FIELD_NAME##_ -= value; } \

#define DEFINE_ACCESSOR_ENUM(FIELD_NAME, TYPE) \
  TYPE FIELD_NAME() const { return FIELD_NAME##_; } \
  void set_##FIELD_NAME(enum TYPE FIELD_NAME) { FIELD_NAME##_ = FIELD_NAME; } \

#define DEFINE_ACCESSOR_RAW_PTR(FIELD_NAME, TYPE) \
  const TYPE* FIELD_NAME() const { return FIELD_NAME##_; } \
  TYPE* mutable_##FIELD_NAME() { return FIELD_NAME##_; } \
  void set_##FIELD_NAME(TYPE* FIELD_NAME) { \
    if (FIELD_NAME##_) { \
      delete FIELD_NAME##_; \
    } \
    FIELD_NAME##_ = FIELD_NAME; \
  } \

#define DEFINE_ACCESSOR_SMART_PTR(FIELD_NAME, TYPE) \
  const TYPE& FIELD_NAME() const { return *FIELD_NAME##_.get(); } \
  TYPE* mutable_##FIELD_NAME() const { return FIELD_NAME##_.get(); } \
  void set_##FIELD_NAME(TYPE* FIELD_NAME) { FIELD_NAME##_.reset(FIELD_NAME); } \

#define FORBID_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete; \
  TypeName& operator=(const TypeName&) = delete; \

#define CHECK(CONDITION, ERR_MSG, ...) \
  if (!(CONDITION)) { \
    LogFATAL(("SANITY_CHECK failed: " + (std::string)ERR_MSG).c_str(),  \
             ## __VA_ARGS__);  \
  } \

#define SANITY_CHECK(CONDITION, ERR_MSG, ...)  \
  CHECK(CONDITION, ERR_MSG, ## __VA_ARGS__)

#endif /* MACRO_UTILS */