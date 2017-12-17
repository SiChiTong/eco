// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Error.proto

#ifndef PROTOBUF_Error_2eproto__INCLUDED
#define PROTOBUF_Error_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace eco {
namespace service {
namespace proto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Error_2eproto();
void protobuf_AssignDesc_Error_2eproto();
void protobuf_ShutdownFile_Error_2eproto();

class Error;

// ===================================================================

class Error : public ::google::protobuf::Message {
 public:
  Error();
  virtual ~Error();

  Error(const Error& from);

  inline Error& operator=(const Error& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Error& default_instance();

  void Swap(Error* other);

  // implements Message ----------------------------------------------

  Error* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Error& from);
  void MergeFrom(const Error& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional bytes info = 2;
  inline bool has_info() const;
  inline void clear_info();
  static const int kInfoFieldNumber = 2;
  inline const ::std::string& info() const;
  inline void set_info(const ::std::string& value);
  inline void set_info(const char* value);
  inline void set_info(const void* value, size_t size);
  inline ::std::string* mutable_info();
  inline ::std::string* release_info();
  inline void set_allocated_info(::std::string* info);

  // @@protoc_insertion_point(class_scope:eco.service.proto.Error)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_info();
  inline void clear_has_info();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* info_;
  ::google::protobuf::int32 id_;
  friend void  protobuf_AddDesc_Error_2eproto();
  friend void protobuf_AssignDesc_Error_2eproto();
  friend void protobuf_ShutdownFile_Error_2eproto();

  void InitAsDefaultInstance();
  static Error* default_instance_;
};
// ===================================================================


// ===================================================================

// Error

// optional int32 id = 1;
inline bool Error::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Error::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Error::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Error::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Error::id() const {
  // @@protoc_insertion_point(field_get:eco.service.proto.Error.id)
  return id_;
}
inline void Error::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:eco.service.proto.Error.id)
}

// optional bytes info = 2;
inline bool Error::has_info() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Error::set_has_info() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Error::clear_has_info() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Error::clear_info() {
  if (info_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    info_->clear();
  }
  clear_has_info();
}
inline const ::std::string& Error::info() const {
  // @@protoc_insertion_point(field_get:eco.service.proto.Error.info)
  return *info_;
}
inline void Error::set_info(const ::std::string& value) {
  set_has_info();
  if (info_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    info_ = new ::std::string;
  }
  info_->assign(value);
  // @@protoc_insertion_point(field_set:eco.service.proto.Error.info)
}
inline void Error::set_info(const char* value) {
  set_has_info();
  if (info_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    info_ = new ::std::string;
  }
  info_->assign(value);
  // @@protoc_insertion_point(field_set_char:eco.service.proto.Error.info)
}
inline void Error::set_info(const void* value, size_t size) {
  set_has_info();
  if (info_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    info_ = new ::std::string;
  }
  info_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:eco.service.proto.Error.info)
}
inline ::std::string* Error::mutable_info() {
  set_has_info();
  if (info_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    info_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:eco.service.proto.Error.info)
  return info_;
}
inline ::std::string* Error::release_info() {
  clear_has_info();
  if (info_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = info_;
    info_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Error::set_allocated_info(::std::string* info) {
  if (info_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete info_;
  }
  if (info) {
    set_has_info();
    info_ = info;
  } else {
    clear_has_info();
    info_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:eco.service.proto.Error.info)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace service
}  // namespace eco

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Error_2eproto__INCLUDED