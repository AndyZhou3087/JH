#include "StorageRoom.h"
#include "Const.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

std::map<int, std::vector<StorageData>> StorageRoom::map_storageData;
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

void StorageRoom::save()
{
	std::string str;
	int mapsize = map_storageData.size();
	for (int i = 0; i < mapsize; i++)
	{
		int size = map_storageData[i].size();
		for (int j = 0; j < size; j++)
		{
			StorageData sdata = map_storageData[i][j];
			int tempid = sdata.type * 1000 + sdata.id;
			std::string idstr = StringUtils::format("%d-%d;", tempid, sdata.count);
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

		StorageData sdata;
		int idtype = atoi(tmp2[0].c_str());
		sdata.type = idtype / 1000;
		sdata.id = idtype % 1000;
		sdata.count = atoi(tmp2[1].c_str());
		map_storageData[sdata.type].push_back(sdata);
	}
}

void StorageRoom::add(StorageData data)
{
	bool isnew = true;
	for (unsigned int i = 0; i < map_storageData[data.type].size(); i++)
	{
		StorageData *sdata = &map_storageData[data.type][i];
		if (data.id == sdata->id)
		{
			isnew = false;
			sdata->count += data.count;
		}
		else
		{
			isnew = true;
		}
	}
	if (isnew)
		map_storageData[data.type].push_back(data);
	StorageRoom::save();
}

void StorageRoom::use(int id)
{
	for (unsigned int type = 0; type < map_storageData.size(); type++)
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
	StorageRoom::save();
}

int StorageRoom::getCountByTypeId(int id)
{
	for (unsigned int type = 0; type < map_storageData.size(); type++)
	{
		for (unsigned int i = 0; i < map_storageData[type].size(); i++)
		{
			StorageData *sdata = &map_storageData[type][i];
			if (id == sdata->id)
			{
				return sdata->count;
			}
		}
	}
	return 0;
}