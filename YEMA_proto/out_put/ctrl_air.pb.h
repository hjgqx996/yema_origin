// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ctrl_air.proto

#ifndef PROTOBUF_ctrl_5fair_2eproto__INCLUDED
#define PROTOBUF_ctrl_5fair_2eproto__INCLUDED

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
void protobuf_AddDesc_ctrl_5fair_2eproto();
void protobuf_AssignDesc_ctrl_5fair_2eproto();
void protobuf_ShutdownFile_ctrl_5fair_2eproto();

class AirStateInfo;
class CtrlAirMessage;

// ===================================================================

class CtrlAirMessage : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CtrlAirMessage) */ {
 public:
  CtrlAirMessage();
  virtual ~CtrlAirMessage();

  CtrlAirMessage(const CtrlAirMessage& from);

  inline CtrlAirMessage& operator=(const CtrlAirMessage& from) {
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
  static const CtrlAirMessage& default_instance();

  void Swap(CtrlAirMessage* other);

  // implements Message ----------------------------------------------

  inline CtrlAirMessage* New() const { return New(NULL); }

  CtrlAirMessage* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CtrlAirMessage& from);
  void MergeFrom(const CtrlAirMessage& from);
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
  void InternalSwap(CtrlAirMessage* other);
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

  // optional .CtrlCommonInfo air_ctrl_info = 2;
  bool has_air_ctrl_info() const;
  void clear_air_ctrl_info();
  static const int kAirCtrlInfoFieldNumber = 2;
  const ::CtrlCommonInfo& air_ctrl_info() const;
  ::CtrlCommonInfo* mutable_air_ctrl_info();
  ::CtrlCommonInfo* release_air_ctrl_info();
  void set_allocated_air_ctrl_info(::CtrlCommonInfo* air_ctrl_info);

  // optional .AirStateInfo air_state_info = 4;
  bool has_air_state_info() const;
  void clear_air_state_info();
  static const int kAirStateInfoFieldNumber = 4;
  const ::AirStateInfo& air_state_info() const;
  ::AirStateInfo* mutable_air_state_info();
  ::AirStateInfo* release_air_state_info();
  void set_allocated_air_state_info(::AirStateInfo* air_state_info);

  // @@protoc_insertion_point(class_scope:CtrlAirMessage)
 private:
  inline void set_has_common_param();
  inline void clear_has_common_param();
  inline void set_has_air_ctrl_info();
  inline void clear_has_air_ctrl_info();
  inline void set_has_air_state_info();
  inline void clear_has_air_state_info();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::CtrlCommonParamTbox* common_param_;
  ::CtrlCommonInfo* air_ctrl_info_;
  ::AirStateInfo* air_state_info_;
  friend void  protobuf_AddDesc_ctrl_5fair_2eproto();
  friend void protobuf_AssignDesc_ctrl_5fair_2eproto();
  friend void protobuf_ShutdownFile_ctrl_5fair_2eproto();

  void InitAsDefaultInstance();
  static CtrlAirMessage* default_instance_;
};
// -------------------------------------------------------------------

class AirStateInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:AirStateInfo) */ {
 public:
  AirStateInfo();
  virtual ~AirStateInfo();

  AirStateInfo(const AirStateInfo& from);

  inline AirStateInfo& operator=(const AirStateInfo& from) {
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
  static const AirStateInfo& default_instance();

  void Swap(AirStateInfo* other);

  // implements Message ----------------------------------------------

  inline AirStateInfo* New() const { return New(NULL); }

  AirStateInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AirStateInfo& from);
  void MergeFrom(const AirStateInfo& from);
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
  void InternalSwap(AirStateInfo* other);
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

  // optional int32 ac = 1;
  bool has_ac() const;
  void clear_ac();
  static const int kAcFieldNumber = 1;
  ::google::protobuf::int32 ac() const;
  void set_ac(::google::protobuf::int32 value);

  // optional int32 ptc = 2;
  bool has_ptc() const;
  void clear_ptc();
  static const int kPtcFieldNumber = 2;
  ::google::protobuf::int32 ptc() const;
  void set_ptc(::google::protobuf::int32 value);

  // optional int32 wind_level = 3;
  bool has_wind_level() const;
  void clear_wind_level();
  static const int kWindLevelFieldNumber = 3;
  ::google::protobuf::int32 wind_level() const;
  void set_wind_level(::google::protobuf::int32 value);

  // optional int32 blower_mode = 4;
  bool has_blower_mode() const;
  void clear_blower_mode();
  static const int kBlowerModeFieldNumber = 4;
  ::google::protobuf::int32 blower_mode() const;
  void set_blower_mode(::google::protobuf::int32 value);

  // optional int32 cycle_mode = 5;
  bool has_cycle_mode() const;
  void clear_cycle_mode();
  static const int kCycleModeFieldNumber = 5;
  ::google::protobuf::int32 cycle_mode() const;
  void set_cycle_mode(::google::protobuf::int32 value);

  // optional int32 pre_defrosting = 6;
  bool has_pre_defrosting() const;
  void clear_pre_defrosting();
  static const int kPreDefrostingFieldNumber = 6;
  ::google::protobuf::int32 pre_defrosting() const;
  void set_pre_defrosting(::google::protobuf::int32 value);

  // optional int32 after_defrosting = 7;
  bool has_after_defrosting() const;
  void clear_after_defrosting();
  static const int kAfterDefrostingFieldNumber = 7;
  ::google::protobuf::int32 after_defrosting() const;
  void set_after_defrosting(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:AirStateInfo)
 private:
  inline void set_has_ac();
  inline void clear_has_ac();
  inline void set_has_ptc();
  inline void clear_has_ptc();
  inline void set_has_wind_level();
  inline void clear_has_wind_level();
  inline void set_has_blower_mode();
  inline void clear_has_blower_mode();
  inline void set_has_cycle_mode();
  inline void clear_has_cycle_mode();
  inline void set_has_pre_defrosting();
  inline void clear_has_pre_defrosting();
  inline void set_has_after_defrosting();
  inline void clear_has_after_defrosting();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 ac_;
  ::google::protobuf::int32 ptc_;
  ::google::protobuf::int32 wind_level_;
  ::google::protobuf::int32 blower_mode_;
  ::google::protobuf::int32 cycle_mode_;
  ::google::protobuf::int32 pre_defrosting_;
  ::google::protobuf::int32 after_defrosting_;
  friend void  protobuf_AddDesc_ctrl_5fair_2eproto();
  friend void protobuf_AssignDesc_ctrl_5fair_2eproto();
  friend void protobuf_ShutdownFile_ctrl_5fair_2eproto();

  void InitAsDefaultInstance();
  static AirStateInfo* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CtrlAirMessage

// optional .CtrlCommonParamTbox common_param = 1;
inline bool CtrlAirMessage::has_common_param() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CtrlAirMessage::set_has_common_param() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CtrlAirMessage::clear_has_common_param() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CtrlAirMessage::clear_common_param() {
  if (common_param_ != NULL) common_param_->::CtrlCommonParamTbox::Clear();
  clear_has_common_param();
}
inline const ::CtrlCommonParamTbox& CtrlAirMessage::common_param() const {
  // @@protoc_insertion_point(field_get:CtrlAirMessage.common_param)
  return common_param_ != NULL ? *common_param_ : *default_instance_->common_param_;
}
inline ::CtrlCommonParamTbox* CtrlAirMessage::mutable_common_param() {
  set_has_common_param();
  if (common_param_ == NULL) {
    common_param_ = new ::CtrlCommonParamTbox;
  }
  // @@protoc_insertion_point(field_mutable:CtrlAirMessage.common_param)
  return common_param_;
}
inline ::CtrlCommonParamTbox* CtrlAirMessage::release_common_param() {
  // @@protoc_insertion_point(field_release:CtrlAirMessage.common_param)
  clear_has_common_param();
  ::CtrlCommonParamTbox* temp = common_param_;
  common_param_ = NULL;
  return temp;
}
inline void CtrlAirMessage::set_allocated_common_param(::CtrlCommonParamTbox* common_param) {
  delete common_param_;
  common_param_ = common_param;
  if (common_param) {
    set_has_common_param();
  } else {
    clear_has_common_param();
  }
  // @@protoc_insertion_point(field_set_allocated:CtrlAirMessage.common_param)
}

// optional .CtrlCommonInfo air_ctrl_info = 2;
inline bool CtrlAirMessage::has_air_ctrl_info() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CtrlAirMessage::set_has_air_ctrl_info() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CtrlAirMessage::clear_has_air_ctrl_info() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CtrlAirMessage::clear_air_ctrl_info() {
  if (air_ctrl_info_ != NULL) air_ctrl_info_->::CtrlCommonInfo::Clear();
  clear_has_air_ctrl_info();
}
inline const ::CtrlCommonInfo& CtrlAirMessage::air_ctrl_info() const {
  // @@protoc_insertion_point(field_get:CtrlAirMessage.air_ctrl_info)
  return air_ctrl_info_ != NULL ? *air_ctrl_info_ : *default_instance_->air_ctrl_info_;
}
inline ::CtrlCommonInfo* CtrlAirMessage::mutable_air_ctrl_info() {
  set_has_air_ctrl_info();
  if (air_ctrl_info_ == NULL) {
    air_ctrl_info_ = new ::CtrlCommonInfo;
  }
  // @@protoc_insertion_point(field_mutable:CtrlAirMessage.air_ctrl_info)
  return air_ctrl_info_;
}
inline ::CtrlCommonInfo* CtrlAirMessage::release_air_ctrl_info() {
  // @@protoc_insertion_point(field_release:CtrlAirMessage.air_ctrl_info)
  clear_has_air_ctrl_info();
  ::CtrlCommonInfo* temp = air_ctrl_info_;
  air_ctrl_info_ = NULL;
  return temp;
}
inline void CtrlAirMessage::set_allocated_air_ctrl_info(::CtrlCommonInfo* air_ctrl_info) {
  delete air_ctrl_info_;
  air_ctrl_info_ = air_ctrl_info;
  if (air_ctrl_info) {
    set_has_air_ctrl_info();
  } else {
    clear_has_air_ctrl_info();
  }
  // @@protoc_insertion_point(field_set_allocated:CtrlAirMessage.air_ctrl_info)
}

// optional .AirStateInfo air_state_info = 4;
inline bool CtrlAirMessage::has_air_state_info() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void CtrlAirMessage::set_has_air_state_info() {
  _has_bits_[0] |= 0x00000004u;
}
inline void CtrlAirMessage::clear_has_air_state_info() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void CtrlAirMessage::clear_air_state_info() {
  if (air_state_info_ != NULL) air_state_info_->::AirStateInfo::Clear();
  clear_has_air_state_info();
}
inline const ::AirStateInfo& CtrlAirMessage::air_state_info() const {
  // @@protoc_insertion_point(field_get:CtrlAirMessage.air_state_info)
  return air_state_info_ != NULL ? *air_state_info_ : *default_instance_->air_state_info_;
}
inline ::AirStateInfo* CtrlAirMessage::mutable_air_state_info() {
  set_has_air_state_info();
  if (air_state_info_ == NULL) {
    air_state_info_ = new ::AirStateInfo;
  }
  // @@protoc_insertion_point(field_mutable:CtrlAirMessage.air_state_info)
  return air_state_info_;
}
inline ::AirStateInfo* CtrlAirMessage::release_air_state_info() {
  // @@protoc_insertion_point(field_release:CtrlAirMessage.air_state_info)
  clear_has_air_state_info();
  ::AirStateInfo* temp = air_state_info_;
  air_state_info_ = NULL;
  return temp;
}
inline void CtrlAirMessage::set_allocated_air_state_info(::AirStateInfo* air_state_info) {
  delete air_state_info_;
  air_state_info_ = air_state_info;
  if (air_state_info) {
    set_has_air_state_info();
  } else {
    clear_has_air_state_info();
  }
  // @@protoc_insertion_point(field_set_allocated:CtrlAirMessage.air_state_info)
}

// -------------------------------------------------------------------

// AirStateInfo

// optional int32 ac = 1;
inline bool AirStateInfo::has_ac() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AirStateInfo::set_has_ac() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AirStateInfo::clear_has_ac() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AirStateInfo::clear_ac() {
  ac_ = 0;
  clear_has_ac();
}
inline ::google::protobuf::int32 AirStateInfo::ac() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.ac)
  return ac_;
}
inline void AirStateInfo::set_ac(::google::protobuf::int32 value) {
  set_has_ac();
  ac_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.ac)
}

// optional int32 ptc = 2;
inline bool AirStateInfo::has_ptc() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AirStateInfo::set_has_ptc() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AirStateInfo::clear_has_ptc() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AirStateInfo::clear_ptc() {
  ptc_ = 0;
  clear_has_ptc();
}
inline ::google::protobuf::int32 AirStateInfo::ptc() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.ptc)
  return ptc_;
}
inline void AirStateInfo::set_ptc(::google::protobuf::int32 value) {
  set_has_ptc();
  ptc_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.ptc)
}

// optional int32 wind_level = 3;
inline bool AirStateInfo::has_wind_level() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void AirStateInfo::set_has_wind_level() {
  _has_bits_[0] |= 0x00000004u;
}
inline void AirStateInfo::clear_has_wind_level() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void AirStateInfo::clear_wind_level() {
  wind_level_ = 0;
  clear_has_wind_level();
}
inline ::google::protobuf::int32 AirStateInfo::wind_level() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.wind_level)
  return wind_level_;
}
inline void AirStateInfo::set_wind_level(::google::protobuf::int32 value) {
  set_has_wind_level();
  wind_level_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.wind_level)
}

// optional int32 blower_mode = 4;
inline bool AirStateInfo::has_blower_mode() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void AirStateInfo::set_has_blower_mode() {
  _has_bits_[0] |= 0x00000008u;
}
inline void AirStateInfo::clear_has_blower_mode() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void AirStateInfo::clear_blower_mode() {
  blower_mode_ = 0;
  clear_has_blower_mode();
}
inline ::google::protobuf::int32 AirStateInfo::blower_mode() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.blower_mode)
  return blower_mode_;
}
inline void AirStateInfo::set_blower_mode(::google::protobuf::int32 value) {
  set_has_blower_mode();
  blower_mode_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.blower_mode)
}

// optional int32 cycle_mode = 5;
inline bool AirStateInfo::has_cycle_mode() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void AirStateInfo::set_has_cycle_mode() {
  _has_bits_[0] |= 0x00000010u;
}
inline void AirStateInfo::clear_has_cycle_mode() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void AirStateInfo::clear_cycle_mode() {
  cycle_mode_ = 0;
  clear_has_cycle_mode();
}
inline ::google::protobuf::int32 AirStateInfo::cycle_mode() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.cycle_mode)
  return cycle_mode_;
}
inline void AirStateInfo::set_cycle_mode(::google::protobuf::int32 value) {
  set_has_cycle_mode();
  cycle_mode_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.cycle_mode)
}

// optional int32 pre_defrosting = 6;
inline bool AirStateInfo::has_pre_defrosting() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void AirStateInfo::set_has_pre_defrosting() {
  _has_bits_[0] |= 0x00000020u;
}
inline void AirStateInfo::clear_has_pre_defrosting() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void AirStateInfo::clear_pre_defrosting() {
  pre_defrosting_ = 0;
  clear_has_pre_defrosting();
}
inline ::google::protobuf::int32 AirStateInfo::pre_defrosting() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.pre_defrosting)
  return pre_defrosting_;
}
inline void AirStateInfo::set_pre_defrosting(::google::protobuf::int32 value) {
  set_has_pre_defrosting();
  pre_defrosting_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.pre_defrosting)
}

// optional int32 after_defrosting = 7;
inline bool AirStateInfo::has_after_defrosting() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void AirStateInfo::set_has_after_defrosting() {
  _has_bits_[0] |= 0x00000040u;
}
inline void AirStateInfo::clear_has_after_defrosting() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void AirStateInfo::clear_after_defrosting() {
  after_defrosting_ = 0;
  clear_has_after_defrosting();
}
inline ::google::protobuf::int32 AirStateInfo::after_defrosting() const {
  // @@protoc_insertion_point(field_get:AirStateInfo.after_defrosting)
  return after_defrosting_;
}
inline void AirStateInfo::set_after_defrosting(::google::protobuf::int32 value) {
  set_has_after_defrosting();
  after_defrosting_ = value;
  // @@protoc_insertion_point(field_set:AirStateInfo.after_defrosting)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ctrl_5fair_2eproto__INCLUDED
