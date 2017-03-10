/********************************************************************
* 背包
*********************************************************************/

#ifndef _MYPACKAGE_H_
#define _MYPACKAGE_H_
#include "cocos2d.h"
USING_NS_CC;

typedef struct
{
	int id;
	int type;
	int count;

}PackageData;


class MyPackage
{
public:
	MyPackage();
	~MyPackage();

public:

	static int add(PackageData pdata);
	static void takeoff();
	static bool isFull(PackageData pdata);
	static void save();
	static void load();

	static int getMax();
	static void setMax(int val);

private:
	static std::vector<PackageData> vec_packages;
};
#endif

