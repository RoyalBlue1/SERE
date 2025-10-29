#include "Mapping.h"


float Mapping::MapVar(float xVal) {

	float percentageXDiff; // xmm7_4
	float dataXDifference; // xmm6_4


	float inversePercentageXDiff;
	float float_10;
	float float_14;
	uint32_t v5 = 0;

	if (controlPoints[0].x < xVal)
	{
		if ( xVal < controlPoints[controlPoints.size()-1].x)
		{
			float lowerDataX = (float)controlPoints[0].x;
			float highterDataX = (float)controlPoints[1].x;

			while ( xVal > highterDataX)
			{
				lowerDataX = highterDataX;
				highterDataX = (float)controlPoints[2 + v5++].x;
			}


			dataXDifference = highterDataX - lowerDataX;
			percentageXDiff = (float)(xVal - lowerDataX) / dataXDifference;
		}
		else
		{
			v5 = (uint32_t)controlPoints.size() - 2;
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
		return (float)((float)((float)(float_10 * controlPoints[v5].dir) + inversePercentageXDiff * controlPoints[v5].y) + (float)(percentageXDiff * controlPoints[v5+1].y))
		+ (float)(float_14 * controlPoints[v5 +1].dir);
	else
		return (float)(percentageXDiff * controlPoints[v5 +1].y) + (float)(inversePercentageXDiff * controlPoints[v5].y);

}
void Mapping::Sort() {
	std::sort(controlPoints.begin(), controlPoints.end(), [](const MappingValue_t& a, const MappingValue_t& b) {
		return a.x < b.x;
	});
}

void Mapping::ShowEditUi(float currentX) {
	ImGui::Checkbox("Use Cubic Spline",&cubicSpline);
	ImGui::SameLine();
	if(ImGui::Button("Add Control Point")) {
		AddControlPoint();
	}
	ImGui::SameLine();
	if(ImGui::Button("Remove Control Point")) {
		RemoveControlPoint();
	}
	if(ImPlot::BeginPlot("Mapping Plot",ImVec2(600,400),ImPlotFlags_NoTitle)) {
		ImPlotRect range =  ImPlot::GetPlotLimits();

		for (auto& point:controlPoints) {
			point.ShowPoint(cubicSpline);
		}
		Sort();


		std::vector<ImVec2> resultLine;
		for (float x = range.Min().x; x < range.Max().x; x += (range.Size().x / 200.f)) {
			resultLine.push_back({x,MapVar(x)});
		}

		ImPlot::PlotLine("Mapping",&resultLine[0].x,&resultLine[0].y,resultLine.size(),0,0,sizeof(ImVec2));

		ImPlot::PushStyleColor(ImPlotCol_Line,ImVec4(.0f,0.2f,0.8f,1.f));
		ImPlot::PlotInfLines("Input X",&currentX,1);
		ImPlot::PopStyleColor();

		ImPlot::EndPlot();
	}


}