#pragma once

#include "Imgui/ImNodeFlow.h"
#include "RuiVariables.h"

template <>
void ImFlow::InPin<MathVariable>::drawNodeContent();

template <>
void ImFlow::InPin<MathVariable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<MathVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<FloatVariable>::drawNodeContent();

template <>
void ImFlow::InPin<FloatVariable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<FloatVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<Float2Variable>::drawNodeContent();

template <>
void ImFlow::InPin<Float2Variable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<Float2Variable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<Float3Variable>::drawNodeContent();

template <>
void ImFlow::InPin<Float3Variable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<Float3Variable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<ColorVariable>::drawNodeContent();

template <>
void ImFlow::InPin<ColorVariable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<ColorVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<TransformSize>::drawNodeContent();

template <>
void ImFlow::InPin<TransformSize>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<TransformSize>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<AssetVariable>::drawNodeContent();

template <>
void ImFlow::InPin<AssetVariable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<AssetVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);

template<>
void ImFlow::InPin<StringVariable>::drawNodeContent();

template <>
void ImFlow::InPin<StringVariable>::LoadEmptyValue(rapidjson::Value& value);

template <>
void ImFlow::InPin<StringVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator);
