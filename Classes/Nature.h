﻿#ifndef _NATURE_H_
#define _NATURE_H_
#include "cocos2d.h"
USING_NS_CC;
#define NORMAL_TIMEINTERVAL 1.0f
typedef enum
{
	Spring = 0,
	Summer,
	Autumn,
	Winter
}EReason;

typedef enum
{
	Suny = 0,
	Cloudy,
	Rainy,
	Snowy
}EWeather;
typedef enum
{
	Day = 0,
	Night
}EDAYNIGTH;

class Nature:public Node
{
public:
	Nature();
	~Nature();
	CC_SYNTHESIZE(int, m_temperature, Temperature);
	CC_SYNTHESIZE(float, m_time, Time);//虚拟一天时间，0.2s--对应1min
	CC_SYNTHESIZE(EReason, m_reason, Reason);
	CC_SYNTHESIZE(EWeather, m_weather, Weather);
	CC_SYNTHESIZE(EDAYNIGTH, m_daynight, DayOrNight);
	CC_SYNTHESIZE(int, m_pastdays, PastDays);
	CC_SYNTHESIZE(float, m_timeinterval, TimeInterval);

	void ChangeWeather();
	void ChangeReason();
	void ChangeDayNight();

	virtual bool init();

	CREATE_FUNC(Nature);

public:
	int static ReasonCDays;//季节切换天数
private:
	int static systime();
	void updateData(float dt);
};

static std::string reasonname[] = { "春", "夏", "秋", "冬" };
static std::string reasondesc1[] = { "春暖花开", "骄阳似火", "金风玉露", "银装素裹" };
static std::string reasondesc[] = { "万物复苏，野外的动物逐渐变多。", "雨过天晴，各种矿类暴露在野外。", "果实累累，山中水果进入丰收期。", "林寒洞肃，树苗已长成参天大树。" };

static std::string weathername[] = {"晴", "阴", "雨", "雪"};
static std::string weatherdesc1[] = { "风和日丽", "阴云密布", "狂风暴雨", "大雪纷飞" };
static std::string weatherdesc[] = { "风和日暖，温度适宜。", "阴云密布，风高气爽。", "疾风骤雨，温度下降。", "弥天大雪，温度骤降。" };

static std::string liveDayDesc = {"记录闯荡江湖的时间，以便制定合理的江湖计划。"};
static std::string timeDesc = { "白天时间：6：00~18:00，黑夜时间：18：00~6：00。黑夜有一定几率触发特殊事件。" };
static std::string tempeDesc = { "温度过高时会造成外伤发炎，温度过低时会加速生命值消耗。" };
#endif

