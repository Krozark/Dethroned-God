#include "ReverseClock.h"

using namespace sf;

/*
* Crée une horloge ;
* Si play = true, démarre l'horloge,
* Si play = false, met l'horloge en pause.
*/
ReverseClock::ReverseClock(float t, bool play)
{
    SetTime(t, play);
}

/*
* Crée une horloge sans la démarrer.
*/
ReverseClock::ReverseClock() : totalTime(0.f)
{
    Stop();
}

/*
* Retourne le temps restant avant la fin d'une horloge (hors pause),
* Retourne -1 si celle-ci est déjà arrêtée.
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
* Retourne le temps écoulé depuis le démarrage d'une horloge (hors pause).
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
* Redémarre une horloge en pause si elle n'est pas déjà arrêtée.
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
* Redémarre une horloge avec le dernier temps avec laquelle elle a été démarrée.
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
* Arrête une horloge.
*/
void ReverseClock::Stop()
{
	remainingTime = -1;
	m_pause = true;
}

/*
* Définit un nouveau temps pour l'horloge,
* - La démarre si play = true
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
* Redémarre une horloge si elle est en pause mais n'est pas arrêtée,
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
