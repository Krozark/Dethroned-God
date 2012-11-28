#ifndef PERSONNAGE_HPP
#define PERSONNAGE_HPP


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "Animated.hpp"
#include "Taille_fenetre.hpp"
#include "Object.hpp"
#include "Floor.hpp"
#include "Entite.hpp"


using namespace std;
using namespace sf;

class Personnage : public Entite
{
    public:

        //constructeurs & destructeurs
        Personnage(Image &image,int x,int y, Vector2f pos): Entite(image, x, y, pos)
        {
            init();
        }
        ~Personnage();

        //fonctions
        void init();
        void bouger_perso(RenderWindow* Window, float time);
        void SetDefaultCmd();

        //getters & setters
        void Set_mouv(int add);
        int Get_mouv();
        void Set_mouv_temp(int add);
        int Get_mouv_temp();

        void SetDisplacePerso(bool);
        bool GetDisplacePerso();
        bool m_displacePerso;
        void UpdateP(float time, std::vector<Floor> &floors);

        Anim GoRight, GoLeft;

        Event gauche,droite,courrir,pause,sauter;

        bool escape;
        bool GetEscape() { return escape;};


    private:

        int mouv;                   //mouvement initiole du perso
        int mouv_temp;              //mouvement souvegardé si le perso cour

        float dep_x,dep_y;
        int dep_dir,dep_touche;    //deplacement en x, deplacement en y, direction en degrés

        int touche_droite,touche_gauche;

        bool        key_left, key_right;
};

#endif
