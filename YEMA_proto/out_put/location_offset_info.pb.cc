// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: location_offset_info.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "location_offset_info.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* LocationOffsetInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LocationOffsetInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* LocationOffsetData_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LocationOffsetData_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_location_5foffset_5finfo_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_location_5foffset_5finfo_2eproto() {
  protobuf_AddDesc_location_5foffset_5finfo_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "location_offset_info.proto");
  GOOGLE_CHECK(file != NULL);
  LocationOffsetInfo_descriptor_ = file->message_type(0);
  static const int LocationOffsetInfo_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetInfo, locationoffsetdata_),
  };
  LocationOffsetInfo_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      LocationOffsetInfo_descriptor_,
      LocationOffsetInfo::default_instance_,
      LocationOffsetInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetInfo, _has_bits_[0]),
      -1,
      -1,
      sizeof(LocationOffsetInfo),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetInfo, _internal_metadata_),
      -1);
  LocationOffsetData_descriptor_ = file->message_type(1);
  static const int LocationOffsetData_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, offsetlon_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, offsetlat_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, offsetdir_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, offsetspeed_),
  };
  LocationOffsetData_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      LocationOffsetData_descriptor_,
      LocationOffsetData::default_instance_,
      LocationOffsetData_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, _has_bits_[0]),
      -1,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, _extensions_),
      sizeof(LocationOffsetData),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LocationOffsetData, _internal_metadata_),
      -1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_location_5foffset_5finfo_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      LocationOffsetInfo_descriptor_, &LocationOffsetInfo::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      LocationOffsetData_descriptor_, &LocationOffsetData::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_location_5foffset_5finfo_2eproto() {
  delete LocationOffsetInfo::default_instance_;
  delete LocationOffsetInfo_reflection_;
  delete LocationOffsetData::default_instance_;
  delete LocationOffsetData_reflection_;
}

void protobuf_AddDesc_location_5foffset_5finfo_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_location_5foffset_5finfo_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\032location_offset_info.proto\"E\n\022Location"
    "OffsetInfo\022/\n\022locationOffsetData\030\001 \003(\0132\023"
    ".LocationOffsetData\"i\n\022LocationOffsetDat"
    "a\022\021\n\toffsetLon\030\001 \002(\005\022\021\n\toffsetLat\030\002 \002(\005\022"
    "\021\n\toffsetDir\030\003 \002(\005\022\023\n\013offsetSpeed\030\004 \002(\005*"
    "\005\010 \020\200\001", 206);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "location_offset_info.proto", &protobuf_RegisterTypes);
  LocationOffsetInfo::default_instance_ = new LocationOffsetInfo();
  LocationOffsetData::default_instance_ = new LocationOffsetData();
  LocationOffsetInfo::default_instance_->InitAsDefaultInstance();
  LocationOffsetData::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_location_5foffset_5finfo_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_location_5foffset_5finfo_2eproto {
  StaticDescriptorInitializer_location_5foffset_5finfo_2eproto() {
    protobuf_AddDesc_location_5foffset_5finfo_2eproto();
  }
} static_descriptor_initializer_location_5foffset_5finfo_2eproto_;

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int LocationOffsetInfo::kLocationOffsetDataFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

LocationOffsetInfo::LocationOffsetInfo()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:LocationOffsetInfo)
}

void LocationOffsetInfo::InitAsDefaultInstance() {
}

LocationOffsetInfo::LocationOffsetInfo(const LocationOffsetInfo& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:LocationOffsetInfo)
}

void LocationOffsetInfo::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LocationOffsetInfo::~LocationOffsetInfo() {
  // @@protoc_insertion_point(destructor:LocationOffsetInfo)
  SharedDtor();
}

void LocationOffsetInfo::SharedDtor() {
  if (this != default_instance_) {
  }
}

void LocationOffsetInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LocationOffsetInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LocationOffsetInfo_descriptor_;
}

const LocationOffsetInfo& LocationOffsetInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_location_5foffset_5finfo_2eproto();
  return *default_instance_;
}

LocationOffsetInfo* LocationOffsetInfo::default_instance_ = NULL;

LocationOffsetInfo* LocationOffsetInfo::New(::google::protobuf::Arena* arena) const {
  LocationOffsetInfo* n = new LocationOffsetInfo;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void LocationOffsetInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:LocationOffsetInfo)
  locationoffsetdata_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool LocationOffsetInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:LocationOffsetInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .LocationOffsetData locationOffsetData = 1;
      case 1: {
        if (tag == 10) {
          DO_(input->IncrementRecursionDepth());
         parse_loop_locationOffsetData:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtualNoRecursionDepth(
                input, add_locationoffsetdata()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_loop_locationOffsetData;
        input->UnsafeDecrementRecursionDepth();
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:LocationOffsetInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:LocationOffsetInfo)
  return false;
#undef DO_
}

void LocationOffsetInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:LocationOffsetInfo)
  // repeated .LocationOffsetData locationOffsetData = 1;
  for (unsigned int i = 0, n = this->locationoffsetdata_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->locationoffsetdata(i), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:LocationOffsetInfo)
}

::google::protobuf::uint8* LocationOffsetInfo::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:LocationOffsetInfo)
  // repeated .LocationOffsetData locationOffsetData = 1;
  for (unsigned int i = 0, n = this->locationoffsetdata_size(); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        1, this->locationoffsetdata(i), false, target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:LocationOffsetInfo)
  return target;
}

int LocationOffsetInfo::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:LocationOffsetInfo)
  int total_size = 0;

  // repeated .LocationOffsetData locationOffsetData = 1;
  total_size += 1 * this->locationoffsetdata_size();
  for (int i = 0; i < this->locationoffsetdata_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->locationoffsetdata(i));
  }

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LocationOffsetInfo::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:LocationOffsetInfo)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const LocationOffsetInfo* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const LocationOffsetInfo>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:LocationOffsetInfo)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:LocationOffsetInfo)
    MergeFrom(*source);
  }
}

void LocationOffsetInfo::MergeFrom(const LocationOffsetInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:LocationOffsetInfo)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  locationoffsetdata_.MergeFrom(from.locationoffsetdata_);
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void LocationOffsetInfo::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:LocationOffsetInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LocationOffsetInfo::CopyFrom(const LocationOffsetInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:LocationOffsetInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LocationOffsetInfo::IsInitialized() const {

  if (!::google::protobuf::internal::AllAreInitialized(this->locationoffsetdata())) return false;
  return true;
}

void LocationOffsetInfo::Swap(LocationOffsetInfo* other) {
  if (other == this) return;
  InternalSwap(other);
}
void LocationOffsetInfo::InternalSwap(LocationOffsetInfo* other) {
  locationoffsetdata_.UnsafeArenaSwap(&other->locationoffsetdata_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata LocationOffsetInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LocationOffsetInfo_descriptor_;
  metadata.reflection = LocationOffsetInfo_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// LocationOffsetInfo

// repeated .LocationOffsetData locationOffsetData = 1;
int LocationOffsetInfo::locationoffsetdata_size() const {
  return locationoffsetdata_.size();
}
void LocationOffsetInfo::clear_locationoffsetdata() {
  locationoffsetdata_.Clear();
}
const ::LocationOffsetData& LocationOffsetInfo::locationoffsetdata(int index) const {
  // @@protoc_insertion_point(field_get:LocationOffsetInfo.locationOffsetData)
  return locationoffsetdata_.Get(index);
}
::LocationOffsetData* LocationOffsetInfo::mutable_locationoffsetdata(int index) {
  // @@protoc_insertion_point(field_mutable:LocationOffsetInfo.locationOffsetData)
  return locationoffsetdata_.Mutable(index);
}
::LocationOffsetData* LocationOffsetInfo::add_locationoffsetdata() {
  // @@protoc_insertion_point(field_add:LocationOffsetInfo.locationOffsetData)
  return locationoffsetdata_.Add();
}
::google::protobuf::RepeatedPtrField< ::LocationOffsetData >*
LocationOffsetInfo::mutable_locationoffsetdata() {
  // @@protoc_insertion_point(field_mutable_list:LocationOffsetInfo.locationOffsetData)
  return &locationoffsetdata_;
}
const ::google::protobuf::RepeatedPtrField< ::LocationOffsetData >&
LocationOffsetInfo::locationoffsetdata() const {
  // @@protoc_insertion_point(field_list:LocationOffsetInfo.locationOffsetData)
  return locationoffsetdata_;
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int LocationOffsetData::kOffsetLonFieldNumber;
const int LocationOffsetData::kOffsetLatFieldNumber;
const int LocationOffsetData::kOffsetDirFieldNumber;
const int LocationOffsetData::kOffsetSpeedFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

LocationOffsetData::LocationOffsetData()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:LocationOffsetData)
}

void LocationOffsetData::InitAsDefaultInstance() {
}

LocationOffsetData::LocationOffsetData(const LocationOffsetData& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:LocationOffsetData)
}

void LocationOffsetData::SharedCtor() {
  _cached_size_ = 0;
  offsetlon_ = 0;
  offsetlat_ = 0;
  offsetdir_ = 0;
  offsetspeed_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LocationOffsetData::~LocationOffsetData() {
  // @@protoc_insertion_point(destructor:LocationOffsetData)
  SharedDtor();
}

void LocationOffsetData::SharedDtor() {
  if (this != default_instance_) {
  }
}

void LocationOffsetData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LocationOffsetData::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LocationOffsetData_descriptor_;
}

const LocationOffsetData& LocationOffsetData::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_location_5foffset_5finfo_2eproto();
  return *default_instance_;
}

LocationOffsetData* LocationOffsetData::default_instance_ = NULL;

LocationOffsetData* LocationOffsetData::New(::google::protobuf::Arena* arena) const {
  LocationOffsetData* n = new LocationOffsetData;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void LocationOffsetData::Clear() {
// @@protoc_insertion_point(message_clear_start:LocationOffsetData)
  _extensions_.Clear();
#if defined(__clang__)
#define ZR_HELPER_(f) \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Winvalid-offsetof\"") \
  __builtin_offsetof(LocationOffsetData, f) \
  _Pragma("clang diagnostic pop")
#else
#define ZR_HELPER_(f) reinterpret_cast<char*>(\
  &reinterpret_cast<LocationOffsetData*>(16)->f)
#endif

#define ZR_(first, last) do {\
  ::memset(&first, 0,\
           ZR_HELPER_(last) - ZR_HELPER_(first) + sizeof(last));\
} while (0)

  ZR_(offsetlon_, offsetspeed_);

#undef ZR_HELPER_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool LocationOffsetData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:LocationOffsetData)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 offsetLon = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &offsetlon_)));
          set_has_offsetlon();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_offsetLat;
        break;
      }

      // required int32 offsetLat = 2;
      case 2: {
        if (tag == 16) {
         parse_offsetLat:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &offsetlat_)));
          set_has_offsetlat();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_offsetDir;
        break;
      }

      // required int32 offsetDir = 3;
      case 3: {
        if (tag == 24) {
         parse_offsetDir:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &offsetdir_)));
          set_has_offsetdir();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_offsetSpeed;
        break;
      }

      // required int32 offsetSpeed = 4;
      case 4: {
        if (tag == 32) {
         parse_offsetSpeed:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &offsetspeed_)));
          set_has_offsetspeed();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        if ((256u <= tag && tag < 1024u)) {
          DO_(_extensions_.ParseField(tag, input, default_instance_,
                                      mutable_unknown_fields()));
          continue;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:LocationOffsetData)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:LocationOffsetData)
  return false;
#undef DO_
}

void LocationOffsetData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:LocationOffsetData)
  // required int32 offsetLon = 1;
  if (has_offsetlon()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->offsetlon(), output);
  }

  // required int32 offsetLat = 2;
  if (has_offsetlat()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->offsetlat(), output);
  }

  // required int32 offsetDir = 3;
  if (has_offsetdir()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->offsetdir(), output);
  }

  // required int32 offsetSpeed = 4;
  if (has_offsetspeed()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->offsetspeed(), output);
  }

  // Extension range [32, 128)
  _extensions_.SerializeWithCachedSizes(
      32, 128, output);

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:LocationOffsetData)
}

::google::protobuf::uint8* LocationOffsetData::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:LocationOffsetData)
  // required int32 offsetLon = 1;
  if (has_offsetlon()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->offsetlon(), target);
  }

  // required int32 offsetLat = 2;
  if (has_offsetlat()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->offsetlat(), target);
  }

  // required int32 offsetDir = 3;
  if (has_offsetdir()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->offsetdir(), target);
  }

  // required int32 offsetSpeed = 4;
  if (has_offsetspeed()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->offsetspeed(), target);
  }

  // Extension range [32, 128)
  target = _extensions_.InternalSerializeWithCachedSizesToArray(
      32, 128, false, target);

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:LocationOffsetData)
  return target;
}

int LocationOffsetData::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:LocationOffsetData)
  int total_size = 0;

  if (has_offsetlon()) {
    // required int32 offsetLon = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetlon());
  }

  if (has_offsetlat()) {
    // required int32 offsetLat = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetlat());
  }

  if (has_offsetdir()) {
    // required int32 offsetDir = 3;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetdir());
  }

  if (has_offsetspeed()) {
    // required int32 offsetSpeed = 4;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetspeed());
  }

  return total_size;
}
int LocationOffsetData::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:LocationOffsetData)
  int total_size = 0;

  if (((_has_bits_[0] & 0x0000000f) ^ 0x0000000f) == 0) {  // All required fields are present.
    // required int32 offsetLon = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetlon());

    // required int32 offsetLat = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetlat());

    // required int32 offsetDir = 3;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetdir());

    // required int32 offsetSpeed = 4;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->offsetspeed());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  total_size += _extensions_.ByteSize();

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LocationOffsetData::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:LocationOffsetData)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const LocationOffsetData* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const LocationOffsetData>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:LocationOffsetData)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:LocationOffsetData)
    MergeFrom(*source);
  }
}

void LocationOffsetData::MergeFrom(const LocationOffsetData& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:LocationOffsetData)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_offsetlon()) {
      set_offsetlon(from.offsetlon());
    }
    if (from.has_offsetlat()) {
      set_offsetlat(from.offsetlat());
    }
    if (from.has_offsetdir()) {
      set_offsetdir(from.offsetdir());
    }
    if (from.has_offsetspeed()) {
      set_offsetspeed(from.offsetspeed());
    }
  }
  _extensions_.MergeFrom(from._extensions_);
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void LocationOffsetData::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:LocationOffsetData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LocationOffsetData::CopyFrom(const LocationOffsetData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:LocationOffsetData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LocationOffsetData::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;


  if (!_extensions_.IsInitialized()) return false;  return true;
}

void LocationOffsetData::Swap(LocationOffsetData* other) {
  if (other == this) return;
  InternalSwap(other);
}
void LocationOffsetData::InternalSwap(LocationOffsetData* other) {
  std::swap(offsetlon_, other->offsetlon_);
  std::swap(offsetlat_, other->offsetlat_);
  std::swap(offsetdir_, other->offsetdir_);
  std::swap(offsetspeed_, other->offsetspeed_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
  _extensions_.Swap(&other->_extensions_);
}

::google::protobuf::Metadata LocationOffsetData::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LocationOffsetData_descriptor_;
  metadata.reflection = LocationOffsetData_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// LocationOffsetData

// required int32 offsetLon = 1;
bool LocationOffsetData::has_offsetlon() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void LocationOffsetData::set_has_offsetlon() {
  _has_bits_[0] |= 0x00000001u;
}
void LocationOffsetData::clear_has_offsetlon() {
  _has_bits_[0] &= ~0x00000001u;
}
void LocationOffsetData::clear_offsetlon() {
  offsetlon_ = 0;
  clear_has_offsetlon();
}
 ::google::protobuf::int32 LocationOffsetData::offsetlon() const {
  // @@protoc_insertion_point(field_get:LocationOffsetData.offsetLon)
  return offsetlon_;
}
 void LocationOffsetData::set_offsetlon(::google::protobuf::int32 value) {
  set_has_offsetlon();
  offsetlon_ = value;
  // @@protoc_insertion_point(field_set:LocationOffsetData.offsetLon)
}

// required int32 offsetLat = 2;
bool LocationOffsetData::has_offsetlat() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void LocationOffsetData::set_has_offsetlat() {
  _has_bits_[0] |= 0x00000002u;
}
void LocationOffsetData::clear_has_offsetlat() {
  _has_bits_[0] &= ~0x00000002u;
}
void LocationOffsetData::clear_offsetlat() {
  offsetlat_ = 0;
  clear_has_offsetlat();
}
 ::google::protobuf::int32 LocationOffsetData::offsetlat() const {
  // @@protoc_insertion_point(field_get:LocationOffsetData.offsetLat)
  return offsetlat_;
}
 void LocationOffsetData::set_offsetlat(::google::protobuf::int32 value) {
  set_has_offsetlat();
  offsetlat_ = value;
  // @@protoc_insertion_point(field_set:LocationOffsetData.offsetLat)
}

// required int32 offsetDir = 3;
bool LocationOffsetData::has_offsetdir() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
void LocationOffsetData::set_has_offsetdir() {
  _has_bits_[0] |= 0x00000004u;
}
void LocationOffsetData::clear_has_offsetdir() {
  _has_bits_[0] &= ~0x00000004u;
}
void LocationOffsetData::clear_offsetdir() {
  offsetdir_ = 0;
  clear_has_offsetdir();
}
 ::google::protobuf::int32 LocationOffsetData::offsetdir() const {
  // @@protoc_insertion_point(field_get:LocationOffsetData.offsetDir)
  return offsetdir_;
}
 void LocationOffsetData::set_offsetdir(::google::protobuf::int32 value) {
  set_has_offsetdir();
  offsetdir_ = value;
  // @@protoc_insertion_point(field_set:LocationOffsetData.offsetDir)
}

// required int32 offsetSpeed = 4;
bool LocationOffsetData::has_offsetspeed() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
void LocationOffsetData::set_has_offsetspeed() {
  _has_bits_[0] |= 0x00000008u;
}
void LocationOffsetData::clear_has_offsetspeed() {
  _has_bits_[0] &= ~0x00000008u;
}
void LocationOffsetData::clear_offsetspeed() {
  offsetspeed_ = 0;
  clear_has_offsetspeed();
}
 ::google::protobuf::int32 LocationOffsetData::offsetspeed() const {
  // @@protoc_insertion_point(field_get:LocationOffsetData.offsetSpeed)
  return offsetspeed_;
}
 void LocationOffsetData::set_offsetspeed(::google::protobuf::int32 value) {
  set_has_offsetspeed();
  offsetspeed_ = value;
  // @@protoc_insertion_point(field_set:LocationOffsetData.offsetSpeed)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)