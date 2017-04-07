/********************************************************************
* 背包
*********************************************************************/

#ifndef _MYPACKAGE_H_
#define _MYPACKAGE_H_
#include "cocos2d.h"
USING_NS_CC;

typedef struct
{
	std::string strid;
	int type;
	int count;
	int lv;//内功，外功等级，
	int extype;//用于区分采集1，砍伐2，挖掘工具3，其他类型为0
	int exp;
	int goodvalue;

}PackageData;

class MyPackage
{
public:
	MyPackage();
	~MyPackage();

public:

	static int add(PackageData pdata);
	static void cutone(int index);
	static void takeoff();
	static bool isFull(PackageData pdata);
	static void save();
	static void load();

	static int getMax();
	static void setMax(int val);
	static int getSize();
public:
	static std::vector<PackageData> vec_packages;
private:
	
	static int max;
};
#endif

