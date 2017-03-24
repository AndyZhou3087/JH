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

		PackageData sdata;
		int idtype = atoi(tmp2[0].c_str());
		sdata.type = idtype / 1000;
		sdata.id = idtype % 1000;
		sdata.count = atoi(tmp2[1].c_str());
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
		if (data.id == sdata->id)
		{
			sdata->count += data.count;
			break;
		}
		
	}
	if (i == typesize)
		map_storageData[data.type].push_back(data);
	StorageRoom::save();
}

void StorageRoom::use(int id)
{
	for (unsigned int type = 0; type < map_storageData.size(); type++)
	{
		for (unsigned int i = 0; i < map_storageData[type].size(); i++)
		{
			PackageData *sdata = &map_storageData[type][i];
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
			PackageData *sdata = &map_storageData[type][i];
			if (id == sdata->id)
			{
				return sdata->count;
			}
		}
	}
	return 0;
}