#include "GlobalData.h"
#include "json.h"

std::vector<ResData> GlobalData::vec_resData;
std::vector<PackageData> GlobalData::vec_getResData;

GlobalData::GlobalData()
{

}


GlobalData::~GlobalData()
{

}

void GlobalData::init()
{
	rapidjson::Document doc = ReadJsonFile("data/res.json");
	rapidjson::Value& values = doc["r"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		ResData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = atoi(v.GetString());
		v = item["max"];
		data.max = atoi(v.GetString());

		data.count = data.max;
		data.pastmin = 0;
		v = item["perhour"];
		data.speed = (int)(atof(v.GetString()) * 60);
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["actype"];
		data.actype = atoi(v.GetString());
		v = item["ep"];

		int a = v.Size();
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.ep.push_back(v[m].GetInt());
		}
		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.res.push_back(v[m].GetInt());
		}
		v = item["cname"];
		strcpy(data.cname, v.GetString());
		v = item["desc"];
		strcpy(data.desc, v.GetString());
		vec_resData.push_back(data);

	}
}