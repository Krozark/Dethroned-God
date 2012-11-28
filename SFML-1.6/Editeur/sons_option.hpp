#ifndef SONS_OPTION_HPP
#define SONS_OPTION_HPP

#include "Taille_fenetre.hpp"
#include <SFML/Audio.hpp>
#include <QtGui>
#include <iostream>
#include "lister.hpp"


class SONS_OPTION : public QFrame
{
    Q_OBJECT
    public:
    SONS_OPTION(QWidget* parent =0) :QFrame (parent)
    {
        init();
    };

    void SetSon(Sons_Spacialise * son){curent_son=son; MAJ();};

    void closeEvent(QCloseEvent* event);

    public slots:
        void Play_stop();
        void ChangeSon(QString);
        void ChangeAttenuation(double);
        void ChangeMinDistance(double);
        void ChangeVolume(double);
        void ChangeTypeSon();
    private:

    std::string chemin;
    LISTEUR listeur;
    Sons_Spacialise* curent_son;
    QPushButton* Play_Stop;
    QPushButton* Type_Sons;
    QComboBox*  Choix_son;
    QDoubleSpinBox *attenuation,*MinDistance,*Volume;
    QLabel *Label[3];
    QGridLayout* layout;
    sf::Music music;
    void init();
    void MAJ();

};



#endif
