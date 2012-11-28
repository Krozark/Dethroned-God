#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Animated.hpp"
#include "Taille_fenetre.hpp"
#include "Object.hpp"

struct int2 //structure pour enregistre des coordonées
{
 int x;
 int y;
};

class MAP
{
/***********************
cette classe permet de gere les objet à ajouter dans la cartes
*/
    public:

    MAP(); // constructeur
    //skin,coordonées sur image skin x,y,x2,y2, coordonée dest sur fenettre en divisant par taille_objet_pix
    void ajout_fix(Image &image,int x1, int y1,int x2, int y2,unsigned int cx,unsigned int cy);
    void ajout_anim(Image &image,int taillex,int tailley,int ligne_bonne,unsigned int cx,unsigned int cy);

    void Charger_objet(OBJECT &obj); // permet d'ajouter un objet

    void Draw(RenderWindow &window); //permet de dessiner tout le rendu

    std::vector <OBJECT> tab_carte; // permet de stoquer tous els objets

    private:
    unsigned int x,y;   //taille en vertical et horizontale

};



#endif

