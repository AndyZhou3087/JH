﻿
/********************************************************************
*数据存储操作，保存到本地
*********************************************************************/
#ifndef _GAMEDATA_SAVE_H_
#define _GAMEDATA_SAVE_H_
#include "cocos2d.h"
class GameDataSave
{
public:
	/****************************
	类实例，单例模式
	****************************/
	static GameDataSave* getInstance();

	/****************************
	释放
	****************************/
	static void purgeGameSave();

	/****************************
	获取保存的生存天数
	****************************/
	int getLiveDays();

	/****************************
	保存生存天数
	****************************/
	void setLiveDays(int val);

	/****************************
	获取保存的每天时间，24小时制
	****************************/
	int getNatureTime();

	/****************************
	保存每天时间，24小时制
	****************************/
	void setNatureTime(int val);

	/****************************
	获取保存的天气
	****************************/
	int getNatureWeather();

	/****************************
	保存天气
	****************************/
	void setNatureWeather(int val);

	/****************************
	获取保存的天气改变次数
	****************************/
	int getNatureWeatherChangeCount();

	/****************************
	保存天气改变次数
	****************************/
	void setNatureWeatherChangeCount(int val);

	/****************************
	获取保存的季节
	****************************/
	int getNatureReason();

	/****************************
	保存季节
	****************************/
	void setNatureReason(int val);

	/****************************
	获取保存的温度
	****************************/
	int getNatureTemperature();

	/****************************
	保存温度
	****************************/
	void setNatureTemperature(int val);

	/****************************
	获取保存的角色外伤值
	****************************/
	int getHeroOutinjury();

	/****************************
	保存角色外伤值
	****************************/
	void setHeroOutinjury(int val);

	/****************************
	获取保存的角色内伤值
	****************************/
	int getHeroInnerinjury();

	/****************************
	保存角色内伤值
	****************************/
	void setHeroInnerinjury(int val);

	/****************************
	获取保存的角色饱食度值
	****************************/
	int getHeroHunger();

	/****************************
	保存角色饱食度值
	****************************/
	void setHeroHunger(int val);

	/****************************
	获取保存的角色精神值
	****************************/
	float getHeroSpirit();

	/****************************
	保存角色精神值
	****************************/
	void setHeroSpirit(float val);

	/****************************
	获取保存的角色生命值，生命为0，游戏结束
	****************************/
	int getHeroLife();

	/****************************
	保存角色生命值
	****************************/
	void setHeroLife(int val);

	/****************************
	获取保存的仓库数据
	****************************/
	std::string getStorageData();

	/****************************
	保存仓库数据
	****************************/
	void setStorageData(std::string valstr);

	/****************************
	获取建筑物等级
	@param buildname 建筑物名称
	@return 建筑物等级
	****************************/
	int getBuildLV(std::string buildname);

	/****************************
	保存建筑物等级
	@param buildname 建筑物名称
	@param 建筑物等级
	****************************/
	void setBuildLV(std::string buildname, int val);

	/****************************
	获取角色进入的地点
	@return 地点
	****************************/
	std::string getHeroAddr();

	/****************************
	保存角色进入的地点
	@param 地点
	****************************/
	void setHeroAddr(std::string val);

	/****************************
	获取临时地点存放的数据
	@param 地点
	@return 临时地点存放的数据
	****************************/
	std::string getTempStorage(std::string addrname);

	/****************************
	保存临时地点存放的数据
	@param 地点
	@param 临时地点存放的数据
	****************************/
	void setTempStorage(std::string addrname, std::string vstr);

	/****************************
	获取背包数据
	****************************/
	std::string getPackage();

	/****************************
	保存背包数据
	****************************/
	void setPackage(std::string vstr);

	/****************************
	获取后山资源列表数据
	****************************/
	std::string getResData();

	/****************************
	保存后山资源列表数据
	****************************/
	void setResData(std::string vstr);

	/****************************
	获取角色ID,目前只有4个
	****************************/
	int getHeroId();

	/****************************
	保存角色ID
	****************************/
	void setHeroId(int id);

	/****************************
	获取角色等级
	****************************/
	int getHeroLV();

	/****************************
	保存角色等级
	****************************/
	void setHeroLV(int lv);

	/****************************
	获取角色经验值
	****************************/
	int getHeroExp();

	/****************************
	保存角色经验值
	****************************/
	void setHeroExp(int val);

	/****************************
	获取角色是否在外面 （家以外）
	****************************/
	bool getHeroIsOut();

	/****************************
	保存角色是否在外面 （家以外）
	****************************/
	void setHeroIsOut(bool val);

	/****************************
	获取角色属性数据 （装备栏）
	****************************/
	std::string getHeroProperData();

	/****************************
	保存角色属性数据 （装备栏）
	****************************/
	void setHeroProperData(std::string strval);

	/****************************
	获取角色是否解锁 4个角色哪几个解锁
	****************************/
	std::string getHeroUnlockData();

	/****************************
	保持角色是否解锁 4个角色哪几个解锁
	****************************/
	void setHeroUnlockData(std::string strval);

	/****************************
	获取用户ID,单机模式“新游戏”就会生成一个新的userid
	****************************/
	std::string getUserId();

	/****************************
	保存用户ID,单机模式“新游戏”就会生成一个新的userid
	****************************/
	void setUserId(std::string val);

	/****************************
	获取剧情完成到哪一个
	****************************/
	void setPlotMissionIndex(int val);

	/****************************
	保存剧情完成到哪一个
	****************************/
	int getPlotMissionIndex();

	/****************************
	获取剧情完成状态
	****************************/
	void setPlotMissionStatus(std::string strval);

	/****************************
	保存剧情完成状态
	****************************/
	std::string getPlotMissionStatus();

	/****************************
	保存剧情完成后解锁章节
	@param val 第几章
	****************************/
	void setPlotUnlockChapter(int val);

	/****************************
	获取解锁章节
	****************************/
	int getPlotUnlockChapter();

	/****************************
	获取存档ID
	****************************/
	std::string getSaveListId();

	/****************************
	保存存档ID
	****************************/
	void setSaveListId(std::string val);

	/****************************
	获取是否有新手引导
	****************************/
	bool getIsNewerGuide(int index);

	/****************************
	设置是否有新手引导
	****************************/
	void setIsNewerGuide(int index, bool val);

private:
	/****************************
	加上userid后的字符串
	****************************/
	std::string addUIDString(std::string val);
private:
	static GameDataSave* _Context;//类实例
	static std::string userid;//uid

};
#endif

