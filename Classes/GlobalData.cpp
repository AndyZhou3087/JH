#include "GlobalData.h"
#include "Building.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "json.h"

std::vector<ResData> GlobalData::vec_resData;
std::vector<std::string> GlobalData::vec_hillResid;

std::map<std::string, MapData> GlobalData::map_maps;

std::map<std::string, NpcData> GlobalData::map_npcs;

std::map<int, HeroAtrData> GlobalData::map_heroAtr;

std::map<std::string, WG_NGData> GlobalData::map_wgngs;

GlobalData::GlobalData()
{

}


GlobalData::~GlobalData()
{

}

void GlobalData::loadResJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/res.json");
	rapidjson::Value& values = doc["r"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		ResData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.strid = v.GetString();
		v = item["max"];
		data.max = atoi(v.GetString());

		data.count = data.max;
		data.pastmin = 0;
		data.waittime = 0.0f;
		v = item["perhour"];
		data.speed = (int)(atof(v.GetString()) * 60);
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["actype"];
		data.actype = atoi(v.GetString());
		v = item["ep"];

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

		v = item["unit"];
		data.unitname = v.GetString();
		vec_resData.push_back(data);

	}
}

void GlobalData::loadHillResJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/homehill.json");
	rapidjson::Value& values = doc["sh"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		vec_hillResid.push_back(values[i].GetString());
	}
}

void GlobalData::loadMapJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/map.json");
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		MapData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		strcpy(data.strid, v.GetString());
		v = item["npc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.compare("0") != 0)
				data.npcs.push_back(npcid);
		}
		v = item["cname"];
		strcpy(data.cname, v.GetString());

		v = item["t"];
		strcpy(data.tpngname, v.GetString());

		v = item["desc"];
		data.desc = v.GetString();

		map_maps[data.strid] = data;

	}
}

void GlobalData::loadNpcJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/npc.json");
	rapidjson::Value& values = doc["n"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		NpcData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		strcpy(data.id, v.GetString());
		v = item["name"];
		strcpy(data.name, v.GetString());
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["life"];
		data.life = atoi(v.GetString());
		v = item["atk"];
		data.atk = atoi(v.GetString());
		v = item["df"];
		data.df = atoi(v.GetString());
		v = item["exp"];
		data.exp = atoi(v.GetString());

		v = item["winres"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.winres.push_back(str);
		}
		v = item["winresrnd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.winresrnd.push_back(atoi(str.c_str()));
		}
		v = item["exchg"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.exchgres.push_back(str);
		}
		v = item["exchgneed"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& ndresitem = v[m];
			if (ndresitem.Size() > 0)
			{
				std::vector<std::string> vec_temp;
				for (unsigned int n = 0; n < ndresitem.Size(); n++)
				{
					std::string str = ndresitem[m].GetString();
					if (str.length() > 1)
						vec_temp.push_back(str);
				}
				data.exchgneedres.push_back(vec_temp);
			}
		}
		map_npcs[data.id] = data;
	}
}

void GlobalData::saveResData()
{
	std::string str;
	for (unsigned int i = 0; i < vec_resData.size(); i++)
	{
		std::string onestr = StringUtils::format("%d-%d-%.1f;", vec_resData[i].count, vec_resData[i].pastmin, vec_resData[i].waittime);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setResData(str.substr(0, str.length() - 1));
}

void GlobalData::loadResData()
{
	std::string datastr = GameDataSave::getInstance()->getResData();
	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		vec_resData[i].count = atoi(tmp[0].c_str());
		vec_resData[i].pastmin = atoi(tmp[1].c_str());
		vec_resData[i].waittime = atof(tmp[2].c_str());
	}
}

void GlobalData::loadHeroAtrJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/heroatr.json");
	rapidjson::Value& values = doc["h"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		HeroAtrData data;
		rapidjson::Value& v = vitem["id"];
		data.id = atoi(v.GetString());

		v = vitem["name"];
		strcpy(data.name, v.GetString());

		v = vitem["atk"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_atk.push_back(v[i].GetInt());
		}

		v = vitem["df"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_df.push_back(v[i].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[i].GetInt());
		}

		v = vitem["maxhp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_maxhp.push_back(v[i].GetInt());
		}
		map_heroAtr[data.id] = data;
	}
}

void GlobalData::loadWG_NGJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/wg.json");
	rapidjson::Value& values = doc["w"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		WG_NGData data;
		rapidjson::Value& v = vitem["id"];
		strcpy(data.id, v.GetString());

		v = vitem["cname"];
		data.cname = v.GetString();

		v = vitem["maxlv"];
		data.maxlv = atoi(v.GetString());

		v = vitem["desc"];
		data.desc = v.GetString();

		v = vitem["bns"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_bns.push_back(v[i].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[i].GetInt());
		}
		data.lv = 0;
		map_wgngs[data.id] = data;
	}

	doc = ReadJsonFile("data/ng.json");
	values = doc["n"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		WG_NGData data;
		rapidjson::Value& v = vitem["id"];
		strcpy(data.id, v.GetString());

		v = vitem["cname"];
		data.cname = v.GetString();

		v = vitem["maxlv"];
		data.maxlv = atoi(v.GetString());

		v = vitem["desc"];
		data.desc = v.GetString();

		v = vitem["bns"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_bns.push_back(v[i].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[i].GetInt());
		}
		data.lv = 0;
		map_wgngs[data.id] = data;
	}
}

