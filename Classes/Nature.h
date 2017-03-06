#ifndef _NATURE_H_
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
static std::string reasondesc1[] = { "春春春春", "夏夏夏夏", "秋秋秋秋", "冬冬冬冬冬冬冬冬冬冬冬冬" };
static std::string reasondesc[] = { "春春春春春春春春", "夏夏夏夏夏夏", "秋秋秋秋秋秋秋秋秋秋", "冬冬冬冬冬冬" };

static std::string weathername[] = {"晴天", "阴天", "下雨", "下雪"};
static std::string weatherdesc1[] = { "晴天晴天晴天", "阴天阴天阴天", "下雨下雨下雨", "下雪下雪下雪" };
static std::string weatherdesc[] = { "晴天晴天晴天晴天", "阴天阴天阴天阴天阴天", "下雨下雨下雨下雨下雨", "下雪下雪下雪下雪下雪" };

static std::string liveDayDesc = {"记录生活时间，以便制定更合理的计划"};
static std::string timeDesc = { "06:00-18:00白天 18:00-06:00黑夜" };

#endif

