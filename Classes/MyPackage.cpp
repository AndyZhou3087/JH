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
			if (pdata.id == vec_packages[i].id)
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

void MyPackage::cutone(int index)
{
	vec_packages[index].count;
	if (--vec_packages[index].count <= 0)
	{
		vec_packages.erase(vec_packages.begin()+index);
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
			if (vec_packages[i].id != pdata.id)
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
		std::string onestr = StringUtils::format("%d-%d;", vec_packages[i].id * 1000 + vec_packages[i].type, vec_packages[i].count);
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
		int idtype = atoi(tmp[0].c_str());
		PackageData data;
		data.id = idtype / 1000;
		data.type = idtype % 1000;
		data.count = atoi(tmp[1].c_str());
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