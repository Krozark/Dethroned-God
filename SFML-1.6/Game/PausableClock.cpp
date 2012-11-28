#include "PausableClock.h"

PausableClock::PausableClock()
{
    Stop();
}

void PausableClock::Pause()
{
	if(!m_pause)
	{
		m_pause=true;
		m_elapsedTime+=sf::Clock::GetElapsedTime();
	}
}

void PausableClock::Play()
{
	if(m_pause)
	{
		m_pause=false;
		sf::Clock::Reset();
	}
}

bool PausableClock::Toggle()
{
	if(m_pause)
	{
		Play();
		return true;
	}
	Pause();
	return false;
}

void PausableClock::Stop()
{
    sf::Clock::Reset();
	m_pause=true;
	m_elapsedTime=0;
}

void PausableClock::Reset()
{
	m_elapsedTime=0;
	sf::Clock::Reset();
	m_pause=false;
}

float PausableClock::GetElapsedTime()
{
	if(m_pause)
		return m_elapsedTime;
	return sf::Clock::GetElapsedTime() + m_elapsedTime;
}


