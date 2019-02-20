// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ctrl_charge.proto

#ifndef PROTOBUF_ctrl_5fcharge_2eproto__INCLUDED
#define PROTOBUF_ctrl_5fcharge_2eproto__INCLUDED

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
#include "ctrl_common_param_tbox.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_ctrl_5fcharge_2eproto();
void protobuf_AssignDesc_ctrl_5fcharge_2eproto();
void protobuf_ShutdownFile_ctrl_5fcharge_2eproto();

class ChargeStateInfo;
class CtrlChargeMessage;

// ===================================================================

class CtrlChargeMessage : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CtrlChargeMessage) */ {
 public:
  CtrlChargeMessage();
  virtual ~CtrlChargeMessage();

  CtrlChargeMessage(const CtrlChargeMessage& from);

  inline CtrlChargeMessage& operator=(const CtrlChargeMessage& from) {
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
  static const CtrlChargeMessage& default_instance();

  void Swap(CtrlChargeMessage* other);

  // implements Message ----------------------------------------------

  inline CtrlChargeMessage* New() const { return New(NULL); }

  CtrlChargeMessage* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CtrlChargeMessage& from);
  void MergeFrom(const CtrlChargeMessage& from);
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
  void InternalSwap(CtrlChargeMessage* other);
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

  // optional .CtrlCommonParamTbox common_param = 1;
  bool has_common_param() const;
  void clear_common_param();
  static const int kCommonParamFieldNumber = 1;
  const ::CtrlCommonParamTbox& common_param() const;
  ::CtrlCommonParamTbox* mutable_common_param();
  ::CtrlCommonParamTbox* release_common_param();
  void set_allocated_common_param(::CtrlCommonParamTbox* common_param);

  // optional .CtrlCommonInfo charge_ctrl_info = 2;
  bool has_charge_ctrl_info() const;
  void clear_charge_ctrl_info();
  static const int kChargeCtrlInfoFieldNumber = 2;
  const ::CtrlCommonInfo& charge_ctrl_info() const;
  ::CtrlCommonInfo* mutable_charge_ctrl_info();
  ::CtrlCommonInfo* release_charge_ctrl_info();
  void set_allocated_charge_ctrl_info(::CtrlCommonInfo* charge_ctrl_info);

  // optional .ChargeStateInfo charge_state_info = 3;
  bool has_charge_state_info() const;
  void clear_charge_state_info();
  static const int kChargeStateInfoFieldNumber = 3;
  const ::ChargeStateInfo& charge_state_info() const;
  ::ChargeStateInfo* mutable_charge_state_info();
  ::ChargeStateInfo* release_charge_state_info();
  void set_allocated_charge_state_info(::ChargeStateInfo* charge_state_info);

  // @@protoc_insertion_point(class_scope:CtrlChargeMessage)
 private:
  inline void set_has_common_param();
  inline void clear_has_common_param();
  inline void set_has_charge_ctrl_info();
  inline void clear_has_charge_ctrl_info();
  inline void set_has_charge_state_info();
  inline void clear_has_charge_state_info();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::CtrlCommonParamTbox* common_param_;
  ::CtrlCommonInfo* charge_ctrl_info_;
  ::ChargeStateInfo* charge_state_info_;
  friend void  protobuf_AddDesc_ctrl_5fcharge_2eproto();
  friend void protobuf_AssignDesc_ctrl_5fcharge_2eproto();
  friend void protobuf_ShutdownFile_ctrl_5fcharge_2eproto();

  void InitAsDefaultInstance();
  static CtrlChargeMessage* default_instance_;
};
// -------------------------------------------------------------------

class ChargeStateInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ChargeStateInfo) */ {
 public:
  ChargeStateInfo();
  virtual ~ChargeStateInfo();

  ChargeStateInfo(const ChargeStateInfo& from);

  inline ChargeStateInfo& operator=(const ChargeStateInfo& from) {
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
  static const ChargeStateInfo& default_instance();

  void Swap(ChargeStateInfo* other);

  // implements Message ----------------------------------------------

  inline ChargeStateInfo* New() const { return New(NULL); }

  ChargeStateInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ChargeStateInfo& from);
  void MergeFrom(const ChargeStateInfo& from);
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
  void InternalSwap(ChargeStateInfo* other);
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

  // optional int32 charge_on_off = 1;
  bool has_charge_on_off() const;
  void clear_charge_on_off();
  static const int kChargeOnOffFieldNumber = 1;
  ::google::protobuf::int32 charge_on_off() const;
  void set_charge_on_off(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:ChargeStateInfo)
 private:
  inline void set_has_charge_on_off();
  inline void clear_has_charge_on_off();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 charge_on_off_;
  friend void  protobuf_AddDesc_ctrl_5fcharge_2eproto();
  friend void protobuf_AssignDesc_ctrl_5fcharge_2eproto();
  friend void protobuf_ShutdownFile_ctrl_5fcharge_2eproto();

  void InitAsDefaultInstance();
  static ChargeStateInfo* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CtrlChargeMessage

// optional .CtrlCommonParamTbox common_param = 1;
inline bool CtrlChargeMessage::has_common_param() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CtrlChargeMessage::set_has_common_param() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CtrlChargeMessage::clear_has_common_param() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CtrlChargeMessage::clear_common_param() {
  if (common_param_ != NULL) common_param_->::CtrlCommonParamTbox::Clear();
  clear_has_common_param();
}
inline const ::CtrlCommonParamTbox& CtrlChargeMessage::common_param() const {
  // @@protoc_insertion_point(field_get:CtrlChargeMessage.common_param)
  return common_param_ != NULL ? *common_param_ : *default_instance_->common_param_;
}
inline ::CtrlCommonParamTbox* CtrlChargeMessage::mutable_common_param() {
  set_has_common_param();
  if (common_param_ == NULL) {
    common_param_ = new ::CtrlCommonParamTbox;
  }
  // @@protoc_insertion_point(field_mutable:CtrlChargeMessage.common_param)
  return common_param_;
}
inline ::CtrlCommonParamTbox* CtrlChargeMessage::release_common_param() {
  // @@protoc_insertion_point(field_release:CtrlChargeMessage.common_param)
  clear_has_common_param();
  ::CtrlCommonParamTbox* temp = common_param_;
  common_param_ = NULL;
  return temp;
}
inline void CtrlChargeMessage::set_allocated_common_param(::CtrlCommonParamTbox* common_param) {
  delete common_param_;
  common_param_ = common_param;
  if (common_param) {
    set_has_common_param();
  } else {
    clear_has_common_param();
  }
  // @@protoc_insertion_point(field_set_allocated:CtrlChargeMessage.common_param)
}

// optional .CtrlCommonInfo charge_ctrl_info = 2;
inline bool CtrlChargeMessage::has_charge_ctrl_info() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CtrlChargeMessage::set_has_charge_ctrl_info() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CtrlChargeMessage::clear_has_charge_ctrl_info() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CtrlChargeMessage::clear_charge_ctrl_info() {
  if (charge_ctrl_info_ != NULL) charge_ctrl_info_->::CtrlCommonInfo::Clear();
  clear_has_charge_ctrl_info();
}
inline const ::CtrlCommonInfo& CtrlChargeMessage::charge_ctrl_info() const {
  // @@protoc_insertion_point(field_get:CtrlChargeMessage.charge_ctrl_info)
  return charge_ctrl_info_ != NULL ? *charge_ctrl_info_ : *default_instance_->charge_ctrl_info_;
}
inline ::CtrlCommonInfo* CtrlChargeMessage::mutable_charge_ctrl_info() {
  set_has_charge_ctrl_info();
  if (charge_ctrl_info_ == NULL) {
    charge_ctrl_info_ = new ::CtrlCommonInfo;
  }
  // @@protoc_insertion_point(field_mutable:CtrlChargeMessage.charge_ctrl_info)
  return charge_ctrl_info_;
}
inline ::CtrlCommonInfo* CtrlChargeMessage::release_charge_ctrl_info() {
  // @@protoc_insertion_point(field_release:CtrlChargeMessage.charge_ctrl_info)
  clear_has_charge_ctrl_info();
  ::CtrlCommonInfo* temp = charge_ctrl_info_;
  charge_ctrl_info_ = NULL;
  return temp;
}
inline void CtrlChargeMessage::set_allocated_charge_ctrl_info(::CtrlCommonInfo* charge_ctrl_info) {
  delete charge_ctrl_info_;
  charge_ctrl_info_ = charge_ctrl_info;
  if (charge_ctrl_info) {
    set_has_charge_ctrl_info();
  } else {
    clear_has_charge_ctrl_info();
  }
  // @@protoc_insertion_point(field_set_allocated:CtrlChargeMessage.charge_ctrl_info)
}

// optional .ChargeStateInfo charge_state_info = 3;
inline bool CtrlChargeMessage::has_charge_state_info() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void CtrlChargeMessage::set_has_charge_state_info() {
  _has_bits_[0] |= 0x00000004u;
}
inline void CtrlChargeMessage::clear_has_charge_state_info() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void CtrlChargeMessage::clear_charge_state_info() {
  if (charge_state_info_ != NULL) charge_state_info_->::ChargeStateInfo::Clear();
  clear_has_charge_state_info();
}
inline const ::ChargeStateInfo& CtrlChargeMessage::charge_state_info() const {
  // @@protoc_insertion_point(field_get:CtrlChargeMessage.charge_state_info)
  return charge_state_info_ != NULL ? *charge_state_info_ : *default_instance_->charge_state_info_;
}
inline ::ChargeStateInfo* CtrlChargeMessage::mutable_charge_state_info() {
  set_has_charge_state_info();
  if (charge_state_info_ == NULL) {
    charge_state_info_ = new ::ChargeStateInfo;
  }
  // @@protoc_insertion_point(field_mutable:CtrlChargeMessage.charge_state_info)
  return charge_state_info_;
}
inline ::ChargeStateInfo* CtrlChargeMessage::release_charge_state_info() {
  // @@protoc_insertion_point(field_release:CtrlChargeMessage.charge_state_info)
  clear_has_charge_state_info();
  ::ChargeStateInfo* temp = charge_state_info_;
  charge_state_info_ = NULL;
  return temp;
}
inline void CtrlChargeMessage::set_allocated_charge_state_info(::ChargeStateInfo* charge_state_info) {
  delete charge_state_info_;
  charge_state_info_ = charge_state_info;
  if (charge_state_info) {
    set_has_charge_state_info();
  } else {
    clear_has_charge_state_info();
  }
  // @@protoc_insertion_point(field_set_allocated:CtrlChargeMessage.charge_state_info)
}

// -------------------------------------------------------------------

// ChargeStateInfo

// optional int32 charge_on_off = 1;
inline bool ChargeStateInfo::has_charge_on_off() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ChargeStateInfo::set_has_charge_on_off() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ChargeStateInfo::clear_has_charge_on_off() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ChargeStateInfo::clear_charge_on_off() {
  charge_on_off_ = 0;
  clear_has_charge_on_off();
}
inline ::google::protobuf::int32 ChargeStateInfo::charge_on_off() const {
  // @@protoc_insertion_point(field_get:ChargeStateInfo.charge_on_off)
  return charge_on_off_;
}
inline void ChargeStateInfo::set_charge_on_off(::google::protobuf::int32 value) {
  set_has_charge_on_off();
  charge_on_off_ = value;
  // @@protoc_insertion_point(field_set:ChargeStateInfo.charge_on_off)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ctrl_5fcharge_2eproto__INCLUDED
