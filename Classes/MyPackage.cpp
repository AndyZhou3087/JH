#include "MyPackage.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"

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
			if (pdata.strid.compare(vec_packages[i].strid) == 0)
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

void MyPackage::cutone(std::string strid)
{
	int index = 0;
	for (index = 0; index < MyPackage::getSize(); index++)
	{
		if (strid.compare(MyPackage::vec_packages[index].strid) == 0)
		{
			break;
		}
	}
	if (--vec_packages[index].count <= 0)
	{
		vec_packages.erase(vec_packages.begin() + index);
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
					ret = false;
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
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%s-%s;", vec_packages[i].strid.c_str(), vec_packages[i].type, vec_packages[i].count, vec_packages[i].extype, vec_packages[i].lv, vec_packages[i].exp, vec_packages[i].goodvalue, vec_packages[i].name.c_str(), vec_packages[i].desc.c_str());
		str.append(onestr);
	}
	GameDataSave::getInstance()->setPackage(str.substr(0, str.length() - 1));
}

void MyPackage::load()
{
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
		data.name = tmp[7];
		data.desc = tmp[8];
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