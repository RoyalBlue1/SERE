#include "Mapping.h"


float Mapping::MapVar(float xVal) {

	float percentageXDiff; // xmm7_4
	float dataXDifference; // xmm6_4


	float inversePercentageXDiff;
	float float_10;
	float float_14;
	uint32_t v5 = 0;

	if (values[0].x < xVal)
	{
		if ( xVal < values[values.size()-1].x)
		{
			float lowerDataX = values[0].x;
			float highterDataX = values[1].x;

			while ( xVal > highterDataX)
			{
				lowerDataX = highterDataX;
				highterDataX = values[2 + v5++].x;
			}


			dataXDifference = highterDataX - lowerDataX;
			percentageXDiff = (float)(xVal - lowerDataX) / dataXDifference;
		}
		else
		{
			v5 = values.size() - 2;
			percentageXDiff = 1.0;
			dataXDifference = 1.0;
		}
	}
	else
	{
		percentageXDiff = 0.0;
		dataXDifference = 0.0;
	}

	if ( cubicSpline )
	{

		//ystart = &data[mapping->sampleCount+(2 * v5 * mapping->plotCount)];

		inversePercentageXDiff = (float)((float)(percentageXDiff * 2.0f) + 1.0f)
			* (float)((float)(percentageXDiff - 1.0f) * (float)(percentageXDiff - 1.0f));
		percentageXDiff = (float)(3.0f - (float)(percentageXDiff * 2.0f)) * (float)(percentageXDiff * percentageXDiff);
		float_10 = (float)((float)((float)(percentageXDiff - 1.0f) * (float)(percentageXDiff - 1.0f)) * dataXDifference)
			* percentageXDiff;
		float_14 = (float)((float)(percentageXDiff * percentageXDiff) * dataXDifference)
			* (float)(percentageXDiff - 1.0f);
	}
	else
	{
		percentageXDiff = percentageXDiff;

		//ystart = &data[mapping->sampleCount + v5 * mapping->plotCount];
		inversePercentageXDiff = (float)(1.0f - percentageXDiff);
		float_10 = 0.f;
		float_14 = 0.f;
	}




	if ( cubicSpline )
		return (float)((float)((float)(float_10 * values[v5].dir) + inversePercentageXDiff * values[v5].y) + (float)(percentageXDiff * values[v5+1].y))
		+ (float)(float_14 * values[v5 +1].dir);
	else
		return (float)(percentageXDiff * values[v5 +1].y) + inversePercentageXDiff * values[v5].y;

}
void Mapping::Sort() {
	std::sort(values.begin(), values.end(), [](const MappingValue_t& a, const MappingValue_t& b) {
		return a.x < b.x;
	});
}

void Mapping::ShowEditUi(float currentX) {
	ImGui::Checkbox("Use Cubic Spline",&cubicSpline);
	ImGui::SameLine();
	if(ImGui::Button("Add Value")) {
		AddValue();
	}
	ImGui::SameLine();
	if(ImGui::Button("Remove Value")) {
		RemoveValue();
	}
	if(ImPlot::BeginPlot("test",ImVec2(600,400),ImPlotFlags_NoTitle)) {
		ImPlotRect range =  ImPlot::GetPlotLimits();

		for (auto& value:values) {
			value.ShowPoint(cubicSpline);
		}
		Sort();


		std::vector<ImVec2> points;
		for (float x = range.Min().x; x < range.Max().x; x += (range.Size().x / 200.f)) {
			points.push_back({x,MapVar(x)});
		}

		ImPlot::PlotLine("Mapping",&points[0].x,&points[0].y,points.size(),0,0,sizeof(ImVec2));

		ImPlot::PushStyleColor(ImPlotCol_Line,ImVec4(.0f,0.2f,0.8f,1.f));
		ImPlot::PlotInfLines("Input X",&currentX,1);
		ImPlot::PopStyleColor();

		ImPlot::EndPlot();
	}


}