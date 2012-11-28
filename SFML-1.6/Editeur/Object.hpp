#ifndef OBJECT_HPP
#define OBJECT_HPP


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Animated.hpp"
#include "Taille_fenetre.hpp"

using namespace std;
using namespace sf;

/*************************************
classe permetant de gerer des objet
animée basiques
*************************************/

class OBJECT : public Animated::Animated
{
    public:


        OBJECT(); //constructeur par defuat
        OBJECT(Image &image,int x, int y,int x2,int y2); //constructeur ou l'on spécifie l'image sources, et on spécifie la subdivition à afficher
        OBJECT(Image &image,int x,int y,int color);//constructeur ou l'on spécifie l'image sources, son nombre de subdivision en vertical et horizontal, et quelle subdivision doit etre mise par defaut

        void charger_anim(Image &image,int x,int y, int color);//on spécifie l'image sources, son nombre de subdivision en vertical et horizontal, et quelle subdivision doit etre mise par defaut
        void charger_image_sprite(Image &image,int x,int y);//constructeur ou l'on spécifie l'image sources, son nombre de subdivision en vertical et horizontal, et on charge le nombre d'animation corespondantes

        int Get_object_image_taille_x() {return object_image_taille_x;}; //nombre de subdivision horizontale de l'image de fenettre_taille_pix pixels
        int Get_object_image_taille_y() {return object_image_taille_y;}; //nombre de subdivision verticales de l'image de fenettre_taille_pix pixels

        int Get_image_taille_x() {return image_taille_x;}; //revoie la taille = image.GetWidth();
        int Get_image_taille_y() {return image_taille_y;};//renvoie la taille  = image.GetHeight();


        void animation();
        ~OBJECT();

        //different animation possible pour l'objet
        Anim GoUp, GoRight, GoDown, GoLeft;


    private:
        void rectangle(Image &image,int x, int y,int z, int w);

        int object_image_taille_x;
        int object_image_taille_y;
        int image_taille_x;
        int image_taille_y;


};

#endif
