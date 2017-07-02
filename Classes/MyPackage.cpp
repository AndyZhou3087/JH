﻿#include "MyPackage.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "GlobalData.h"

int MyPackage::max = 5;
std::vector<PackageData> MyPackage::vec_packages;

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
		int index = 0;
		for (unsigned int i = 0; i < vec_packages.size(); i++)
		{
			if (pdata.strid.compare(vec_packages[i].strid) == 0 && (pdata.type == FOOD || pdata.type == MEDICINAL || pdata.type == RES_1 || pdata.type == RES_2))
			{
				if (vec_packages[i].count < 10)
				{
					vec_packages[i].count++;
					save();
					return 0;
				}
				else
				{
					index++;
				}
			}
			else
			{
				index++;
			}
		}

		if (index == vec_packages.size())
			vec_packages.push_back(pdata);
		save();
	}
	return 0;
}

void MyPackage::cutone(std::string strid, int count)
{
	int index = 0;
	for (index = MyPackage::getSize() - 1; index >= 0; index--)
	{
		if (strid.compare(MyPackage::vec_packages[index].strid) == 0)
		{
			vec_packages[index].count -= count;
			if (vec_packages[index].count <= 0)
			{
				vec_packages.erase(vec_packages.begin() + index);
			}
			break;
		}
	}

	save();
}


void MyPackage::cutone(PackageData pdata)
{
	int index = 0;
	for (index = MyPackage::getSize() - 1; index >= 0; index--)
	{
		if (pdata.strid.compare(MyPackage::vec_packages[index].strid) == 0 && pdata.goodvalue == MyPackage::vec_packages[index].goodvalue)
		{
			vec_packages[index].count -= pdata.count;
			if (vec_packages[index].count <= 0)
			{
				vec_packages.erase(vec_packages.begin() + index);
			}
			break;
		}
	}

	save();
}

void MyPackage::takeoff()
{
	vec_packages.clear();
	save();
}

bool MyPackage::isFull(PackageData pdata)
{
	int index = 0;
	bool ret = false;
	if (vec_packages.size() == getMax())
	{
		for (unsigned int i = 0; i < vec_packages.size(); i++)
		{
			if (vec_packages[i].strid.compare(pdata.strid) != 0)
				index++;
			else
			{
				if (vec_packages[i].count >= 10)
					ret = true;
				else
				{
					if (pdata.type == FOOD || pdata.type == MEDICINAL || pdata.type == RES_1 || pdata.type == RES_2)
						ret = false;
					else
						ret = true;
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
	std::string str;
	for (unsigned int i = 0; i < vec_packages.size(); i++)
	{
		//"%s-%d-%d-%d-%d;", sdata.strid.c_str(), sdata.type, sdata.count, sdata.extype, sdata.lv
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d;", vec_packages[i].strid.c_str(), vec_packages[i].type, vec_packages[i].count, vec_packages[i].extype, vec_packages[i].lv, vec_packages[i].exp, vec_packages[i].goodvalue);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setPackage(str.substr(0, str.length() - 1));
}

void MyPackage::load()
{
	vec_packages.clear();
	std::string datastr = GameDataSave::getInstance()->getPackage();
	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;

		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		vec_packages.push_back(data);
	}
}

int MyPackage::getMax()
{
	return max;
}

void MyPackage::setMax(int val)
{
	max = val;
}

int MyPackage::getSize()
{
	return vec_packages.size();
}