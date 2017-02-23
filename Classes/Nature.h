#ifndef _NATURE_H_
#define _NATURE_H_
#include "cocos2d.h"
USING_NS_CC;
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
}EWheather;
typedef enum
{
	Day = 0,
	Night
}EDAYNIGTH;

class Nature
{
public:
	Nature();
	~Nature();
	CC_SYNTHESIZE(int, m_temperature, Temperature);
	CC_SYNTHESIZE(int, m_time, Time);//虚拟一天时间，0.2s--对应1min
	CC_SYNTHESIZE(EReason, m_reason, Reason);
	CC_SYNTHESIZE(EWheather, m_wheather, Wheather);
	CC_SYNTHESIZE(EDAYNIGTH, m_daynight, DayOrNight);

	void ChangeWheather();
	void ChangeReason(int livedays);
	void ChangeDayNight(int time);
public:
	int static ReasonCDays;//季节切换天数
private:
	int static systime();
};
#endif

