// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: term_command_result.proto

#ifndef PROTOBUF_term_5fcommand_5fresult_2eproto__INCLUDED
#define PROTOBUF_term_5fcommand_5fresult_2eproto__INCLUDED

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
void protobuf_AddDesc_term_5fcommand_5fresult_2eproto();
void protobuf_AssignDesc_term_5fcommand_5fresult_2eproto();
void protobuf_ShutdownFile_term_5fcommand_5fresult_2eproto();

class TermCommandResult;

// ===================================================================

class TermCommandResult : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TermCommandResult) */ {
 public:
  TermCommandResult();
  virtual ~TermCommandResult();

  TermCommandResult(const TermCommandResult& from);

  inline TermCommandResult& operator=(const TermCommandResult& from) {
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
  static const TermCommandResult& default_instance();

  void Swap(TermCommandResult* other);

  // implements Message ----------------------------------------------

  inline TermCommandResult* New() const { return New(NULL); }

  TermCommandResult* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TermCommandResult& from);
  void MergeFrom(const TermCommandResult& from);
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
  void InternalSwap(TermCommandResult* other);
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

  // required uint32 termType = 1;
  bool has_termtype() const;
  void clear_termtype();
  static const int kTermTypeFieldNumber = 1;
  ::google::protobuf::uint32 termtype() const;
  void set_termtype(::google::protobuf::uint32 value);

  // required string requestId = 2;
  bool has_requestid() const;
  void clear_requestid();
  static const int kRequestIdFieldNumber = 2;
  const ::std::string& requestid() const;
  void set_requestid(const ::std::string& value);
  void set_requestid(const char* value);
  void set_requestid(const char* value, size_t size);
  ::std::string* mutable_requestid();
  ::std::string* release_requestid();
  void set_allocated_requestid(::std::string* requestid);

  // required uint32 status = 3;
  bool has_status() const;
  void clear_status();
  static const int kStatusFieldNumber = 3;
  ::google::protobuf::uint32 status() const;
  void set_status(::google::protobuf::uint32 value);

  // optional string message = 4;
  bool has_message() const;
  void clear_message();
  static const int kMessageFieldNumber = 4;
  const ::std::string& message() const;
  void set_message(const ::std::string& value);
  void set_message(const char* value);
  void set_message(const char* value, size_t size);
  ::std::string* mutable_message();
  ::std::string* release_message();
  void set_allocated_message(::std::string* message);

  // optional int32 respCode = 5;
  bool has_respcode() const;
  void clear_respcode();
  static const int kRespCodeFieldNumber = 5;
  ::google::protobuf::int32 respcode() const;
  void set_respcode(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:TermCommandResult)
 private:
  inline void set_has_termtype();
  inline void clear_has_termtype();
  inline void set_has_requestid();
  inline void clear_has_requestid();
  inline void set_has_status();
  inline void clear_has_status();
  inline void set_has_message();
  inline void clear_has_message();
  inline void set_has_respcode();
  inline void clear_has_respcode();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr requestid_;
  ::google::protobuf::uint32 termtype_;
  ::google::protobuf::uint32 status_;
  ::google::protobuf::internal::ArenaStringPtr message_;
  ::google::protobuf::int32 respcode_;
  friend void  protobuf_AddDesc_term_5fcommand_5fresult_2eproto();
  friend void protobuf_AssignDesc_term_5fcommand_5fresult_2eproto();
  friend void protobuf_ShutdownFile_term_5fcommand_5fresult_2eproto();

  void InitAsDefaultInstance();
  static TermCommandResult* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TermCommandResult

// required uint32 termType = 1;
inline bool TermCommandResult::has_termtype() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TermCommandResult::set_has_termtype() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TermCommandResult::clear_has_termtype() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TermCommandResult::clear_termtype() {
  termtype_ = 0u;
  clear_has_termtype();
}
inline ::google::protobuf::uint32 TermCommandResult::termtype() const {
  // @@protoc_insertion_point(field_get:TermCommandResult.termType)
  return termtype_;
}
inline void TermCommandResult::set_termtype(::google::protobuf::uint32 value) {
  set_has_termtype();
  termtype_ = value;
  // @@protoc_insertion_point(field_set:TermCommandResult.termType)
}

// required string requestId = 2;
inline bool TermCommandResult::has_requestid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TermCommandResult::set_has_requestid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TermCommandResult::clear_has_requestid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TermCommandResult::clear_requestid() {
  requestid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_requestid();
}
inline const ::std::string& TermCommandResult::requestid() const {
  // @@protoc_insertion_point(field_get:TermCommandResult.requestId)
  return requestid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TermCommandResult::set_requestid(const ::std::string& value) {
  set_has_requestid();
  requestid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TermCommandResult.requestId)
}
inline void TermCommandResult::set_requestid(const char* value) {
  set_has_requestid();
  requestid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TermCommandResult.requestId)
}
inline void TermCommandResult::set_requestid(const char* value, size_t size) {
  set_has_requestid();
  requestid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TermCommandResult.requestId)
}
inline ::std::string* TermCommandResult::mutable_requestid() {
  set_has_requestid();
  // @@protoc_insertion_point(field_mutable:TermCommandResult.requestId)
  return requestid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TermCommandResult::release_requestid() {
  // @@protoc_insertion_point(field_release:TermCommandResult.requestId)
  clear_has_requestid();
  return requestid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TermCommandResult::set_allocated_requestid(::std::string* requestid) {
  if (requestid != NULL) {
    set_has_requestid();
  } else {
    clear_has_requestid();
  }
  requestid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), requestid);
  // @@protoc_insertion_point(field_set_allocated:TermCommandResult.requestId)
}

// required uint32 status = 3;
inline bool TermCommandResult::has_status() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void TermCommandResult::set_has_status() {
  _has_bits_[0] |= 0x00000004u;
}
inline void TermCommandResult::clear_has_status() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void TermCommandResult::clear_status() {
  status_ = 0u;
  clear_has_status();
}
inline ::google::protobuf::uint32 TermCommandResult::status() const {
  // @@protoc_insertion_point(field_get:TermCommandResult.status)
  return status_;
}
inline void TermCommandResult::set_status(::google::protobuf::uint32 value) {
  set_has_status();
  status_ = value;
  // @@protoc_insertion_point(field_set:TermCommandResult.status)
}

// optional string message = 4;
inline bool TermCommandResult::has_message() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void TermCommandResult::set_has_message() {
  _has_bits_[0] |= 0x00000008u;
}
inline void TermCommandResult::clear_has_message() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void TermCommandResult::clear_message() {
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_message();
}
inline const ::std::string& TermCommandResult::message() const {
  // @@protoc_insertion_point(field_get:TermCommandResult.message)
  return message_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TermCommandResult::set_message(const ::std::string& value) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TermCommandResult.message)
}
inline void TermCommandResult::set_message(const char* value) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TermCommandResult.message)
}
inline void TermCommandResult::set_message(const char* value, size_t size) {
  set_has_message();
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TermCommandResult.message)
}
inline ::std::string* TermCommandResult::mutable_message() {
  set_has_message();
  // @@protoc_insertion_point(field_mutable:TermCommandResult.message)
  return message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TermCommandResult::release_message() {
  // @@protoc_insertion_point(field_release:TermCommandResult.message)
  clear_has_message();
  return message_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TermCommandResult::set_allocated_message(::std::string* message) {
  if (message != NULL) {
    set_has_message();
  } else {
    clear_has_message();
  }
  message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:TermCommandResult.message)
}

// optional int32 respCode = 5;
inline bool TermCommandResult::has_respcode() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void TermCommandResult::set_has_respcode() {
  _has_bits_[0] |= 0x00000010u;
}
inline void TermCommandResult::clear_has_respcode() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void TermCommandResult::clear_respcode() {
  respcode_ = 0;
  clear_has_respcode();
}
inline ::google::protobuf::int32 TermCommandResult::respcode() const {
  // @@protoc_insertion_point(field_get:TermCommandResult.respCode)
  return respcode_;
}
inline void TermCommandResult::set_respcode(::google::protobuf::int32 value) {
  set_has_respcode();
  respcode_ = value;
  // @@protoc_insertion_point(field_set:TermCommandResult.respCode)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_term_5fcommand_5fresult_2eproto__INCLUDED
