#ifndef SOUNDSPATIALIZATION_H
#define SOUNDSPATIALIZATION_H

#include <SFML/Audio.hpp>

class SoundSpatialization {
    float listenerDepth;
    float baseAttenuation;
	float baseEffectiveMinDistance;
	sf::Sound snd;

public:
	/**
	Initialize sound spatialization.

	\param defaultMinDistance	the minimum distance from which the sound will listener will \em{ in the XY plane }.
	\param defaultAttenuation	the minimum distance from which the listener will in the XY plane
	\param listenerDepth		the distance on the Z axis where to put the listener
	*/
	void initialize(float defaultMinDistance=200.f, float defaultAttenuation=17.5f, float listenerDepth=300.f);

	/**
		Applies the base attenuation and min distance parameters to a sound
	*/
	void setupSound(sf::Sound sound);

	/**
		Sets the position of a sound given a "screen" position
	*/
	void localizeSound(sf::Vector2f pos);

	/**
		Sets the position of the listener in a "screen" position
	*/
	void localizeListener(sf::Vector2f pos);
};

#endif // SOUNDSPATIALIZATION_H
