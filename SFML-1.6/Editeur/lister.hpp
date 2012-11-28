#ifndef LISTEUR_HPP
#define LISTEUR_HPP

#include <iostream>
#include <string>
#include <vector>

#include <Qt/qobject.h>


class LISTEUR : public QObject
{
    /*****************************
    cette class permet de listere le contenu d'un dossier
    */

public:
        LISTEUR(); //constructeur
         std::vector <std::string> lister(std::string dir,std::string exp=std::string(".."),std::string exp2=std::string("."),std::string exp3=std::string(".")); //permet de spécifier le dossier à lister, et les exeption eventuelles
};
#endif

