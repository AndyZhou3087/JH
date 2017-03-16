#include "MyPackage.h"
#include "Const.h"
#include "CommonFuncs.h"

std::map<int,std::vector<PackageData>> MyPackage::map_packages;
int MyPackage::max = 5;
MyPackage::MyPackage()
{

}


MyPackage::~MyPackage()
{

}

int MyPackage::add(PackageData pdata)
{
	if (isFull(pdata))
		return -1;
	else
	{
		for (unsigned int i = 0; i < map_packages.size(); i++)
		{
			int size = map_packages[i].size();
			if (size > 0)
			{
				if (pdata.type == map_packages[i][0].id)
				{
					if (size < 10)
						map_packages[i].push_back(pdata);
				}
			}
			else
			{
				map_packages[i].push_back(pdata);
			}
		}
	}
}

void MyPackage::takeoff()
{
	for (unsigned int i = 0; i < map_packages.size(); i++)
		map_packages[i].clear();
	map_packages.clear();
}

bool MyPackage::isFull(PackageData pdata)
{
	int index = 0;
	bool ret = false;
	if (map_packages.size() == getMax())
	{
		for (unsigned int i = 0; i < map_packages.size(); i++)
		{
			if (map_packages[i].size() > 0)
			{
				if (map_packages[i][0].type != pdata.type)
					index++;
				else
				{
					if (map_packages[i].size() >= 10)
						ret = true;
					else
						ret = false;
				}

			}
		}
	}

	if (index == getMax())
		ret = true;
	return ret;
}

void MyPackage::save()
{

}

void MyPackage::load()
{

}

int MyPackage::getMax()
{
	return max;
}

void MyPackage::setMax(int val)
{
	max = val;
}