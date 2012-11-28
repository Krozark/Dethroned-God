#ifndef EDITEUR_MAP_HPP
#define EDITEUR_MAP_HPP

#include "QSFMLCanvas.hpp"
//base
#include <fstream>
//QT
#include <QtGui>
//SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//perso
#include "Taille_fenetre.hpp"
//#include "map.hpp"
#include "lister.hpp"
#include "afficheur.hpp"
#include "editeur_fenettre.hpp"

class EDITEUR_MAP : public QSFMLCanvas
{
    /***************************
    cette classe représente la fenettre
    glabale de l'application.
    elle delègue des traitement à d'autre
    class
    */
    Q_OBJECT

public :


    EDITEUR_MAP(QWidget* Parent=0) : QSFMLCanvas(Parent, QPoint(0,0), QSize(fenettre_taille_x,fenettre_taille_y)) //constructeur
    {
        init();
    };

    void mousePressEvent ( QMouseEvent * event ); //permet de gérer les evenement souris
    void keyPressEvent (QKeyEvent * event);//permet de gere les evenement clavier

    void OnInit();//se qui doit etre fait au premier lancement
    void OnUpdate();//se qui est fait à chaque frame
    void init();//se que doit etre fait dans tous les constructeurs

    QScrollArea* F_Edition; //fenettre d'édition de la carte
    AFFICHEUR_GRILLE *affiche; //classe permetant de gere l'edition de la carte


public slots:

    void Set_map(const QString m);  //permet de charger une carte
    void ecrire_fichier(bool=false); // permet d'enregistrer le niveau dans un fichier
    void reset(); //permet d'effacer la carte courante
    void quitter();
    void envoie_site();
    void Set_map_load(); //permet de charger une carte depuis un fichier
    void Set_type_edition(int c) {type_edition=(_type_edition)c;affiche->Set_type_edition(c);}; // permet de choisir le mode d'edition
    void Set_Ligne(int c) { if (type_edition == OBJ) OBJEditeur->Set_Ligne(c);else if (type_edition == PNJ) PNJEditeur->Set_Ligne(c);};//permet de choisir la direction / ligne d'une image à animer
    void SetType(int t) {affiche->SetType((_type_objets)t);}; // permet de choisir le type d'edition courant
    void SetLvl(int l) {affiche->SetLvl(l);}; //permet de choisir le niveau courant
    void ChangeAutotEnabled(bool); //permet de choisir si le mode automatique doit etre activé ou pas
    void BougerFenettreCliked() {bougerFenettre=true;}; //permet de dire si on doit bouger la fenettre ou pas ici oui
    void BougerFenettreRelase() {bougerFenettre=false;};//idem ici non

private:

    void initPNJ(_PNJ* p); //permet d'initialiser un PNJ

    LISTEUR resources; // permet de lister les fichier contenu dans un dossier
    std::vector <std::string> tab_nom_map; //nom des dossiers de carte

    QComboBox* B_Maps; // permet de choisir le nom de la carte à utilisé
    QComboBox* B_Choix; //permet de choisir le choix du type d'edition

    QPushButton* Enregistrer;//permet de dire que l'ion veu enregistrer la carte dans un fichier
    QLineEdit* NomFichier;//permet de choisir le nom de la carte a enregistrer, charger
    QPushButton* Reset; //bouton pour dire que l'on veu reset la carte, et la remetre à zero
    QPushButton* Load; //permet de dire que l'on veu charger la carte
    QPushButton* B_Quit; // permet de quiter l'application
    QPushButton* Site;
    QCheckBox* edition_auto; // permet de choisir le choix d'edition automatique des rebors
    //MAP
    std::vector <sf::Sprite> grille_map; // tableau contenant toutes les images à afficher de la carte

    std::vector <sf::Image> resources_image_map;//tableau contenant les image sources de la carte
    bool AutoIs;            //savoir si le bouton d'edition auto est enfoncé ou pas
    bool AutoEnabled;       //savoir si l'image dedition auto existe ou pas
    bool VerifAuto();   //on verifi si on peu ou pas appliquer le mode d'automatisme
    std::vector<sf::Sprite> resources_sprite_map;//tableau contenant les resources d'une carte que l'on peu utiliser pour la creer
    std::vector <std::string> resources_image_map_nom; //nom des images
    string curent_map;//chemin de la carte en cour
        //Objets
    QFrame* OBJFrame; // fenettre de choix des different objets
    EDITEUR_FENETTRE* OBJEditeur; //classe qui permet le choix des different objets
        //PNJ
    QFrame* PNJFrame; //fenettre de choix des different pnj
    EDITEUR_FENETTRE* PNJEditeur;//classe qui permet le chois des different pnj
    //taille editeurs
    int tx,ty;


    sf::Image I_grille; //image de base de la grille pour une case si rien y est mis
    sf::Sprite S_grille; // son sprite

    sf::Image curent_selection_image;//image courante qui est utilisé
    sf::Sprite curent_selection;//son sprite

    sf::String Erreur; // texte qui indique à l'utilisateur les differentes erreures faites, ou qui confirme

    sf::Vector2f clickPush,clickRelase; // coordonée de la souris quand elle est enfoncé, relaché


    void maj_resources(); // permet de mettre à jours les resources de la carte
    void charger_resources_map(); //permet de charger les resources de la cartes
    void afficher_resources_images(); //permet d'afficher les resources
    _type_edition  type_edition; //permet de definir le type d'edition en cour
    int choix_ligne; //permet de definir la ligne/direction qui doit etre appliquee aux objets/pnj
    bool bougerFenettre;//pour dir si il faut bouger la carte ou pas a cause de la souris
};

#endif

