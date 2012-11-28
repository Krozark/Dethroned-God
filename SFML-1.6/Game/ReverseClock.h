#ifndef REVERSECLOCK_H
#define REVERSECLOCK_H

#include <SFML/System.hpp>

class ReverseClock : public sf::Clock
{
    public:
        //Initialise l'horloge, et la lance avec le temps donné si play = true
        ReverseClock(float t, bool play = false);
        //Initialise l'horloge à 0 et la pause
        ReverseClock();
        //Met en pause l'horloge
        void Pause();
        //Met en marche l'horloge si elle est en pause
        void Play();
        //Redémarre l'horloge avec la dernière valeur utilisée
        void Replay();
        //Change l'état de l'horloge (si non arrêtée) (pause<->marche)
        void Toggle();
        //Définit un nouveau temps pour l'horloge, et la démarre si play = true
        void SetTime(float t, bool play = false);
        //Retournent l'état de l'horloge
        bool IsPlaying();
        bool IsPaused();
        //Arrête l'horloge et la remet à zéro
        void Stop();
        //Retourne le temps écoulé depuis le lancement de l'horloge
        float GetElapsedTime();
        //Retourne le temps restant avant l'arrêt de l'horloge
        float GetRemainingTime();

    private:
        float finalTime;
        float remainingTime;
        float totalTime;
        bool m_pause;
};
#endif
