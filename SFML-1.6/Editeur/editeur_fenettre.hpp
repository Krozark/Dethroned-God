#ifndef EDITEUR_PNJ_HPP
#define EDITEUR_PNJ_HPP

#include "QSFMLCanvas.hpp"
//base
#include <fstream>
//QT
#include <QtGui>
//SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
//perso
#include "Taille_fenetre.hpp"
#include "lister.hpp"

class EDITEUR_FENETTRE : public QSFMLCanvas
{
    /***************************
    cette classe permet d'afficher
    different image contenues dans un doddier
    sous forme de tableau
    et permet d'en choisir une
    pour l'utiliser ailleur
    */
    Q_OBJECT

public :

    EDITEUR_FENETTRE(std::string dos,int X=1,int Y=1,QWidget* Parent=0,int TX=taille_objets_pix*20,int TY=taille_objets_pix*30) : QSFMLCanvas(Parent, QPoint(0,0), QSize(TX,TY)) //constructeur ou on spécifier le chemin du dossier à examiner, le nombre de subdivision de l'image, et le nombre max en x et y a afficher
    {
        dossier=dos;
        x=X;
        y=Y;
        init();
    };

    void mousePressEvent ( QMouseEvent * event ); //permet de gere les evenement de la souris

    void OnInit(); //se qui doit etre fait au premier lancement
    void OnUpdate(); //se qui doit etre fait à chaque  frame
    void init(); //se qui doit etre fait dans tous les constructeurs
    void Set_fenettre_principale(QWidget* f) { fenettre_principale=f;QObject::connect(B_Choix_ligne,SIGNAL(currentIndexChanged (int)),fenettre_principale,SLOT(Set_Ligne(int)));}; // permet dedeffinir la fennetre conteneuse de l'objet

    void Set_Ligne(int c) {if(c<y && c>=0) ligne=c;else ligne=y-1;ChargerSprites();}; // permet de definir la des image qui doit etre affiché
    void ChargerSprites(); //permet de charger tous les sprite avec les lignes quil faut
    void ChargerImages( );//permet de charger les image du dossier
    std::string Get_dossier() {return dossier;}; //retourne le dossier qui est utilisé comme sources
    unsigned int Get_ligne() {return ligne;}; // permet de connaitre la ligne qui est en cour d'utilisation sur les images

    unsigned int Get_curent() {return curent_case;}; // permet de connaitre la case du tableau qui à ete selectionné
    unsigned int Get_passed() {return passed_case;}; // permet de connaitre la casse precedente avecn la selection
    _type_objets Get_type() {return (_type_objets)(B_Type->currentIndex());}; //permet de connaitre le type d'objets qui doit etre utilisé
    int Get_x() {return x;}; //permet de connaitre le nomre de subdivision en X
    int Get_y() {return y;};//idem en Y

    //resources sprites
    int add_image(std::string s,std::string dossier=".",int y=-1,int dir=-1); //ajoute une image et renvoi la case dans laquelle elle se trouve pour les tableaux
    std::vector <std::string> resources_image_nom; // tableau contenant le nom de toutes les images qui sont utilisées
    std::vector <sf::Image> resources_image; // tableau contenant les images sources qui sont affichées

public slots:
    void changeLvl(int); // permet de choisir les niveau des monstres à utiliser
    void changeType(int); //permet de choisir le type des objet qui doit etre utilisé

    signals:
    void LvlChanged(int); //permet de dire que le niveau à changé
    void TypeChanged(int); //permet de dire que le type à changé

private:

    LISTEUR resources; //permet de lister les fichier contenus dan un dossier
    //nombre de ligne par image et collone
    int x,y;
    //ligne en cour d'utilisation dan l'image
    unsigned int ligne;
   //case en cour de selection
    unsigned int curent_case,passed_case;
    //fenettre principale du programme
    QWidget* fenettre_principale;
    sf::Image I_Selection;  //image source pour montrer quelle case est selectionné
    sf::Sprite S_Selection; //son sprite
    //resources
    std::string dossier; // permet de retenir le dossier utilisé

    std::vector<sf::Sprite> resources_sprite; // tableau contenant les sprites qui sont affichés
    //bouton
    QComboBox* B_Choix_ligne,*B_Type;//permet de choisir la direction/subdivision de l'image à utilisé, le type d'objet à placer sur la carte
    QSpinBox * B_lvl; // permet de choisir le niveau des monstres à positionner
};

#endif
