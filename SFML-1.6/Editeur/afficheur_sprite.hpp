#ifndef AFFICHEUR_SPRITE_HPP
#define AFFICHEUR_SPRITE_HPP

#include "QSFMLCanvas.hpp"
#include "Taille_fenetre.hpp"
#include "Object.hpp"

class AFFICHEUR_SPRITE : public QSFMLCanvas
{
    /*****************************
    cette classe permet simplement
    l'affichage d'une animation
    et qui puisse etre utilisé comme
    un widget
    */

    public:

    AFFICHEUR_SPRITE(QWidget* Parent=0): QSFMLCanvas(Parent, QPoint(0,0), QSize(taille_objets_pix*2,taille_objets_pix*2)) //constructeur
    {
    };


    void OnUpdate(); //se qui est fait à chaque frame
    void OnInit(); //se qui est fait à l'initialisation

    void Set_sprite(OBJECT s); // permet de définir l'objet à afficher

    OBJECT spr; //permet de garder en memoir l'objet à afficher
};


#endif
