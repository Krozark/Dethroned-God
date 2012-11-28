#ifndef EDITEUR_SKILL_HPP
#define EDITEUR_SKILL_HPP

#include "Taille_fenetre.hpp"
#include "lister.hpp"
#include <QtGui>
/*********************************************
cette classe permet de creer et de modifier
les pouvoirs que peu avoir le hero
(sans pour autant pouvoir permettre
 d'ecrire le code c/c++ du sort[fait dans le jeu])
**********************************************/

class Editeur_Skill : public QFrame
{
    Q_OBJECT

public:

    //constructeur par defaut
    Editeur_Skill(QWidget *parent=0): QFrame(parent)
    {
        init();
    };

    void init(); //fonction d'initialisation qu'il faut appeler dans les constructeur
    void chargerFichier();  //fonction qui permet de charger le fichier de skill et de mettre les variable à jour



public slots:
        void addLigne();    //permet d'ajouter une nouvelle ligne, et donc un nouveau skill
        void ecrireFichier();   //enregistre les donnée dans le fichier de skill

private:
    QPushButton* Add,*Save;     //boutons pour ajouter un nouveau skill  et sauver dans un fichier
    QGridLayout* layoutScroll;  //layout contenant tous les widgets
    QVBoxLayout* VLayout;           //layout pour les boutons
    QWidget* conteneur;             //objet dans lequelle tout est contenu
    LISTEUR Listeur;                //objet qui permet de lister le contenu d'un dossier

    QScrollArea* Scroll;        //objets pour avoir un defilement en prevention d'un nombre important de skills
    QLabel* entete[7];          //entete du tableau de widget (savoir à quoi chaqun sert)

    //pour tous les tableau suivant, la case i de chaqun des tableau corespond au même skill quelque soit le tableau utilisé

    std::vector<QLineEdit*> tab_Nom; //tableau contenat les nom des skills
    std::vector<QLabel*> tab_ID;    //tableau contenant les ID des skills
    std::vector<QTextEdit*> tab_description;    //tableau contenant les description des skills
    std::vector<QComboBox*> tab_Icon;       //tableau contenant les icons des skills et leur chemin
    std::vector<QSpinBox*> tab_duree;       //tableau contenant la duréé de chaque skill
    std::vector<QSpinBox*> tab_recharge;    //tableau contenant le temps qu'il faut pour recharger le skill
    std::vector<QSpinBox*> tab_mana;        //tableau contenant le mana qu'il faut pour utiliser le skill
};


#endif
