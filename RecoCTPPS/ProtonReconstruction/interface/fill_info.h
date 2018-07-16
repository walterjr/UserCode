#ifndef _fill_info_h_
#define _fill_info_h_

#include <vector>
#include <string>

//----------------------------------------------------------------------------------------------------

struct FillInfo
{
	unsigned int fillNumber;
	bool margin;
	unsigned int runMin, runMax;
	std::string alignmentTag;

	FillInfo(unsigned int _fn=0, bool _m=false, unsigned int _rmin=0, unsigned int _rmax=0, const std::string &at="") :
		fillNumber(_fn), margin(_m), runMin(_rmin), runMax(_rmax), alignmentTag(at)
	{
	}
};

//----------------------------------------------------------------------------------------------------

struct FillInfoCollection : public std::vector<FillInfo> 
{
	unsigned int FindByFill(unsigned int fill, bool margin, FillInfo &fillInfo) const
	{
		for (const auto it : *this)
		{
			if (it.fillNumber == fill && it.margin == margin)
            {
				fillInfo = it;
                return 0;
            }
		}

        return 1;
	}

	unsigned int FindByRun(unsigned int run, FillInfo &fillInfo) const
	{
		for (const auto it : *this)
		{
			if (it.runMin <= run && it.runMax >= run)
            {
				fillInfo = it;
                return 0;
            }
		}

		return 1;
	}
};

//----------------------------------------------------------------------------------------------------

FillInfoCollection fillInfoCollection;

//----------------------------------------------------------------------------------------------------

void InitFillInfoCollection()
{
	fillInfoCollection.push_back(FillInfo(4947, true, 273725, 273730, "period1_physics_margin/fill_4947/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4953, true, 274094, 274094, "period1_physics_margin/fill_4953/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4961, true, 274198, 274200, "period1_physics_margin/fill_4961/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4964, true, 274240, 274241, "period1_physics_margin/fill_4964/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4964, false, 274244, 274244, "period1_physics/fill_4964/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4976, true, 274282, 274286, "period1_physics_margin/fill_4976/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4985, false, 274387, 274388, "period1_physics/fill_4985/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4988, false, 274420, 274422, "period1_physics/fill_4988/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(4990, false, 274440, 274443, "period1_physics/fill_4990/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5005, false, 274954, 274959, "period1_physics/fill_5005/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5013, false, 274966, 274971, "period1_physics/fill_5013/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5017, false, 274998, 275001, "period1_physics/fill_5017/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5020, false, 275059, 275074, "period1_physics/fill_5020/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5021, false, 275124, 275125, "period1_physics/fill_5021/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5024, false, 275282, 275293, "period1_physics/fill_5024/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5026, false, 275309, 275311, "period1_physics/fill_5026/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5027, false, 275319, 275338, "period1_physics/fill_5027/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5028, false, 275344, 275345, "period1_physics/fill_5028/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5029, false, 275370, 275371, "period1_physics/fill_5029/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5030, false, 275375, 275376, "period1_physics/fill_5030/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5038, false, 275656, 275659, "period1_physics/fill_5038/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5043, false, 275757, 275783, "period1_physics/fill_5043/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5045, false, 275828, 275847, "period1_physics/fill_5045/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5048, false, 275886, 275890, "period1_physics/fill_5048/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5052, false, 275911, 275931, "period1_physics/fill_5052/2018_07_16"));

	fillInfoCollection.push_back(FillInfo(5261, false, 279760, 279767, "period1_physics/fill_5261/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5264, false, 279794, 279794, "period1_physics/fill_5264/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5265, false, 279823, 279823, "period1_physics/fill_5265/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5266, false, 279841, 279841, "period1_physics/fill_5266/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5267, false, 279844, 279865, "period1_physics/fill_5267/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5274, false, 279931, 279931, "period1_physics/fill_5274/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5275, false, 279966, 279966, "period1_physics/fill_5275/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5276, false, 279975, 279975, "period1_physics/fill_5276/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5277, false, 279993, 280024, "period1_physics/fill_5277/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5279, false, 280187, 280194, "period1_physics/fill_5279/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5287, false, 280327, 280364, "period1_physics/fill_5287/2018_07_16"));
	fillInfoCollection.push_back(FillInfo(5288, false, 280383, 280385, "period1_physics/fill_5288/2018_07_16"));

	fillInfoCollection.push_back(FillInfo(5393, false, 282730, 282735, "TODO"));
	fillInfoCollection.push_back(FillInfo(5401, false, 282920, 282924, "TODO"));
	fillInfoCollection.push_back(FillInfo(5405, false, 283040, 283043, "TODO"));
	fillInfoCollection.push_back(FillInfo(5406, false, 283049, 283067, "TODO"));
	fillInfoCollection.push_back(FillInfo(5418, false, 283305, 283308, "TODO"));
	fillInfoCollection.push_back(FillInfo(5421, false, 283353, 283359, "TODO"));
	fillInfoCollection.push_back(FillInfo(5423, false, 283407, 283416, "TODO"));
	fillInfoCollection.push_back(FillInfo(5424, false, 283453, 283453, "TODO"));
	fillInfoCollection.push_back(FillInfo(5427, false, 283478, 283481, "TODO"));
	fillInfoCollection.push_back(FillInfo(5433, false, 283548, 283560, "TODO"));
	fillInfoCollection.push_back(FillInfo(5437, false, 283672, 283685, "TODO"));
	fillInfoCollection.push_back(FillInfo(5439, false, 283820, 283835, "TODO"));
	fillInfoCollection.push_back(FillInfo(5441, false, 283863, 283865, "TODO"));
	fillInfoCollection.push_back(FillInfo(5442, false, 283876, 283878, "TODO"));
	fillInfoCollection.push_back(FillInfo(5443, false, 283884, 283885, "TODO"));
	fillInfoCollection.push_back(FillInfo(5446, false, 283933, 283934, "TODO"));
	fillInfoCollection.push_back(FillInfo(5448, false, 283946, 283964, "TODO"));
	fillInfoCollection.push_back(FillInfo(5450, false, 284006, 284014, "TODO"));
	fillInfoCollection.push_back(FillInfo(5451, false, 284025, 284044, "TODO"));
}

#endif
