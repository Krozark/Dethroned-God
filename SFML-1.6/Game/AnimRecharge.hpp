#ifndef ANIMRECHARGE_HPP
#define ANIMRECHARGE_HPP

#include <iostream>

#include "Animated.hpp"
#include "Taille_fenetre.hpp"

class AnimRecharge : public Animated
{
    public :
        AnimRecharge(sf::Image &img,int tailleImgX,int temps);

        void run(float time);
        void setTime(int time);

    private :
        void preloadAnim(sf::Image &img,int x); //charge les images et les sprites
        void loadAnim(sf::Image &image, int x); //charge les animations

        Anim        animGame;
        sf::Sprite  spr;

};


#endif

