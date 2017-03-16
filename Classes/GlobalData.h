#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_
#include "cocos2d.h"
#include "MyPackage.h"
USING_NS_CC;


typedef struct
{
	int id;
	int count;
	int max;
	int speed;
	int type;
	int actype;
	std::vector<int> ep;
	char cname[32];
	char desc[200];
	std::vector<int> res;
	int pastmin;

}ResData;
class GlobalData
{
public:
	GlobalData();
	~GlobalData();

public:
	static void init();

public:
	static std::vector<ResData> vec_resData;
	static std::vector<PackageData> vec_getResData;
};
#endif

