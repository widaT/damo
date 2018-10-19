// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: feature.proto

#ifndef PROTOBUF_INCLUDED_feature_2eproto
#define PROTOBUF_INCLUDED_feature_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3006001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_feature_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_feature_2eproto {
  static const ::google::protobuf::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors_feature_2eproto();
namespace featurepb {
class Feature;
class FeatureDefaultTypeInternal;
extern FeatureDefaultTypeInternal _Feature_default_instance_;
}  // namespace featurepb
namespace google {
namespace protobuf {
template<> ::featurepb::Feature* Arena::CreateMaybeMessage<::featurepb::Feature>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace featurepb {

// ===================================================================

class Feature : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:featurepb.Feature) */ {
 public:
  Feature();
  virtual ~Feature();

  Feature(const Feature& from);

  inline Feature& operator=(const Feature& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Feature(Feature&& from) noexcept
    : Feature() {
    *this = ::std::move(from);
  }

  inline Feature& operator=(Feature&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const Feature& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Feature* internal_default_instance() {
    return reinterpret_cast<const Feature*>(
               &_Feature_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Feature* other);
  friend void swap(Feature& a, Feature& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Feature* New() const final {
    return CreateMaybeMessage<Feature>(NULL);
  }

  Feature* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Feature>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Feature& from);
  void MergeFrom(const Feature& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Feature* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated float feature = 1;
  int feature_size() const;
  void clear_feature();
  static const int kFeatureFieldNumber = 1;
  float feature(int index) const;
  void set_feature(int index, float value);
  void add_feature(float value);
  const ::google::protobuf::RepeatedField< float >&
      feature() const;
  ::google::protobuf::RepeatedField< float >*
      mutable_feature();

  // @@protoc_insertion_point(class_scope:featurepb.Feature)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< float > feature_;
  mutable std::atomic<int> _feature_cached_byte_size_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_feature_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Feature

// repeated float feature = 1;
inline int Feature::feature_size() const {
  return feature_.size();
}
inline void Feature::clear_feature() {
  feature_.Clear();
}
inline float Feature::feature(int index) const {
  // @@protoc_insertion_point(field_get:featurepb.Feature.feature)
  return feature_.Get(index);
}
inline void Feature::set_feature(int index, float value) {
  feature_.Set(index, value);
  // @@protoc_insertion_point(field_set:featurepb.Feature.feature)
}
inline void Feature::add_feature(float value) {
  feature_.Add(value);
  // @@protoc_insertion_point(field_add:featurepb.Feature.feature)
}
inline const ::google::protobuf::RepeatedField< float >&
Feature::feature() const {
  // @@protoc_insertion_point(field_list:featurepb.Feature.feature)
  return feature_;
}
inline ::google::protobuf::RepeatedField< float >*
Feature::mutable_feature() {
  // @@protoc_insertion_point(field_mutable_list:featurepb.Feature.feature)
  return &feature_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace featurepb

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_feature_2eproto
