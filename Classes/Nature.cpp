#include "Nature.h"

int Nature::ReasonCDays = 90;

int WheatherWeight[][4] = { { 40, 62, 100, 100 }, { 40, 62, 100, 100 }, { 40, 62, 100, 100 }, { 20, 40, 60, 100 } };
int tempeRange[][2] = { { 10, 20 }, { 15, 25 }, { 5, 15 }, {-20, 0} };
Nature::Nature()
{

	m_time = 0;
	m_reason = EReason::Spring;
	m_wheather = EWheather::Suny;
	m_daynight = EDAYNIGTH::Night;
	srand(systime());
	int r = tempeRange[m_reason][0] + rand() % (tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1);
	m_temperature = r;
}


Nature::~Nature()
{

}

int Nature::systime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}
void Nature::ChangeWheather()
{
	srand(systime());
	int r = rand() % 100;
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		if (r < WheatherWeight[m_reason][i])
		{
			break;
		}
	}
	EWheather w = (EWheather)i;
	if (w != m_wheather)
	{
		int c = w - m_wheather;
		setWheather(w);
		setTemperature(m_temperature - c * 5);
	}


}

void Nature::ChangeReason(int livedays)
{
	int yudays = livedays % (4 * ReasonCDays);
	EReason r = Spring;
	if (yudays < ReasonCDays)
	{
		r = Spring;
	}
	else if (yudays < ReasonCDays * 2)
	{
		r = Summer;
	}
	else if (yudays < ReasonCDays * 3)
	{
		r = Autumn;
	}
	else
	{
		r = Winter;
	}
	if (r != m_reason)
	{
		setReason(r);
		srand(systime());
		int  t = tempeRange[m_reason][0] + rand() % (tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1);
		setTemperature(t);
	}

}
void Nature::ChangeDayNight(int time)
{
	if (time < 60 * 6 - 1 || time >= 18 * 60)
	{
		if (m_daynight == Day)
		{
			setDayOrNight(Night);
			setTemperature(m_temperature - 5);
		}
		
	}
	else
	{
		if (m_daynight == Night)
		{
			setDayOrNight(Day);
			setTemperature(m_temperature + 5);
		}
	}
}