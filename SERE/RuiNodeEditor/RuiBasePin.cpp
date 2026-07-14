
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

template<>
void ImFlow::InPin<Float2Variable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s <%f,%f>",m_proto->name.c_str(),val().value.x, val().value.y);
    }
    else
    {
        if (ImGui::TreeNode(m_proto->name.c_str()))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("X",&m_emptyVal.value.x,0.002f,0.f,1.f);
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragFloat("Y",&m_emptyVal.value.y,0.002f,0.f,1.f);

            ImGui::TreePop();
        }
    }
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
        if (ImGui::TreeNode(m_proto->name.c_str()))
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
        if (ImGui::TreeNode(m_proto->name.c_str()))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::DragScalarN("size",ImGuiDataType_Float,disp,4);
            m_emptyVal.size = _mm_load_ps(disp);
            ImGui::TreePop();
        }


    }
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
        if (ImGui::TreeNode(m_proto->name.c_str()))
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

template<>
void ImFlow::InPin<StringVariable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s %s",m_proto->name.c_str(),val().value.c_str());
    }
    else
    {
        if (ImGui::TreeNode(m_proto->name.c_str()))
        {
            ImGui::SetNextItemWidth(120.f);
            ImGui::InputText("Value",&m_emptyVal.value,0);

            ImGui::TreePop();
        }
    }
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