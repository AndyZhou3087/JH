#include "MyPackage.h"
#include "Const.h"
#include "CommonFuncs.h"

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

	}
}

void MyPackage::takeoff()
{

}

bool MyPackage::isFull(PackageData pdata)
{

}

void MyPackage::save()
{

}

void MyPackage::load()
{

}

int MyPackage::getMax()
{

}

void MyPackage::setMax(int val)
{

}