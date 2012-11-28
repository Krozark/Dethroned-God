#ifndef SAVESTRUCT_HPP
#define SAVESTRUCT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Pnj.hpp"
#include "Objet.hpp"

////////////////////////////////////////////////
/// Structure de sauvegarde du Jeu
////////////////////////////////////////////////
struct _sauvegarde
{
    //nom du niveau
    char nomVerifLevel[16];
    char nomLevel[100];
    char mythologieLevel[100];
    char typeLevel[100];

    //temps de jeu
    float totalGameTime;

    //stats / scores du héros
    _saveHeros saveHeros;

    //position du héros
    int posXheros;
    int posYheros;

    //variable de quête
    int varQuete;

    //direction du héros
    _type_direction direction;

    //états des mobs(enable ou pas)
    bool mobs[100];

    //états des objets
    bool objets[100];

    int currentFrameAnimRecharge[TAILLE_RACCOURCIS_BARRE_SORTS];
};

#endif
