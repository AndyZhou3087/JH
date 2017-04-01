#include "StorageRoom.h"
#include "Const.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

std::map<int, std::vector<PackageData>> StorageRoom::map_storageData;
StorageRoom::StorageRoom()
{
}


StorageRoom::~StorageRoom()
{

}

bool StorageRoom::init()
{
	if (Sprite::initWithSpriteFrameName("ui/storageroom.png"))
	{

		return true;
	}
	return false;
}

void StorageRoom::save()
{
	std::string str;

	std::map<int, std::vector<PackageData>>::iterator it; 
	for (it = map_storageData.begin(); it != map_storageData.end(); ++it)
	{
		int size = map_storageData[it->first].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = map_storageData[it->first][j];
			std::string idstr = StringUtils::format("%s-%d-%d-%d-%d;", sdata.strid.c_str(), sdata.type, sdata.count, sdata.extype, sdata.lv);
			str.append(idstr);
		}
	}
	GameDataSave::getInstance()->setStorageData(str.substr(0, str.length() - 1));
}

void StorageRoom::loadStorageData()
{
	std::string strval = GameDataSave::getInstance()->getStorageData();
	std::vector<std::string> tmp;
	CommonFuncs::split(strval, tmp, ";");

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		std::vector<std::string> tmp2;
		CommonFuncs::split(tmp[i], tmp2, "-");

		PackageData sdata;
		sdata.strid = tmp2[0];
		sdata.type = atoi(tmp2[1].c_str());
		sdata.count = atoi(tmp2[2].c_str());
		sdata.extype = atoi(tmp2[3].c_str());
		sdata.lv = atoi(tmp2[4].c_str());
		map_storageData[sdata.type].push_back(sdata);
	}
}

void StorageRoom::add(PackageData data)
{
	int typesize = map_storageData[data.type].size();
	int i = 0;
	for (i = 0; i < typesize; i++)
	{
		PackageData *sdata = &map_storageData[data.type][i];
		if (data.strid.compare(sdata->strid) == 0)
		{
			sdata->count += data.count;
			break;
		}
		
	}
	if (i == typesize)
		map_storageData[data.type].push_back(data);
	StorageRoom::save();
}

void StorageRoom::use(std::string strid)
{
	for (unsigned int type = 0; type < map_storageData.size(); type++)
	{
		for (unsigned int i = 0; i < map_storageData[type].size(); i++)
		{
			PackageData *sdata = &map_storageData[type][i];
			if (strid.compare(sdata->strid) == 0)
			{
				if (--sdata->count <= 0)
				{
					map_storageData[type].erase(map_storageData[type].begin() + i);
				}
				break;
			}
		}
	}
	StorageRoom::save();
}

int StorageRoom::getCountById(std::string strid)
{
	for (unsigned int type = 0; type < map_storageData.size(); type++)
	{
		for (unsigned int i = 0; i < map_storageData[type].size(); i++)
		{
			PackageData *sdata = &map_storageData[type][i];
			if (strid.compare(sdata->strid) == 0)
			{
				return sdata->count;
			}
		}
	}
	return 0;
}