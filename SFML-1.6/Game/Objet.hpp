#ifndef OBJET_HPP_INCLUDED
#define OBJET_HPP_INCLUDED

#include "Entite_non_jouable.hpp"


class Objet : public Entite_non_jouable
{
    public :
       // Objet(sf::Image& skin, int frameX, int frameY, _type_direction dirDepart, sf::Vector2f pos, _type_objets type=OBJET_RIEN);
        Objet(sf::Image &skin,_OBJ_ &obj);
        Objet() {};

        void    Update(float time,int nombre_division, bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX]);

        _type_objets getType() { return typeObjet; };
        bool operator==(const Objet &obj) {return (typeObjet==obj.typeObjet)? true:false;};
        bool operator==(const _type_objets &type) {return (typeObjet==type)? true:false;};

        int getIdSkill() { return idSkill; };
        sf::Vector2f GetDestination() {return destination;};
        const inline float getRebond() { return -destination.x; };
        const inline float getAcceleration() { return destination.x; };
        const inline float getOr() { return destination.x; };
        void               setOr(float val) { destination.x = val; };
        const inline float getExperience() { return destination.y; };
        const inline float getDegats() { return destination.x; };

        bool operator ^(Entite &e);

    private :
        void loadOneAnim(sf::Image &image,int nbLignes,int y,int x=4);

        _type_objets typeObjet;
        int idSkill;
        sf::Vector2f destination;

};

#endif
