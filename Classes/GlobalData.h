#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_
#include "cocos2d.h"
USING_NS_CC;


typedef struct
{
	std::string strid;
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
	std::string unitname;

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

typedef struct
{
	int id;
	char name[32];
	std::vector<int> vec_atk;
	std::vector<int> vec_df;
	std::vector<int> vec_exp;
	std::vector<int> vec_maxhp;
}HeroAtrData;

typedef struct
{
	char id[32];
	int maxlv;
	std::vector<int> vec_bns;
	std::vector<int> vec_exp;
	int lv;
	std::string cname;
	std::string desc;
}WG_NGData;

typedef struct
{
	std::string id;
	int type;
	int atk;
	int df;
	std::string cname;
	std::string desc;
}EquipData;


typedef enum
{
	FOOD = 0,
	MEDICINAL,
	WEAPON,
	PROTECT_EQU,
	N_GONG,//内功
	W_GONG,//外功
	RES_1,
	TOOLS,
	RES_MAX

}StorageType;

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
	static void loadHeroAtrJsonData();
	static void loadWG_NGJsonData();
	static void loadEquipJsonData();

public:
	static std::vector<ResData> vec_resData;
	static std::vector<std::string> vec_hillResid;
	static std::map<std::string, MapData> map_maps;
	static std::map<std::string, NpcData> map_npcs;
	static std::map<int, HeroAtrData> map_heroAtr;
	static std::map<std::string, WG_NGData> map_wgngs;
	static std::map<std::string, EquipData> map_equips;
};
#endif

