
#include "RuiBasePin.h"
#include "Imgui/ImNodeFlow.h"
#include "RuiVariables.h"
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
        ImGui::DragScalar(m_proto->name.c_str(), ImGuiDataType_Float, &m_emptyVal.value);
    }
}

template<>
void ImFlow::InPin<Float2Variable>::drawNodeContent()
{
    if (isConnected())
    {
        ImGui::Text("%s %f %f",m_proto->name.c_str(),val().value.x, val().value.y);
    }
    else
    {
        ImGui::SetNextItemWidth(120.f);
        ImGui::DragScalarN(m_proto->name.c_str(), ImGuiDataType_Float, &m_emptyVal.value.x,2);
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
