/****************************************************************
全局操作类

****************************************************************/
#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_
#include "cocos2d.h"
USING_NS_CC;

/****************************
建筑物建筑，升级，操作 NODE数据结构
*****************************/
typedef struct
{
	char icon[20];//id
	std::vector<int> res;//需要资源
	std::vector<int> ep;//使用后的效果，eg:加生命，加精神
	int blv;//建筑物等级
	char actext[32];//NODE上按钮的文件
	int actime;//操作时间
	int extime;//影响游戏时间
	int type;//类型StorageType
	int extype;//工具时的标识1："采集", 2："砍伐", 3："挖掘"
	std::string cname;//中文显示
	std::string desc;//描述
}BuildActionData;


/****************************
所有资源数据结构
*****************************/
typedef struct
{
	std::string strid;//id
	std::string cname;//中文显示名
	std::string desc;//描述
	int val;//资源价值
	std::vector<std::string> npc;//加成的npc;
	std::vector<float> npcval;//加成
}AllResource;

/****************************
一级资源数据结构
*****************************/
typedef struct
{
	std::string strid;//id
	int count;//产出剩下的个数
	int max;//产出最大个数
	std::vector<float> speed;//产出速度
	int type;//类型StorageType
	int actype;////工具时的标识1："采集", 2："砍伐", 3："挖掘"
	std::vector<int> ep; // 使用后的效果，eg:加生命，加精神
	std::vector<int> res;//合成需要的资源
	float pastmin;//过去的时间
	float waittime;//单个产出等待时间
	std::string unitname;//资源的单位 eg:一根木材

}ResData;

/****************************
地图地点数据结构
*****************************/
typedef struct
{
	char strid[10];//id
	std::vector<std::string> npcs;//npc id
	char cname[32];//地点显示名字
	char tpngname[10];//地点类型
	std::string desc;//地点描述
}MapData;

/****************************
NPC数据结构
*****************************/
typedef struct
{
	char id[10];
	char name[32];
	int type;//保留字段
	int life;//生命值
	int atk;//攻击
	int exp;//经验值
	int df;//防御
	int lv;//npc等级
	std::vector<std::string> winres;//打败npc赢得奖励
	std::vector<int> winresrnd;//打败npc出奖励的概率
	std::vector<int> winrescount;//打多少次随机
	std::vector<std::string> exchgres;//兑换的资源
	std::vector<std::string> words;//npc 对话
}NpcData;

/****************************
角色数据结构
*****************************/
typedef struct
{
	int id;
	char name[32];
	std::vector<int> vec_atk;
	std::vector<int> vec_df;
	std::vector<int> vec_exp;
	std::vector<int> vec_maxhp;
}HeroAtrData;

/****************************
功法数据结构
*****************************/
typedef struct
{
	char id[32];
	int maxlv;
	std::vector<int> vec_bns;//外功--加攻或者内功--加防
	std::vector<int> vec_exp;
	int type;//类型StorageType
	int qu;//品级
	int extype;//1：棍 2：刀 3：剑
}WG_NGData;

/****************************
武器防具数据结构
*****************************/
typedef struct
{
	std::string id;
	int type;//类型StorageType
	int atk;
	int df;
	int extype;//扩展类型，区分棍，刀，剑
	int qu;//品级
}EquipData;

/****************************
所有资源类型
*****************************/
typedef enum
{
	FOOD = 0,//食物
	MEDICINAL,//药材
	WEAPON,//武器
	PROTECT_EQU,//防具
	N_GONG,//内功
	W_GONG,//外功
	RES_1,//一级资源
	TOOLS,//工具
	RES_2,//其他
	RES_MAX

}StorageType;

/****************************
剧情状态
*****************************/
typedef enum
{
	M_NONE = 0,//未开始
	M_DOING,//正在进行中
	M_DONE//完成
}MissionStatus;

/****************************
剧情数据
*****************************/
typedef struct
{
	std::string id;//ID
	std::string snpc;//发剧情的NPCID
	std::string dnpc;//需要挑战的NPCID
	std::vector<std::string> words;//发剧情NPC对话
	std::vector<std::string> mywords;//角色对话
	std::vector<std::string> bossword;//找到挑战NPC对话
	std::vector<std::string> rewords;//完成后的奖励
	int unlockchapter;//完成后解锁的章节
	int status;//状态
	int type;//对话0；需要战斗1
	bool isshowsnpc;
	bool isshowdnpc;
}PlotMissionData;

/****************************
招式招数数据
*****************************/
typedef struct
{
	std::string id;//ID
	std::vector<std::string> snames;//招式名称
}GFSkillData;

class GlobalData
{
public:
	GlobalData();
	~GlobalData();

public:
	/****************************
	解析建筑物数据
	@param 建筑物名称
	*****************************/
	static void loadBuildActionJSon(std::string buildname);

	/****************************
	解析所有资源详细描述数据
	*****************************/
	static void loadAllResourceJsonData();

	/****************************
	解析一级资源数据
	*****************************/
	static void loadResJsonData();

	/****************************
	保存一级资源数据
	*****************************/
	static void saveResData();

	/****************************
	读取保存的一级资源数据
	*****************************/
	static void loadResData();

	/****************************
	解析后山资源数据
	*****************************/
	static void loadHillResJsonData();

	/****************************
	解析地图数据
	*****************************/
	static void loadMapJsonData();

	/****************************
	解析NPC数据
	*****************************/
	static void loadNpcJsonData();

	/****************************
	解析角色数据
	*****************************/
	static void loadHeroAtrJsonData();

	/****************************
	解析功法数据
	*****************************/
	static void loadWG_NGJsonData();

	/****************************
	解析武器防具数据
	*****************************/
	static void loadEquipJsonData();

	/****************************
	读取保存的解锁角色数据
	*****************************/
	static void loadUnlockHeroData();

	/****************************
	保存解锁角色数据
	*****************************/
	static void setUnlockHero(int index, bool val);

	/****************************
	获取角色是否解锁
	*****************************/
	static bool getUnlocHero(int index);

	/****************************
	系统时间，秒数
	*****************************/
	static int getSysSecTime();

	/****************************
	一年中的天数
	@return 一年中的天数(0-365)
	*****************************/
	static int getDayOfYear();

	/****************************
	获取userid
	*****************************/
	static std::string getUId();

	/****************************
	设置userid
	*****************************/
	static void setUId(std::string struid);

	/****************************
	获取默认仓储数据
	@param heroindex 角色ID
	*****************************/
	static std::string getDefaultStorage(int heroindex);

	/****************************
	解析原始默认仓储数据
	@param heroindex 角色ID
	*****************************/
	static std::string getOriginLocalStorage(int heroindex);

	/****************************
	设置剧情ID
	@param 剧情ID
	*****************************/
	static void setPlotMissionIndex(int val);

	/****************************
	获取剧情ID
	*****************************/
	static int getPlotMissionIndex();

	/****************************
	解析剧情ID
	*****************************/
	static void loadPlotMissionJsonData();

	/****************************
	保存剧情状态
	*****************************/
	static void savePlotMissionStatus();

	/****************************
	更新剧情状态
	*****************************/
	static void updatePlotMissionStatus();

	/****************************
	设置支线剧情ID
	@param 剧情ID
	*****************************/
	static void setBranchPlotMissionIndex(int val);

	/****************************
	获取支线剧情ID
	*****************************/
	static int getBranchPlotMissionIndex();

	/****************************
	解析支线剧情ID
	*****************************/
	static void loadBranchPlotMissionJsonData();

	/****************************
	保存支线剧情状态
	*****************************/
	static void saveBranchPlotMissionStatus();

	/****************************
	更新支线剧情状态
	*****************************/
	static void updateBranchPlotMissionStatus();

	/****************************
	获取解锁的章节
	*****************************/
	static int getUnlockChapter();

	/****************************
	设置解锁的章节
	@param 解锁的章节
	*****************************/
	static void setUnlockChapter(int val);

	/****************************
	产生一个随机数
	@param 随机数范围，从0开始
	*****************************/
	static int createRandomNum(int val);

	/****************************
	获取存档的ID
	*****************************/
	static std::vector<std::string> getSaveListId();

	/****************************
	保存存档的ID
	@param 保存的id vector
	*****************************/
	static void setSaveListId(std::vector<std::string> vec_val);

	/****************************
	保存当前的角色ID到进度列表
	*****************************/
	static void setCurHeroIdToSaveList();

	/****************************
	是否在闭关中
	*****************************/
	static bool isExercising();

	/****************************
	是否有分身符
	*****************************/
	static bool isHasFSF();

	/****************************
	解析招式数据
	*****************************/
	static void loadGfskillData();

	/****************************
	临时存放地是否有功法
	@return false:没有，yes：有
	*****************************/
	static bool tempHasgf(std::string strid);

	/****************************
	获取分享的日期，只记录天数（一年中的天数）
	return 天数（一年中的天数）
	*****************************/
	static int getShareDay();

	/****************************
	设置分享的日期
	@para 天数（一年中的天数）
	*****************************/
	static void setShareDay(int day);

	/****************************
	获取制暖的参数(开始制暖游戏分钟数，制暖时间)
	*****************************/
	static std::string getMakeWarmConfig();

	/****************************
	设置制暖的参数(开始制暖游戏分钟数，制暖时间)
	*****************************/
	static void setMakeWarmConfig(std::string strval);

    static void setNoAds(bool val);
    static bool getNoAds();
    
    static void setHasBuy(bool val);
    static bool getHasBuy();
    
    static void setDailyAdsCount(int val);
    static int getDailyAdsCount();
    
    static void setDailyAdsDay(int val);
    static int getDailyAdsDay();
    static void setNoComments(bool val);
    static bool getNoComments();
    
    static void setAdsInterval(int val);
    static int getAdsInterval();
    static void setAdsChoose(long val);
    static int getAdsChoose(int position);

public:
	static std::map<std::string, AllResource> map_allResource;//所有资源，主要是描述和交易数据
	static std::vector<ResData> vec_resData;//一级资源vector
	static std::vector<std::string> vec_hillResid;//后山资源列表vector
	static std::map<std::string, MapData> map_maps;//地图数据
	static std::map<std::string, NpcData> map_npcs;//npc 数据
	static std::map<int, HeroAtrData> map_heroAtr;//角色属性装备栏数据
	static std::map<std::string, WG_NGData> map_wgngs;//功法数据
	static std::map<std::string, EquipData> map_equips;//武器防具数据
	static std::map<std::string, std::vector<BuildActionData>> map_buidACData;//建筑物数据
	static std::vector<PlotMissionData> vec_PlotMissionData;//剧情数据
	static std::vector<PlotMissionData> vec_BranchPlotMissionData;//支线剧情数据
	static std::map<std::string, GFSkillData> map_gfskills;//招式数据
private:
	static bool unlockhero[4];//角色解锁
	static std::string uid;//

	static std::vector<std::string> vec_saveids;//存档的id
    
    static bool hasBuy;
    static int adsinterval;
    static long adschoose;
    static std::string addUidString(std::string val);
};
#endif

