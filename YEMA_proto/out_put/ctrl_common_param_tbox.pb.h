// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ctrl_common_param_tbox.proto

#ifndef PROTOBUF_ctrl_5fcommon_5fparam_5ftbox_2eproto__INCLUDED
#define PROTOBUF_ctrl_5fcommon_5fparam_5ftbox_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2007000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2007000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_ctrl_5fcommon_5fparam_5ftbox_2eproto();
void protobuf_AssignDesc_ctrl_5fcommon_5fparam_5ftbox_2eproto();
void protobuf_ShutdownFile_ctrl_5fcommon_5fparam_5ftbox_2eproto();

class CtrlCommonInfo;
class CtrlCommonParamTbox;

// ===================================================================

class CtrlCommonParamTbox : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CtrlCommonParamTbox) */ {
 public:
  CtrlCommonParamTbox();
  virtual ~CtrlCommonParamTbox();

  CtrlCommonParamTbox(const CtrlCommonParamTbox& from);

  inline CtrlCommonParamTbox& operator=(const CtrlCommonParamTbox& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CtrlCommonParamTbox& default_instance();

  void Swap(CtrlCommonParamTbox* other);

  // implements Message ----------------------------------------------

  inline CtrlCommonParamTbox* New() const { return New(NULL); }

  CtrlCommonParamTbox* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CtrlCommonParamTbox& from);
  void MergeFrom(const CtrlCommonParamTbox& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CtrlCommonParamTbox* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional fixed64 request_id = 1;
  bool has_request_id() const;
  void clear_request_id();
  static const int kRequestIdFieldNumber = 1;
  ::google::protobuf::uint64 request_id() const;
  void set_request_id(::google::protobuf::uint64 value);

  // optional int32 delay_time = 2;
  bool has_delay_time() const;
  void clear_delay_time();
  static const int kDelayTimeFieldNumber = 2;
  ::google::protobuf::int32 delay_time() const;
  void set_delay_time(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:CtrlCommonParamTbox)
 private:
  inline void set_has_request_id();
  inline void clear_has_request_id();
  inline void set_has_delay_time();
  inline void clear_has_delay_time();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint64 request_id_;
  ::google::protobuf::int32 delay_time_;
  friend void  protobuf_AddDesc_ctrl_5fcommon_5fparam_5ftbox_2eproto();
  friend void protobuf_AssignDesc_ctrl_5fcommon_5fparam_5ftbox_2eproto();
  friend void protobuf_ShutdownFile_ctrl_5fcommon_5fparam_5ftbox_2eproto();

  void InitAsDefaultInstance();
  static CtrlCommonParamTbox* default_instance_;
};
// -------------------------------------------------------------------

class CtrlCommonInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CtrlCommonInfo) */ {
 public:
  CtrlCommonInfo();
  virtual ~CtrlCommonInfo();

  CtrlCommonInfo(const CtrlCommonInfo& from);

  inline CtrlCommonInfo& operator=(const CtrlCommonInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CtrlCommonInfo& default_instance();

  void Swap(CtrlCommonInfo* other);

  // implements Message ----------------------------------------------

  inline CtrlCommonInfo* New() const { return New(NULL); }

  CtrlCommonInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CtrlCommonInfo& from);
  void MergeFrom(const CtrlCommonInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CtrlCommonInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional fixed64 valid = 1;
  bool has_valid() const;
  void clear_valid();
  static const int kValidFieldNumber = 1;
  ::google::protobuf::uint64 valid() const;
  void set_valid(::google::protobuf::uint64 value);

  // optional fixed64 result = 2;
  bool has_result() const;
  void clear_result();
  static const int kResultFieldNumber = 2;
  ::google::protobuf::uint64 result() const;
  void set_result(::google::protobuf::uint64 value);

  // optional fixed64 state = 4;
  bool has_state() const;
  void clear_state();
  static const int kStateFieldNumber = 4;
  ::google::protobuf::uint64 state() const;
  void set_state(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:CtrlCommonInfo)
 private:
  inline void set_has_valid();
  inline void clear_has_valid();
  inline void set_has_result();
  inline void clear_has_result();
  inline void set_has_state();
  inline void clear_has_state();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint64 valid_;
  ::google::protobuf::uint64 result_;
  ::google::protobuf::uint64 state_;
  friend void  protobuf_AddDesc_ctrl_5fcommon_5fparam_5ftbox_2eproto();
  friend void protobuf_AssignDesc_ctrl_5fcommon_5fparam_5ftbox_2eproto();
  friend void protobuf_ShutdownFile_ctrl_5fcommon_5fparam_5ftbox_2eproto();

  void InitAsDefaultInstance();
  static CtrlCommonInfo* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CtrlCommonParamTbox

// optional fixed64 request_id = 1;
inline bool CtrlCommonParamTbox::has_request_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CtrlCommonParamTbox::set_has_request_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CtrlCommonParamTbox::clear_has_request_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CtrlCommonParamTbox::clear_request_id() {
  request_id_ = GOOGLE_ULONGLONG(0);
  clear_has_request_id();
}
inline ::google::protobuf::uint64 CtrlCommonParamTbox::request_id() const {
  // @@protoc_insertion_point(field_get:CtrlCommonParamTbox.request_id)
  return request_id_;
}
inline void CtrlCommonParamTbox::set_request_id(::google::protobuf::uint64 value) {
  set_has_request_id();
  request_id_ = value;
  // @@protoc_insertion_point(field_set:CtrlCommonParamTbox.request_id)
}

// optional int32 delay_time = 2;
inline bool CtrlCommonParamTbox::has_delay_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CtrlCommonParamTbox::set_has_delay_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CtrlCommonParamTbox::clear_has_delay_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CtrlCommonParamTbox::clear_delay_time() {
  delay_time_ = 0;
  clear_has_delay_time();
}
inline ::google::protobuf::int32 CtrlCommonParamTbox::delay_time() const {
  // @@protoc_insertion_point(field_get:CtrlCommonParamTbox.delay_time)
  return delay_time_;
}
inline void CtrlCommonParamTbox::set_delay_time(::google::protobuf::int32 value) {
  set_has_delay_time();
  delay_time_ = value;
  // @@protoc_insertion_point(field_set:CtrlCommonParamTbox.delay_time)
}

// -------------------------------------------------------------------

// CtrlCommonInfo

// optional fixed64 valid = 1;
inline bool CtrlCommonInfo::has_valid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CtrlCommonInfo::set_has_valid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CtrlCommonInfo::clear_has_valid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CtrlCommonInfo::clear_valid() {
  valid_ = GOOGLE_ULONGLONG(0);
  clear_has_valid();
}
inline ::google::protobuf::uint64 CtrlCommonInfo::valid() const {
  // @@protoc_insertion_point(field_get:CtrlCommonInfo.valid)
  return valid_;
}
inline void CtrlCommonInfo::set_valid(::google::protobuf::uint64 value) {
  set_has_valid();
  valid_ = value;
  // @@protoc_insertion_point(field_set:CtrlCommonInfo.valid)
}

// optional fixed64 result = 2;
inline bool CtrlCommonInfo::has_result() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CtrlCommonInfo::set_has_result() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CtrlCommonInfo::clear_has_result() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CtrlCommonInfo::clear_result() {
  result_ = GOOGLE_ULONGLONG(0);
  clear_has_result();
}
inline ::google::protobuf::uint64 CtrlCommonInfo::result() const {
  // @@protoc_insertion_point(field_get:CtrlCommonInfo.result)
  return result_;
}
inline void CtrlCommonInfo::set_result(::google::protobuf::uint64 value) {
  set_has_result();
  result_ = value;
  // @@protoc_insertion_point(field_set:CtrlCommonInfo.result)
}

// optional fixed64 state = 4;
inline bool CtrlCommonInfo::has_state() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void CtrlCommonInfo::set_has_state() {
  _has_bits_[0] |= 0x00000004u;
}
inline void CtrlCommonInfo::clear_has_state() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void CtrlCommonInfo::clear_state() {
  state_ = GOOGLE_ULONGLONG(0);
  clear_has_state();
}
inline ::google::protobuf::uint64 CtrlCommonInfo::state() const {
  // @@protoc_insertion_point(field_get:CtrlCommonInfo.state)
  return state_;
}
inline void CtrlCommonInfo::set_state(::google::protobuf::uint64 value) {
  set_has_state();
  state_ = value;
  // @@protoc_insertion_point(field_set:CtrlCommonInfo.state)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ctrl_5fcommon_5fparam_5ftbox_2eproto__INCLUDED