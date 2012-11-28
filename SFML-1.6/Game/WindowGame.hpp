#ifndef WINDOWGAME_HPP
#define WINDOWGAME_HPP

#include <iostream>

#include "Taille_fenetre.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "MenuSauvegarde.hpp"
#include "PausableClock.h"
#include "Sons.hpp"


class WindowGame
{
    public:
        WindowGame(sf::RenderWindow* Window, _options* settings, _touches_heros* touchesHeros, _touches_sys* touches_Sys);
        void run();
        void eventKey(sf::Event event);

    private:
        void changeLevel();
        inline void returnKey();
        inline void gameOver();
        inline void cheatByeBye();

        int levelCourant;

        sf::RenderWindow* App;

        sf::Sprite spr_gameover;
        sf::Image  img_gameover;

        PausableClock   clock;
        ReverseClock    clockChargement;

        Sons            *son;
        Game            *game;
        Menu            *menu;
        MenuSauvegarde  *menuSave;
        _options        *settings;
        _touches_heros  *touches;
        _touches_sys    *touchesSys;
        _saveHeros      saveForLevelSvt;
        _sauvegarde     save;
        statusJeu       ETAT;
};

#endif
