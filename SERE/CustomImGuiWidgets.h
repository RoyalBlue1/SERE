#include "Imgui/imgui.h"
#include "ImageAtlas.h"
#include <string>
#include <algorithm>

struct Mapping {

	struct MappingValue_t {
		float x;
		float y;
		float dir;
        int id;
	};
	bool cubicSpline;
	std::vector<MappingValue_t> values;
    int id;
	void AddValue() {
		MappingValue_t val;
		val.x = values[values.size()-1].x + 1;
		val.y = values[values.size()-1].y;
		val.dir = 0;
        val.id = id;
        id += 2;
		values.push_back(val);
	}
	void RemoveValue() {
		//cant be less than 3 values
		if(values.size()<=3)
			return;
		values.pop_back();
	}
	Mapping() {
		cubicSpline = false;
		MappingValue_t val;
		val.x = 0;
		val.y = 0;
		val.dir = 0;
        val.id = 0;
		values.push_back(val);
		val.x = 1;
		val.y = 1;
        val.id = 2;
		values.push_back(val);
		val.x = 2;
		val.y = 2;
        val.id = 4;
		values.push_back(val);
        id = 6;
	}


    float MapVar(float xVal) {

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
	void sort() {
		std::sort(values.begin(), values.end(), [](const MappingValue_t& a, const MappingValue_t& b) {
			return a.x < b.x;
		});
	}
};

void AssetSelectionPopup(const char* id, uint32_t* hash);
bool AtlasImageButton(const char* id, uint32_t hash,ImVec2 maxSize = {100.f,200.f});
bool AtlasImageButton(const char* id, const Asset_t& asset,ImVec2 maxSize = {100.f,200.f});
bool Slider2D(const char* id,float* xVal,float* yVal);
void MappingCreationPopup(const char* id, Mapping& map);