#include "ReverseClock.h"

using namespace sf;

/*
* Cr�e une horloge ;
* Si play = true, d�marre l'horloge,
* Si play = false, met l'horloge en pause.
*/
ReverseClock::ReverseClock(float t, bool play)
{
    SetTime(t, play);
}

/*
* Cr�e une horloge sans la d�marrer.
*/
ReverseClock::ReverseClock() : totalTime(0.f)
{
    Stop();
}

/*
* Retourne le temps restant avant la fin d'une horloge (hors pause),
* Retourne -1 si celle-ci est d�j� arr�t�e.
*/
float ReverseClock::GetRemainingTime()
{
    if (m_pause)
    {
        return remainingTime;
    }
    else
    {
        if (finalTime >= Clock::GetElapsedTime())
        {
            return finalTime - Clock::GetElapsedTime();
        }
        else
        {
            return -1;
        }
    }
}

/*
* Retourne le temps �coul� depuis le d�marrage d'une horloge (hors pause).
*/
float ReverseClock::GetElapsedTime()
{
    if (GetRemainingTime() == -1)
    {
        return totalTime;
    }
    else
    {
        return totalTime - GetRemainingTime();
    }
}

/*
* Red�marre une horloge en pause si elle n'est pas d�j� arr�t�e.
*/
void ReverseClock::Play()
{
    if (!IsPlaying())
    {
        finalTime = remainingTime + Clock::GetElapsedTime();
        m_pause = false;
    }
}

/*
* Red�marre une horloge avec le dernier temps avec laquelle elle a �t� d�marr�e.
*/
void ReverseClock::Replay()
{
    SetTime(totalTime);
}

/*
* Met une horloge en pause.
*/
void ReverseClock::Pause()
{
    if (!m_pause)
    {
        if (finalTime >= Clock::GetElapsedTime())
        {
            remainingTime = finalTime - Clock::GetElapsedTime();
        }
        else
        {
            remainingTime = -1;
        }
        m_pause = true;
    }
}

/*
* Arr�te une horloge.
*/
void ReverseClock::Stop()
{
	remainingTime = -1;
	m_pause = true;
}

/*
* D�finit un nouveau temps pour l'horloge,
* - La d�marre si play = true
* - La met en pause sinon.
*/
void ReverseClock::SetTime(float t, bool play)
{
    finalTime = t + Clock::GetElapsedTime();
    remainingTime = t;
    totalTime = t;
    m_pause = play;
}

/*
* Retourne :
* - false si l'horloge est en pause,
* - true sinon.
*/
bool ReverseClock::IsPlaying()
{
    return !IsPaused();
}

/*
* Retourne :
* - true si l'horloge est en pause,
* - false sinon.
*/
bool ReverseClock::IsPaused()
{
    return (GetRemainingTime() == -1  or  m_pause);
}

/*
* Red�marre une horloge si elle est en pause mais n'est pas arr�t�e,
* Met cette horloge en pause sinon.
*/
void ReverseClock::Toggle()
{
    if (IsPlaying())
    {
        Pause();
    }
    else if (GetRemainingTime() != -1)
    {
        Play();
    }
}
