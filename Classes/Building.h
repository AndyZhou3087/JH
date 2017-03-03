#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "cocos2d.h"
#include "Goods.h"
#include "json.h"
USING_NS_CC;
typedef struct
{
	char name[20];
	int maxlevel;
	int level;
	std::vector<int> needtime;
	std::vector<std::vector<int>> Res;
	char cname[32];
}BuidingData;
class Building :public Goods
{
public:
	Building();
	~Building();
	void parseData(rapidjson::Value& jsonvalue);

	virtual void build();
	virtual void action(float acsec, int exminute);
public:
	BuidingData data;
protected:
	int goodvaule;
};
#endif

