
#include "RuiBasePin.h"

#include "CustomImGuiWidgets.h"
#include "Imgui/imgui_stdlib.h"

template<>
void ImFlow::InPin<FloatVariable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s %f",m_proto->name.c_str(),val().value);
    }
    else
    {
        ImGui::SetNextItemWidth(120.f);
        ImGui::DragFloat(m_proto->name.c_str(), &m_emptyVal.value,0.002f,0.f,1.f);
    }
}

template <>
void ImFlow::InPin<FloatVariable>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsFloat())
        return;
    m_emptyVal.value = value.GetFloat();
}

template <>
void ImFlow::InPin<FloatVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{

    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        rapidjson::Value().SetFloat(m_emptyVal.value),
        allocator);
}

template<>
void ImFlow::InPin<Float2Variable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s <%f,%f>",m_proto->name.c_str(),val().value.x, val().value.y);
    }
    else
    {
        if (ImGui::TreeNodeEx(m_proto->name.c_str(),ImGuiTreeNodeFlags_SpanLabelWidth))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("X",&m_emptyVal.value.x,0.002f,0.f,1.f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("Y",&m_emptyVal.value.y,0.002f,0.f,1.f);

            ImGui::TreePop();
        }
    }
}

template <>
void ImFlow::InPin<Float2Variable>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsObject())
        return;
    if (value.HasMember("X")&&value["X"].IsFloat())
        m_emptyVal.value.x = value["X"].GetFloat();
    if (value.HasMember("Y")&&value["Y"].IsFloat())
        m_emptyVal.value.y = value["Y"].GetFloat();
}

template <>
void ImFlow::InPin<Float2Variable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::GenericValue<rapidjson::UTF8<>> vector;
    vector.SetObject();
    vector.AddMember("X",m_emptyVal.value.x,allocator);
    vector.AddMember("Y",m_emptyVal.value.y,allocator);
    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        vector,
        allocator);
}

template<>
void ImFlow::InPin<Float3Variable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s <%f,%f,%f>",m_proto->name.c_str(),val().value.x, val().value.y,val().value.z);
    }
    else
    {
        if (ImGui::TreeNodeEx(m_proto->name.c_str(),ImGuiTreeNodeFlags_SpanLabelWidth))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("X",&m_emptyVal.value.x,0.002f,0.f,1.f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("Y",&m_emptyVal.value.y,0.002f,0.f,1.f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("Z",&m_emptyVal.value.z,0.002f,0.f,1.f);

            ImGui::TreePop();
        }
    }
}

template <>
void ImFlow::InPin<Float3Variable>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsObject())
        return;
    if (value.HasMember("X")&&value["X"].IsFloat())
        m_emptyVal.value.x = value["X"].GetFloat();
    if (value.HasMember("Y")&&value["Y"].IsFloat())
        m_emptyVal.value.y = value["Y"].GetFloat();
    if (value.HasMember("Z")&&value["Z"].IsFloat())
        m_emptyVal.value.z = value["Z"].GetFloat();
}

template <>
void ImFlow::InPin<Float3Variable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::GenericValue<rapidjson::UTF8<>> vector;
    vector.SetObject();
    vector.AddMember("X",m_emptyVal.value.x,allocator);
    vector.AddMember("Y",m_emptyVal.value.y,allocator);
    vector.AddMember("Z",m_emptyVal.value.z,allocator);
    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        vector,
        allocator);
}

template<>
void ImFlow::InPin<ColorVariable>::drawNodeContent()
{
    if (isConnected())
    {
        float disp[4];
        memcpy(disp,&val().value,sizeof(float)*4);
        ImGui::ColorEdit4(m_proto->name.c_str(),disp,ImGuiColorEditFlags_NoPicker|ImGuiColorEditFlags_NoOptions|ImGuiColorEditFlags_NoInputs);
    }
    else
    {

        ImGui::ColorEdit4(m_proto->name.c_str(),&m_emptyVal.value.red,ImGuiColorEditFlags_NoInputs);

    }
}

template <>
void ImFlow::InPin<ColorVariable>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsObject())
        return;
    if (value.HasMember("Red")&&value["Red"].IsFloat())
        m_emptyVal.value.red = value["Red"].GetFloat();
    if (value.HasMember("Green")&&value["Green"].IsFloat())
        m_emptyVal.value.green = value["Green"].GetFloat();
    if (value.HasMember("Blue")&&value["Blue"].IsFloat())
        m_emptyVal.value.blue = value["Blue"].GetFloat();
    if (value.HasMember("Alpha")&&value["Alpha"].IsFloat())
        m_emptyVal.value.alpha = value["Alpha"].GetFloat();
}

template <>
void ImFlow::InPin<ColorVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::GenericValue<rapidjson::UTF8<>> vector;
    vector.SetObject();
    vector.AddMember("Red",m_emptyVal.value.red,allocator);
    vector.AddMember("Green",m_emptyVal.value.green,allocator);
    vector.AddMember("Blue",m_emptyVal.value.blue,allocator);
    vector.AddMember("Alpha",m_emptyVal.value.alpha,allocator);

    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        vector,
        allocator);
}

template<>
void ImFlow::InPin<TransformSize>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text(m_proto->name.c_str());
    }
    else
    {
        float disp[4];
        _mm_store_ps(disp,m_emptyVal.size);
        if (ImGui::TreeNodeEx(m_proto->name.c_str(),ImGuiTreeNodeFlags_SpanLabelWidth))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragScalar("X",ImGuiDataType_Float,&disp[0],0.02f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragScalar("Y",ImGuiDataType_Float,&disp[1],0.02f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragScalar("Z",ImGuiDataType_Float,&disp[2],0.02f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragScalar("W",ImGuiDataType_Float,&disp[3],0.02f);
            m_emptyVal.size = _mm_load_ps(disp);
            ImGui::TreePop();
        }


    }
}

template <>
void ImFlow::InPin<TransformSize>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsObject())
        return;
    float size[4];
    if (value.HasMember("X")&&value["X"].IsFloat())
        size[0] = value["X"].GetFloat();
    if (value.HasMember("Y")&&value["Y"].IsFloat())
        size[1] = value["Y"].GetFloat();
    if (value.HasMember("Z")&&value["Z"].IsFloat())
        size[2] = value["Z"].GetFloat();
    if (value.HasMember("W")&&value["W"].IsFloat())
        size[3] = value["W"].GetFloat();
}

template <>
void ImFlow::InPin<TransformSize>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{
    float size[4];
    _mm_store_ps(size,m_emptyVal.size);
    rapidjson::GenericValue<rapidjson::UTF8<>> vector;
    vector.SetObject();
    vector.AddMember("X",size[0],allocator);
    vector.AddMember("Y",size[1],allocator);
    vector.AddMember("Z",size[2],allocator);
    vector.AddMember("W",size[3],allocator);

    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        vector,
        allocator);
}

template<>
void ImFlow::InPin<AssetVariable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s",m_proto->name.c_str());
    }
    else
    {
        ImGui::PushItemWidth(120.f);
        if (ImGui::TreeNodeEx(m_proto->name.c_str(),ImGuiTreeNodeFlags_SpanLabelWidth))
        {

            if (AtlasImageButton("Image",m_emptyVal.hash))
            {
                ImGui::OpenPopup("Select Asset");
            }
            AssetSelectionPopup("Select Asset",&m_emptyVal.hash);
            ImGui::SameLine();
            if (ImGui::SmallButton("X"))
            {
                m_emptyVal.hash = INVALID_ASSET;
            }
            ImGui::TreePop();
        }
        ImGui::PopItemWidth();
    }
}

template <>
void ImFlow::InPin<AssetVariable>::LoadEmptyValue(rapidjson::Value& value)
{

}

template <>
void ImFlow::InPin<AssetVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{

}

template<>
void ImFlow::InPin<StringVariable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s %s",m_proto->name.c_str(),val().value.c_str());
    }
    else
    {
        if (ImGui::TreeNodeEx(m_proto->name.c_str(),ImGuiTreeNodeFlags_SpanLabelWidth))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::InputText("Value",&m_emptyVal.value,0);

            ImGui::TreePop();
        }
    }
}

template <>
void ImFlow::InPin<StringVariable>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsString())
        return;
    m_emptyVal.value = value.GetString();
}

template <>
void ImFlow::InPin<StringVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{
    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        rapidjson::Value(m_emptyVal.value.c_str(),allocator),
        allocator);
}

template <>
void ImFlow::InPin<MathVariable>::drawNodeContent()
{
    if (isConnected()){
        ImGui::Text("%s",m_proto->name.c_str());
    }
    else
    {

        if (m_emptyVal.Type() == MathVariableType::FLOAT)
        {
            FloatVariable& val = std::get<FloatVariable>(m_emptyVal.value);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragScalar(m_proto->name.c_str(), ImGuiDataType_Float,&val.value,0.002f);

        }

    }
}

template <>
void ImFlow::InPin<MathVariable>::LoadEmptyValue(rapidjson::Value& value)
{
    if (!value.IsFloat())
        return;
    m_emptyVal.value = FloatVariable(value.GetFloat());
}

template <>
void ImFlow::InPin<MathVariable>::StoreEmptyValue(rapidjson::GenericValue<rapidjson::UTF8<>>& object, rapidjson::Document::AllocatorType& allocator)
{

    FloatVariable& val = std::get<FloatVariable>(m_emptyVal.value);
    object.AddMember(
        rapidjson::Value(m_proto->name.c_str(),allocator),
        rapidjson::Value().SetFloat(val.value),
        allocator);
}
