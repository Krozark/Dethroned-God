#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include "Animated.hpp"
#include "Taille_fenetre.hpp"
#include "Objet.hpp"
#include "Sons.hpp"
#include "Global_taille_fenetre.hpp"


class Menu
{
    public :
        Menu(sf::RenderWindow* Window, _options* settings, Sons* son, _touches_sys* touchesSy);
        void run();
        void eventKey(sf::Event event2);
        bool getLaunchGame() { return launchGameAllowed; };
        bool getLaunchLoad() { return launchLoadAllowed; };


    private :
        void init();
        void loadAnim(sf::Image &image,int x,int y, int color);
        //void charger_image_sprite(sf::Image &image,int x,int y);
        void display();
        void moveSpriteIcon(int choix);
        void traitementChoix();
        void launchGame();
        unsigned int Get_Choix() {return Choix;};

        sf::RenderWindow*   App;
        _options*           settings;
        sf::Music           musicMenu;
        sf::Image           cursor,
                            exit,
                            newGame,
                            load,
                            title,
                            background;
        sf::Sprite          spr_cursor,
                            spr_new,
                            spr_load,
                            spr_exit,
                            spr_title,
                            spr_background;

        //Object*         animChoix;
        Anim            GoUp,
                        GoDown,
                        GoLeft,
                        GoRight;
        sf::Event           event;
        int             Choix;
        std::vector<sf::Sprite>  tab_choice;
        bool            launchGameAllowed,
                        launchLoadAllowed;
        _touches_sys* touchesSys;

        Sons*            son;
};


#endif


