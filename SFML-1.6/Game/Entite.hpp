#ifndef ENTITE_HPP_INCLUDED
#define ENTITE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Animated.hpp"
#include "Taille_fenetre.hpp"
#include "Globale_entite_game.h"
#include "Sons.hpp"
#include "PausableClock.h"


class Entite : public Animated
{
    public:
        //constructeurs & destructeurs
        Entite(sf::Image &image,int x,int y, _type_direction dirDepart, sf::Vector2f pos);
        Entite() {};

        void charger_image_sprite(sf::Image &image,int x,int y);
        void AddForce(sf::Vector2f force);
        void Stopping_Frotement(float);
        virtual void Update(float time,int nombre_division, bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX]);

        ////////////////////////////////////////////////////////////////////////////
        /// Getters and Setters
        ////////////////////////////////////////////////////////////////////////////
        const _type_direction   GetDirection() {return direction;};
        const sf::Vector2f      getForce() {return m_vector;};
        const int               getQuette() { return quette; };
        const bool              getFloorCollision() {return m_floor_collision;};
        const bool              isEnable() { return enable; };
        const int               getVie() { return vie; };
        const int               getVieMax() { return viemax; };
        const int               getMana() { return mana; };
        const int               getManaMax() { return manamax; };
        void                    setEnable(bool val) { enable=val; }
        virtual void            SetDirection(_type_direction d);
        void                    SetForce(sf::Vector2f force) {m_vector=force;};
        void                    SetGravite(float g) {if (g>0)  gravitee=g;};
        void                    SetFrottement(int f) {if(f>0) frottement=f;};
        void                    setMana(int nb) { mana=nb; };
        void                    setVie(int nb) { vie=nb; };
        void                    AddForceX(float f) { m_vector.x+=f;};
        void                    AddForceY(float f) { m_vector.y+=f;};
        void                    SetForceX(float f) { m_vector.x=f;};
        void                    SetForceY(float f) { m_vector.y=f;};
        virtual void            modifieVie(int nb) {};
        virtual void            modifieMana(int nb) {};


        Anim            GoRight,
                        GoLeft,
                        GoDown,
                        GoUp;

        sf::Vector2f        m_vector;

        bool    operator ^(Entite &e);  //verifie les colisions entre 2 entitées




    protected:
        virtual void chargementAnimations() {};
        virtual void initAnimTemp() {};
        virtual void showAnimations(sf::RenderWindow *App) {};
        void    loadAnim(sf::Image &image, int x, int y, Anim*, Anim* animLeft=0);

        float       gravitee;
        int         frottement,
                    quette,
                    vie,
                    viemax,
                    mana,
                    manamax;

        sf::Vector2f    oldPos;
        _type_direction direction;
         Entite(sf::Vector2f pos);

        bool        m_floor_collision,
                    enable;
};


#endif
