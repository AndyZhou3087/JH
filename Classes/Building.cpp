#include "Building.h"
#include "Nature.h"
#include "Const.h"

extern Nature* g_nature;

Building::Building()
{

}


Building::~Building()
{

}

void Building::parseData(rapidjson::Value& jsonvalue)
{
	if (jsonvalue.IsObject())
	{
		rapidjson::Value& value = jsonvalue["name"];
		strcpy(data.name, value.GetString());

		value = jsonvalue["cname"];
		strcpy(data.cname, value.GetString());

		data.level = 0;
		value = jsonvalue["maxlevel"];
		data.maxlevel = value.GetInt();
		value = jsonvalue["needtime"];
		for (int i = 0; i < data.maxlevel; i++)
		{
			data.needtime.push_back(value[i].GetInt());
		}

		value = jsonvalue["needres"];
		for (int i = 0; i < data.maxlevel; i++)
		{
			std::vector<int> res;
			rapidjson::Value& resvalue = value[i];
			int size = resvalue.Size();
			for (int j = 0; j < size; j++)
			{
				res.push_back(resvalue[j].GetInt());
			}
			data.Res.push_back(res);
		}
	}

}

void Building::build()
{
	float time = data.needtime[data.level] / (ACTION_BAR_TIME * TIMESCALE);
	g_nature->setTimeInterval(time);
}

void Building::action(int minute, int exminute)
{

}