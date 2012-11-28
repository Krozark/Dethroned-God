#ifndef REVERSECLOCK_H
#define REVERSECLOCK_H

#include <SFML/System.hpp>

class ReverseClock : public sf::Clock
{
    public:
        //Initialise l'horloge, et la lance avec le temps donn� si play = true
        ReverseClock(float t, bool play = false);
        //Initialise l'horloge � 0 et la pause
        ReverseClock();
        //Met en pause l'horloge
        void Pause();
        //Met en marche l'horloge si elle est en pause
        void Play();
        //Red�marre l'horloge avec la derni�re valeur utilis�e
        void Replay();
        //Change l'�tat de l'horloge (si non arr�t�e) (pause<->marche)
        void Toggle();
        //D�finit un nouveau temps pour l'horloge, et la d�marre si play = true
        void SetTime(float t, bool play = false);
        //Retournent l'�tat de l'horloge
        bool IsPlaying();
        bool IsPaused();
        //Arr�te l'horloge et la remet � z�ro
        void Stop();
        //Retourne le temps �coul� depuis le lancement de l'horloge
        float GetElapsedTime();
        //Retourne le temps restant avant l'arr�t de l'horloge
        float GetRemainingTime();

    private:
        float finalTime;
        float remainingTime;
        float totalTime;
        bool m_pause;
};
#endif
