#include "StorageRoom.h"
#include "Const.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

static std::map<int, std::vector<StorageData>> map_storageData;


StorageRoom::StorageRoom()
{
	loadStorageData();
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

void StorageRoom::saveStorageData()
{
	std::string str;
	for (int i = 0; i < map_storageData.size(); i++)
	{
		int size = map_storageData[i].size();
		for (int j = 0; j < size; j++)
		{
			StorageData sdata = map_storageData[i].at[j];
			int tempid = sdata.type * 1000 + sdata.id;
			str = StringUtils::format("%d-%d", tempid, sdata.count);
			if (j < size - 1)
				str.append(";");
		}
	}
	GameDataSave::getInstance()->setStorageData(str);
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

		StorageData sdata;
		int idtype = atoi(tmp2[0].c_str());
		sdata.type = idtype / 1000;
		sdata.id = idtype % 1000;
		sdata.count = atoi(tmp2[1].c_str());
		map_storageData[sdata.type].push_back(sdata);
	}
}

void StorageRoom::add(StorageData sdata)
{
	map_storageData[sdata.type].push_back(sdata);
}

void StorageRoom::use(int type, int id)
{
	for (unsigned int i = 0; i < map_storageData[type].size(); i++)
	{
		StorageData *sdata = &map_storageData[type][i];
		if (id == sdata->id)
		{
			if (--sdata->count <= 0)
			{
				map_storageData[type].erase(map_storageData[type].begin() + i);
			}
			break;
		}
	}
}