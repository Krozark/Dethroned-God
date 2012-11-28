#ifndef PNJ_HPP_INCLUDED
#define PNJ_HPP_INCLUDED

#include "Entite_non_jouable.hpp"
#include "ReverseClock.h"

#define vector_saut Vector2f(0,-puissance_saut-GetSize().y-32)
#define Sauter();  if (m_floor_collision and  m_vector.y<=0){ AddForce(vector_saut);}

class Pnj : public Entite_non_jouable
{
    public :
        //Pnj(sf::Image& skin, int frameX, int frameY, _type_direction dirDepart,sf:: Vector2f pos, int x1, int x2, int speed=150);
        Pnj(sf::Image& skin,_PNJ_ &pnj);
        void    bouger_mob(sf::RenderWindow* Window,float time,int nombre_division, bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX]);
        void    trouveEntite(Entite& h);
       /* inline*/ void    Update(float time,int nombre_division, bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX]);

        void    SetVue(unsigned int v) {vue =(int)v*TAILLEBLOC;};
        void    SetMax(unsigned int m) {max= (int)m;};
        void    SetMin(unsigned int m) {min=(int)m;};

        void    modifieVie(int nb,bool critique=false,bool epicFail=false);
        void    modifieVie(int nb) {};
        void    modifieMana(int nb) {};

        int     getExp() {return exp;};
        int     getQuette() {return quette;};
        int     getDegats() {return degats;};
        bool    isAgressif() {return agressif;};
        void    SetAgressif(bool v);
        bool    isBoss() {return (boss);};
        bool getBloque() {return m_bloque;};
        const int   getLvl() { return lvl; };

        _type_element getElement() {return element;};
        _type_direction GetTypeSaut() {return saut;};

        std::vector<sf::String> bonusMalus;
        std::vector<float>  bonusMalusPosDepartY;

        bool operator ^(Entite &e);

      private :
        int                 min,min_origin,
                            max,max_origin,
                            speedMin,speedMax,
                            vue,  //distance de vision
                            exp,
                            speed,
                            degats,
                            berzerk_force,
                            lvl,
                            nb_case_saut,
                            puissance_saut;

        bool                agressif,
                            volant,
                            boss,
                            m_bloque,
                            m_bloque_last,
                            possede_anim_tapper;

        _type_element       element;
        _type_direction     saut;

        enum _ETAT_PNJ{ETAT_RIEN=0,ETAT_GOTO,ETAT_STOP,ETAT_FRAPPE,ETAT_FUI,ETAT_IS_FRAPPE} Etat;
        sf::Vector2f Position_Enemie;

        inline void Change_puissance_saut(int puissance)
        {
            puissance_saut=(puissance <0)? 0:((puissance >speed)? speed:puissance);
        };

        inline void Change_puissance_saut_max()
        {
            puissance_saut=speed;
        };


        protected:
        ReverseClock clock,temps_agro;
};

#endif
