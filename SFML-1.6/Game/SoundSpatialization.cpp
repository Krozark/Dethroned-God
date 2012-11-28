#include <math.h>					//for sqrt()
#include "SoundSpatialization.h"

/*static*/ void SoundSpatialization::initialize(float defaultMinDistance, float defaultAttenuation, float listenerDepth)
{
	SoundSpatialization::listenerDepth=listenerDepth;
    baseAttenuation=defaultAttenuation;

    //calculate the REAL min distance (see min distance sphere graphic @ blog.tbam.com.ar)
    baseEffectiveMinDistance=sqrt(defaultMinDistance*defaultMinDistance + listenerDepth*listenerDepth);

    sf::Listener::SetTarget(0, 0, -listenerDepth);		//face back to the XY plane
}

/*static*/ void SoundSpatialization::localizeListener(sf::Vector2f pos)
{
    sf::Listener::SetPosition(pos.x, pos.y, listenerDepth);		//go some distance away from the XY plane
}

/*static*/ void SoundSpatialization::setupSound(sf::Sound sound)
{
    snd=sound;
	snd.SetMinDistance(baseEffectiveMinDistance);
	snd.SetAttenuation(baseAttenuation);
}

/*static*/ void SoundSpatialization::localizeSound(sf::Vector2f pos)
{
	snd.SetPosition(pos.x,pos.y,0);
}
