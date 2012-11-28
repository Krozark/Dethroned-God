#ifndef PAUSABLECLOCK_H
#define PAUSABLECLOCK_H

#include <SFML/System.hpp>


class PausableClock : public sf::Clock
{
public:
	//!Initialise le timer et le start
	PausableClock();
	//!Met en pause le timer
	void Pause();
	//!Met en marche le timer
	void Play();
	//!Change l'état du timer (pause<->marche)
	bool Toggle();
	bool IsPlay() { return !m_pause; };
	bool IsPause() { return m_pause; };
	//!Arrete le timer et le remet à zéro
	void Stop();
	//!Remet le timer à zéro et le met en marche
	void Reset();
	//!Retourne le temps écouler depuis la dernière fois qu'il à été mis en marche (avec Reset ou Play).
	float GetElapsedTime();

private:
	float m_elapsedTime;
	bool m_pause;
};
#endif
