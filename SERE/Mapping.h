#include <vector>
#include <algorithm>
#include "Imgui/implot.h"

class Mapping {

	class MappingValue_t {
	public:

		MappingValue_t(float xVal,float yVal,float direction,int id):x(xVal),y(yVal),dir(direction),id_(id){}


		void ShowPoint(bool cubicSpline) {
			ImPlotRect range =  ImPlot::GetPlotLimits();
			ImPlot::DragPoint(id_,&x, &y, ImVec4(0.2f, 0.8f, 0.f, 1.f));
			if (cubicSpline) {
				double xDir = x + range.Size().x * 0.02;
				double yDir = y + dir * .2;
				ImPlot::DragPoint(id_ + 1, &xDir, &yDir, ImVec4(0.7f, 0.2f, 0.f, 1.f), 4.f,ImPlotDragToolFlags_Delayed);
				dir = (yDir - y)*5.;
			}
		}

		double x;
		double y;
		double dir;
	private:
		int id_;
	};
public:
	void AddControlPoint() {
		controlPoints.emplace_back(controlPoints[controlPoints.size()-1].x + 1,controlPoints[controlPoints.size()-1].y,0.f,id);
		id +=2;
	}
	void AddControlPoint(float x, float y, float dir = 0.f) {
		controlPoints.emplace_back(x,y,dir,id);
		id +=2;
	}
	void RemoveControlPoint() {
		//cant be less than 3 values
		if(controlPoints.size()<=3)
			return;
		controlPoints.pop_back();
	}
	Mapping():cubicSpline(false),id(0) {
		AddControlPoint(0.f,0.f);
		AddControlPoint(1.f,1.f);
		AddControlPoint(2.f,2.f);
	}

	float MapVar(float xVal);
	void Sort();
	void ShowEditUi(float currentX);

	bool cubicSpline;
	std::vector<MappingValue_t> controlPoints;
private:
	int id;
};
