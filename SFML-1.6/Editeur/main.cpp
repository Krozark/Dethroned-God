#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <QtGui>

#include <iostream>

#include "editeur_MAP.hpp"



using namespace std;
using namespace sf;


int main(int argc, char **argv)
{
    QApplication QT(argc, argv);

    // On crée la fenêtre principale
    QFrame* MainFrame = new QFrame;
    //MainFrame->setWindowTitle("Editeur Dethroned God");
   // MainFrame->resize(fenettre_taille_x, fenettre_taille_y);

    //On crée une vue SFML pour le jeu dans la fenette QT
    EDITEUR_MAP* SFMLView = new EDITEUR_MAP(MainFrame);

     SFMLView->show();
     MainFrame->show();


    return  QT.exec();
}
