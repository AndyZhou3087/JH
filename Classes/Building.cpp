#include "Building.h"

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

};