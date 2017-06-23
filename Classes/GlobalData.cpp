﻿#include "GlobalData.h"
#include "Building.h"
#include "StorageRoom.h"
#include "MyPackage.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "json.h"

std::map<std::string, std::vector<BuildActionData>> GlobalData::map_buidACData;

std::vector<ResData> GlobalData::vec_resData;

std::vector<std::string> GlobalData::vec_hillResid;

std::map<std::string, MapData> GlobalData::map_maps;

std::map<std::string, NpcData> GlobalData::map_npcs;

std::map<int, HeroAtrData> GlobalData::map_heroAtr;

std::map<std::string, WG_NGData> GlobalData::map_wgngs;

std::map<std::string, EquipData> GlobalData::map_equips;

std::vector<PlotMissionData> GlobalData::vec_PlotMissionData;

std::vector<PlotMissionData> GlobalData::vec_BranchPlotMissionData;

std::map<std::string, GFSkillData> GlobalData::map_gfskills;

bool GlobalData::unlockhero[4] = {true, false, false, false};

std::string GlobalData::uid = "";

std::vector<std::string> GlobalData::vec_saveids;

bool GlobalData::hasBuy = false;
int GlobalData::adsinterval = 10;
long GlobalData::adschoose = 11111111;

GlobalData::GlobalData()
{

}


GlobalData::~GlobalData()
{

}

void GlobalData::loadBuildActionJSon(std::string buildname)
{
	map_buidACData[buildname].clear();
	std::string jsonfilename = StringUtils::format("data/%s.json", buildname.c_str());
	rapidjson::Document doc = ReadJsonFile(jsonfilename);
	rapidjson::Value& bc = doc["bc"];
	for (unsigned int i = 0; i < bc.Size(); i++)//建筑物数组
	{
		BuildActionData data;
		rapidjson::Value& jsonvalue = bc[i];
		if (jsonvalue.IsObject())
		{
			rapidjson::Value& value = jsonvalue["id"];
			strcpy(data.icon, value.GetString());

			value = jsonvalue["blv"];
			data.blv = atoi(value.GetString());

			value = jsonvalue["time"];
			data.actime = atoi(value.GetString());

			if (jsonvalue.HasMember("extime"))
			{
				value = jsonvalue["extime"];
				data.extime = atoi(value.GetString());
			}
			else
				data.extime = 0;

			if (jsonvalue.HasMember("type"))
			{
				value = jsonvalue["type"];
				data.type = atoi(value.GetString());
			}
			else
				data.type = -1;

			value = jsonvalue["actext"];
			strcpy(data.actext, value.GetString());

			if (jsonvalue.HasMember("ep"))
			{
				value = jsonvalue["ep"];

				for (unsigned int m = 0; m < value.Size(); m++)
				{
					data.ep.push_back(value[m].GetInt());
				}
			}

			if (jsonvalue.HasMember("extype"))
			{
				value = jsonvalue["extype"];
				data.extype = atoi(value.GetString());
			}
			else
				data.extype = 0;

			value = jsonvalue["res"];
			for (unsigned int i = 0; i < value.Size(); i++)
			{
				int tmp = value[i].GetInt();
				if (tmp > 0)
					data.res.push_back(value[i].GetInt());
			}
			if (jsonvalue.HasMember("name"))
			{
				value = jsonvalue["name"];
				data.cname = value.GetString();
			}
			else
			{
				data.cname = "";
			}
			if (jsonvalue.HasMember("desc"))
			{
				value = jsonvalue["desc"];
				data.desc = value.GetString();
			}
			else
			{
				data.desc = "";
			}

			map_buidACData[buildname].push_back(data);
		}
	}
}

void GlobalData::loadResJsonData()
{
	vec_resData.clear();
	rapidjson::Document doc = ReadJsonFile("data/res.json");
	rapidjson::Value& values = doc["r"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		ResData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.strid = v.GetString();
		v = item["max"];
		data.max = atoi(v.GetString());

		data.count = data.max;
		data.pastmin = 0.0f;
		data.waittime = 0.0f;
		v = item["perhour"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.speed.push_back(v[m].GetDouble() * 60);
		}

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
	vec_hillResid.clear();
	rapidjson::Document doc = ReadJsonFile("data/homehill.json");
	rapidjson::Value& values = doc["sh"];
	for (unsigned int i = 0; i < values.Size(); i++)//后山显示的资源列表
	{
		vec_hillResid.push_back(values[i].GetString());
	}
}

void GlobalData::loadMapJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/map.json");
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//地图地点数组
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
	for (unsigned int i = 0; i < values.Size(); i++)//npc数组
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

		v = item["lv"];
		data.lv = atoi(v.GetString());

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
			int rnd = atoi(str.c_str());
			if (rnd > 0)
			{
				data.winresrnd.push_back(rnd);
				data.winrescount.push_back(-1);
			}
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
		v = item["word"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}


		map_npcs[data.id] = data;
	}
}

void GlobalData::saveResData()
{
	//保存资源数据每项分号（;）分割
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
	//解析保存的资源数据
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
	for (unsigned int i = 0; i < values.Size(); i++)//角色数组4个角色
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
			data.vec_atk.push_back(v[j].GetInt());
		}

		v = vitem["df"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_df.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}

		v = vitem["maxhp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_maxhp.push_back(v[j].GetInt());
		}
		map_heroAtr[data.id] = data;
	}
}

void GlobalData::loadWG_NGJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/wg.json");
	rapidjson::Value& values = doc["w"];
	for (unsigned int i = 0; i < values.Size(); i++)//外功数组
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
			data.vec_bns.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}
		v = vitem["qu"];
		data.qu = atoi(v.GetString());

		v = vitem["type"];
		data.type = atoi(v.GetString());

		map_wgngs[data.id] = data;
	}

	doc = ReadJsonFile("data/ng.json");
	rapidjson::Value& nvalues = doc["n"];
	int a = nvalues.Size();
	for (unsigned int i = 0; i < nvalues.Size(); i++)//内功数组
	{
		rapidjson::Value& vitem = nvalues[i];
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
			data.vec_bns.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}

		v = vitem["qu"];
		data.qu = atoi(v.GetString());

		map_wgngs[data.id] = data;
	}
}

void GlobalData::loadEquipJsonData()
{
	rapidjson::Document doc = ReadJsonFile("data/equip.json");
	rapidjson::Value& values = doc["ae"];
	for (unsigned int i = 0; i < values.Size(); i++)//武器防具数组
	{
		EquipData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["atk"];
		data.atk = atoi(v.GetString());
		v = item["df"];
		data.df = atoi(v.GetString());
		v = item["cname"];
		data.cname = v.GetString();
		v = item["desc"];
		data.desc = v.GetString();
		v = item["extype"];
		data.extype = atoi(v.GetString());
		v = item["qu"];
		data.qu = atoi(v.GetString());
		map_equips[data.id] = data;
	}
}

void GlobalData::loadUnlockHeroData()
{
	//解析角色是否解锁"-"分割
	std::string str = GameDataSave::getInstance()->getHeroUnlockData();
	std::vector<std::string> tmp;
	CommonFuncs::split(str, tmp, "-");

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		unlockhero[i] = atoi(tmp[i].c_str()) == 1 ? true : false;	
	}
	unlockhero[0] = true;
}

void GlobalData::setUnlockHero(int index, bool val)
{
	//保存角色是否解锁"-"分割
	unlockhero[index] = val;
	std::string str;
	for (int i = 0; i < 4; i++)
	{
		std::string tmp = StringUtils::format("%d-", unlockhero[i]?1:0);
		str.append(tmp);
	}
	GameDataSave::getInstance()->setHeroUnlockData(str.substr(0, str.length() - 1));
}

bool GlobalData::getUnlocHero(int index)
{
	return unlockhero[index];
}

int GlobalData::getSysSecTime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}

int GlobalData::getDayOfYear()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int day = tm->tm_yday;
	return day;
}

std::string GlobalData::getUId()
{
	uid = GameDataSave::getInstance()->getUserId();
	return uid;
}
void GlobalData::setUId(std::string struid)
{
	uid = struid;
	GameDataSave::getInstance()->setUserId(struid);
}

std::string GlobalData::getDefaultStorage(int heroindex)
{
	//默认仓库的数据
	
	std::string defaultdata = GameDataSave::getInstance()->getModifyDefaultStorage(heroindex);
	if (defaultdata.length() > 0)
		return defaultdata;
	else
	{
		return getOriginLocalStorage(heroindex);
	}
	return "";
}

std::string GlobalData::getOriginLocalStorage(int heroindex)
{
	rapidjson::Document doc = ReadJsonFile("data/defaultstorage.json");
	rapidjson::Value& values = doc["ds"];

	int size = values.Size();
	if (size > 0 && heroindex <= size)
	{
		rapidjson::Value& item = values[heroindex - 1];
		rapidjson::Value& v = item["val"];
		return v.GetString();
	}
	return "";
}

void GlobalData::setPlotMissionIndex(int val)
{
	int max = vec_PlotMissionData.size();
	if (val >= max)
		val = max - 1;
	GameDataSave::getInstance()->setPlotMissionIndex(val);
}

int GlobalData::getPlotMissionIndex()
{
	return GameDataSave::getInstance()->getPlotMissionIndex();
}


void GlobalData::setBranchPlotMissionIndex(int val)
{
	int max = vec_BranchPlotMissionData.size();
	if (val >= max)
		val = max - 1;
	GameDataSave::getInstance()->setBranchPlotMissionIndex(val);
}

int GlobalData::getBranchPlotMissionIndex()
{
	return GameDataSave::getInstance()->getBranchPlotMissionIndex();
}

void GlobalData::loadPlotMissionJsonData()
{
	int heroindex = GameDataSave::getInstance()->getHeroId();
	std::string plotfilename = "data/plotmission.json";
	if (heroindex == 2)
		plotfilename = "data/plotmission2.json";
	else if (heroindex == 3)
		plotfilename = "data/plotmission3.json";

	rapidjson::Document doc = ReadJsonFile(plotfilename);
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//剧情数组
	{
		rapidjson::Value& vitem = values[i];
		PlotMissionData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["snpc"];
		data.snpc = v.GetString();

		v = vitem["dnpc"];
		data.dnpc = v.GetString();

		v = vitem["unlock"];
		data.unlockchapter = atoi(v.GetString());

		v = vitem["t"];
		data.type = atoi(v.GetString());

		data.status = M_NONE;
		data.isshowdnpc = false;
		data.isshowsnpc = false;

		v = vitem["word"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}

		v = vitem["myword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.mywords.push_back(str);
		}

		v = vitem["bossword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.bossword.push_back(str);
		}
		v = vitem["rwds"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.rewords.push_back(str);
		}
		vec_PlotMissionData.push_back(data);
	}

	GlobalData::updatePlotMissionStatus();
}

void GlobalData::loadBranchPlotMissionJsonData()
{
	std::string plotfilename = "data/branchplotmission.json";

	rapidjson::Document doc = ReadJsonFile(plotfilename);
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//剧情数组
	{
		rapidjson::Value& vitem = values[i];
		PlotMissionData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["snpc"];
		data.snpc = v.GetString();

		v = vitem["dnpc"];
		data.dnpc = v.GetString();

		v = vitem["unlock"];
		data.unlockchapter = atoi(v.GetString());

		v = vitem["t"];
		data.type = atoi(v.GetString());

		data.status = M_NONE;
		data.isshowdnpc = false;
		data.isshowsnpc = false;

		v = vitem["word"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}

		v = vitem["myword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.mywords.push_back(str);
		}

		v = vitem["bossword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.bossword.push_back(str);
		}
		v = vitem["rwds"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.rewords.push_back(str);
		}
		vec_BranchPlotMissionData.push_back(data);
	}

	GlobalData::updateBranchPlotMissionStatus();
}

void GlobalData::savePlotMissionStatus()
{
	//保存剧情状态"-"分割
	std::string str;
	for (unsigned int i = 0; i < GlobalData::vec_PlotMissionData.size(); i++)
	{
		std::string tmpstr = StringUtils::format("%d-", GlobalData::vec_PlotMissionData[i].status);
		str.append(tmpstr);
	}
	GameDataSave::getInstance()->setPlotMissionStatus(str.substr(0, str.length() - 1));
}

void GlobalData::updatePlotMissionStatus()
{
	//解析剧情状态"-"分割
	std::string str = GameDataSave::getInstance()->getPlotMissionStatus();
	if (str.length() > 0)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(str, tmp, "-");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			GlobalData::vec_PlotMissionData[i].status = atoi(tmp[i].c_str());
		}
	}
}

void GlobalData::saveBranchPlotMissionStatus()
{
	//保存支线剧情状态"-"分割
	std::string str;
	for (unsigned int i = 0; i < GlobalData::vec_BranchPlotMissionData.size(); i++)
	{
		std::string tmpstr = StringUtils::format("%d-", GlobalData::vec_BranchPlotMissionData[i].status);
		str.append(tmpstr);
	}
	GameDataSave::getInstance()->setBranchPlotMissionStatus(str.substr(0, str.length() - 1));
}

void GlobalData::updateBranchPlotMissionStatus()
{
	//解析支线剧情状态"-"分割
	std::string str = GameDataSave::getInstance()->getBranchPlotMissionStatus();
	if (str.length() > 0)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(str, tmp, "-");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			GlobalData::vec_BranchPlotMissionData[i].status = atoi(tmp[i].c_str());
		}
	}
}

int GlobalData::getUnlockChapter()
{
	return GameDataSave::getInstance()->getPlotUnlockChapter();
}

void GlobalData::setUnlockChapter(int val)
{
	int lastChapter = GlobalData::getUnlockChapter();

	if (val > lastChapter)
		GameDataSave::getInstance()->setPlotUnlockChapter(val);
}

int GlobalData::createRandomNum(int val)
{
	int syssec = GlobalData::getSysSecTime();
	int static randNum = 0;
	randNum += 100;
	syssec += randNum;
	srand(syssec);
	int r = rand() % val;
	return r;
}

std::vector<std::string> GlobalData::getSaveListId()
{
	std::string str = GameDataSave::getInstance()->getSaveListId();

	std::vector<std::string> tmp;
	CommonFuncs::split(str, tmp, ";");
	vec_saveids.clear();
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		vec_saveids.push_back(tmp[i]);
	}

	return vec_saveids;
}

void GlobalData::setSaveListId(std::vector<std::string> vec_val)
{
	std::string str;
	for (unsigned int i = 0; i < vec_val.size(); i++)
	{
		str.append(vec_val[i]);
		str.append(";");
	}

	GameDataSave::getInstance()->setSaveListId(str.substr(0, str.length() - 1));
}

void GlobalData::setCurHeroIdToSaveList()
{
	std::string uid = GameDataSave::getInstance()->getUserId();

	int heroid = GameDataSave::getInstance()->getHeroIdByUid(uid);
	std::vector<std::string> vec_ids = GlobalData::getSaveListId();

	vec_ids[heroid - 1] = uid;

	GlobalData::setSaveListId(vec_ids);
}

bool GlobalData::isExercising()
{
	std::string str = GameDataSave::getInstance()->getExersiceCfg();
	return str.length() > 0 ? true : false;
}

bool GlobalData::isHasFSF()
{
	for (unsigned int m = 0; m < StorageRoom::map_storageData[RES_2].size(); m++)
	{
		PackageData* pData = &StorageRoom::map_storageData[RES_2][m];
		if (pData->strid.compare("72") == 0 && pData->count > 0)
		{
			return true;
		}
	}

	for (unsigned int m = 0; m < MyPackage::vec_packages.size(); m++)
	{
		PackageData* pData = &MyPackage::vec_packages[m];
		if (pData->strid.compare("72") == 0 && pData->count > 0)
		{
			return true;
		}
	}
	return false;
}

void GlobalData::loadGfskillData()
{
	rapidjson::Document doc = ReadJsonFile("data/gfskill.json");
	rapidjson::Value& values = doc["s"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		GFSkillData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["name"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			data.snames.push_back(str);
		}

		map_gfskills[data.id] = data;
	}
}

bool GlobalData::tempHasgf(std::string strid)
{
	std::vector<std::string> tempResId;

	std::map<std::string, MapData>::iterator it;

	for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	{
		std::string mapid = GlobalData::map_maps[it->first].strid;

		std::string datastr = GameDataSave::getInstance()->getTempStorage(mapid);
		if (datastr.length() > 0)
		{
			std::vector<std::string> vec_retstr;
			CommonFuncs::split(datastr, vec_retstr, ";");
			for (unsigned int i = 0; i < vec_retstr.size(); i++)
			{
				std::vector<std::string> tmp;
				CommonFuncs::split(vec_retstr[i], tmp, "-");
				tempResId.push_back(tmp[0]);
			}
		}
	}

	for (unsigned int i = 0; i < tempResId.size(); i++)
	{
		if (tempResId[i].compare(strid) == 0)
			return true;
	}
	return false;
}

int GlobalData::getShareDay()
{
	return GameDataSave::getInstance()->getShareDayOfYear();
}

void GlobalData::setShareDay(int day)
{
	GameDataSave::getInstance()->setShareDayOfYear(day);
}

std::string GlobalData::addUidString(std::string val)
{
    return uid + val;
}

void GlobalData::setNoAds(bool val)
{
    UserDefault::getInstance()->setBoolForKey(addUidString("noads").c_str(), val);
    UserDefault::getInstance()->flush();
}

bool GlobalData::getNoAds()
{
    return UserDefault::getInstance()->getBoolForKey(addUidString("noads").c_str(), false);
}

void GlobalData::setNoComments(bool val)
{
    UserDefault::getInstance()->setBoolForKey(addUidString("nocomments").c_str(), val);
    UserDefault::getInstance()->flush();
}

bool GlobalData::getNoComments()
{
    return UserDefault::getInstance()->getBoolForKey(addUidString("nocomments").c_str(), false);
}

void GlobalData::setDailyAdsCount(int val)
{
    UserDefault::getInstance()->setIntegerForKey(addUidString("dailyadscount").c_str(), val);
    UserDefault::getInstance()->flush();
}

int GlobalData::getDailyAdsCount()
{
    return UserDefault::getInstance()->getIntegerForKey(addUidString("dailyadscount").c_str(), 0);
}

void GlobalData::setDailyAdsDay(int val)
{
    UserDefault::getInstance()->setIntegerForKey(addUidString("dailyadsday").c_str(), val);
    UserDefault::getInstance()->flush();
}

int GlobalData::getDailyAdsDay()
{
    return UserDefault::getInstance()->getIntegerForKey(addUidString("dailyadsday").c_str(), 0);
}

void GlobalData::setAdsInterval(int val)
{
    adsinterval = val;
}

int GlobalData::getAdsInterval()
{
    return adsinterval;
}

void GlobalData::setAdsChoose(long val)
{
    adschoose = val;
}

int GlobalData::getAdsChoose(int position)
{
    /*long m = adschoose;
     int n =pow(10,position);
     float x = m/n;
     long i = (long)(m/n);
     int j = i % 10;
     return j;*/
    return (int)(adschoose/pow(10, position)) % 10;
}

void GlobalData::setHasBuy(bool val)
{
    hasBuy = val;
}

bool GlobalData::getHasBuy()
{
    return hasBuy;
}

