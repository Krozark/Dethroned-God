#ifndef ENTITE_NON_JOUABLE_HPP
#define ENTITE_NON_JOUABLE_HPP

#include "Entite.hpp"


class Entite_non_jouable : public Entite
{
    public:

        Entite_non_jouable(sf::Image &image,int x,int y, _type_direction dirDepart, sf::Vector2f pos) : Entite (image,x,y,dirDepart,pos)
        {
            dialogue = "";
            quette=0;
            enable=true;
            mustDead=false;
        };
        Entite_non_jouable() {};

        std::string    GetDialogue() {return dialogue;};
        bool           isDead() { return mustDead; };
        inline void    setDead(bool val) { mustDead=val; };
        inline void     Toggle() { if (direction==GAUCHE) SetDirection(DROITE);else SetDirection(GAUCHE);m_vector.x=0;}
        virtual void    modifieMana(int nb) {};
        virtual void    modifieVie(int nb) {};

    private :

    protected:
        bool mustDead;
        std::string      dialogue;

//construct

};
#endif
