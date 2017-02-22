#include "Nature.h"

int Nature::ReasonCDays = 90;

int WheatherWeight[][4] = { { 40, 62, 100, 100 }, { 40, 62, 100, 100 }, { 40, 62, 100, 100 }, { 20, 40, 60, 100 } };
Nature::Nature()
{
	m_temperature = 10;
	m_time = 0;
	m_reason = EReason::Spring;
	m_wheather = EWheather::Suny;
	m_daynight = EDAYNIGTH::Night;
}


Nature::~Nature()
{
}

void Nature::ChangeWheather()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	srand(timep);
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
		setReason(r);

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