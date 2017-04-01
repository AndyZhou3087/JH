﻿#ifndef _GAMEDATA_SAVE_H_
#define _GAMEDATA_SAVE_H_
#include "cocos2d.h"
class GameDataSave
{
public:
	static GameDataSave* getInstance();
	static void purgeGameSave();
	int getLiveDays();
	void setLiveDays(int val);

	int getNatureTime();
	void setNatureTime(int val);

	int getNatureWeather();
	void setNatureWeather(int val);

	int getNatureReason();
	void setNatureReason(int val);

	int getNatureTemperature();
	void setNatureTemperature(int val);

	int getHeroOutinjury();
	void setHeroOutinjury(int val);

	int getHeroInnerinjury();
	void setHeroInnerinjury(int val);

	int getHeroHunger();
	void setHeroHunger(int val);

	int getHeroSpirit();
	void setHeroSpirit(int val);

	int getHeroLife();
	void setHeroLife(int val);

	std::string getStorageData();
	void setStorageData(std::string valstr);

	int getBuildLV(std::string buildname);
	void setBuildLV(std::string buildname, int val);

	std::string getHeroAddr();
	void setHeroAddr(std::string val);

	std::string getTempStorage(std::string addrname);
	void setTempStorage(std::string addrname, std::string vstr);

	std::string getPackage();
	void setPackage(std::string vstr);

	std::string getResData();
	void setResData(std::string vstr);

	int getHeroId();
	void setHeroId(int id);

	int getHeroLV();
	void setHeroLV(int lv);

private:
	static GameDataSave* _Context;

};
#endif

