#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_
#include "cocos2d.h"
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
	float waittime;

}ResData;

typedef struct
{
	char strid[10];
	std::vector<std::string> npcs;
	char cname[32];
	char tpngname[10];
	std::string desc;
}MapData;

typedef struct
{
	char id[10];
	char name[32];
	int type;
	int life;
	int atk;
	int exp;
	int df;
	std::vector<std::string> winres;
	std::vector<int> winresrnd;
	std::vector<std::string> exchgres;
	std::vector<std::vector<std::string>> exchgneedres;
}NpcData;

class GlobalData
{
public:
	GlobalData();
	~GlobalData();

public:
	static void loadResJsonData();
	static void saveResData();
	static void loadResData();
	static void loadHillResJsonData();
	static void loadMapJsonData();
	static void loadNpcJsonData();
public:
	static std::vector<ResData> vec_resData;
	static std::vector<int> vec_hillResid;
	static std::map<std::string, MapData> map_maps;
	static std::map<std::string, NpcData> map_npcs;
};
#endif

