#pragma once

#include "Imgui/ImNodeFlow.h"
#include "RuiVariables.h"

template <>
void ImFlow::InPin<MathVariable>::drawNodeContent();

template<>
void ImFlow::InPin<FloatVariable>::drawNodeContent();

template<>
void ImFlow::InPin<Float2Variable>::drawNodeContent();

template<>
void ImFlow::InPin<Float3Variable>::drawNodeContent();

template<>
void ImFlow::InPin<ColorVariable>::drawNodeContent();

template<>
void ImFlow::InPin<TransformSize>::drawNodeContent();

template<>
void ImFlow::InPin<AssetVariable>::drawNodeContent();

template<>
void ImFlow::InPin<StringVariable>::drawNodeContent();