
#ifndef Acceuil_HPP
#define Acceuil_HPP

#include <iostream>
//SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//perso
#include "Taille_fenetre.hpp"
#include "Object.hpp"
#include "Game.hpp"

struct int2 {
    int x;
    int y;
};

class Acceuil
{
    public :
        Acceuil(RenderWindow* app);
        ~Acceuil();

        void OnUpdate();

        bool IsRunning() {return running;};
        bool IsGamed() {return gamed;};
        void Afficher();

        void Run(); //mettre en route le menu

        int X() {return x;};
        int Y() {return y;};

        void mettre_menu();         //mettre le menu en route

        void set_zoom_plus(); //zoomer dans le jeu
        void set_zoom_moins(); //dezoomer dansle jeu
        //Event event;

        void SetEvent(Event* event) { this->event=event; };

    private:
        //procédures
        void init(); //initialisation des constructeurs
        void clavier_action();
        void mouse_action();
        void mouse_move();
        void traitement_choix();
        void initial_position_menu(Unicode::Text);
        void Move_Sprite_Icon(int choix);
        unsigned int Get_Choix() {return Choix;};
        void Click_Run_Game(); //quand on choisit l'otpion "New game"
        void Run_Game();    //le stoper
        void Stop_Game(); //le relancer

        //fenêtre
        RenderWindow* App;

        //le jeu à lancer
        Game* game;
        Event* event;

        //apparence
        sf::String String_Titre;
        sf::Image Image_Fond;
        sf::Sprite Sprite_Fond;
        sf::Font F_Font;
        sf::Font F_Font_Titre;
        std::vector <String> tab_String; //texte SFML sous les boutons
        sf::Image Image_Icon; //icon pour la gestion des choix
        sf::Sprite Sprite_Icon;
        sf::Image Image_Animation_Choix; //animation sous le choix courant du menu
        Object Animation_Choix;
        std::vector<int2> tab_emplacement_boutons; //emplacement des boutons (texte)

        //sonds buffers pour les touche haut et bas et entrer
        sf::SoundBuffer SB_Bouger, SB_Validation;
        sf::Sound M_Bouger, M_Validation;

        //menu
        Music musicMenu;

        //variables de gestion
        int x,y;    //taille de la fenettre
        unsigned int Choix; //choix du menu
        bool running;    //si est en route
        bool gamed;

        int mousX,mousY;

        //évènements clavier
        Event touche_haut,touche_bas,touche_valide; //touche pour monter dans le menu, decendre, et valider.

};

#endif

