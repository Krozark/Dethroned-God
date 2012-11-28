#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <Qt/qobject.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "lister.hpp"


using namespace std;

LISTEUR::LISTEUR()
{
}


 std::vector <std::string> LISTEUR::lister(std::string dir,std::string exp,std::string exp2,std::string exp3)
{

    std::vector  <std::string> tab_nom;

    DIR *curDir;
    std::string DIRECTORY=dir; /*Ceci est le répertoire courant*/
    bool b_error = false;
    struct dirent *curEntry;
    errno = false;

    if (NULL == (curDir = opendir(DIRECTORY.c_str())))
    {
        //verification des erreurs
        b_error = true;

    }
     else
    {
        errno = false;

        while (NULL != (curEntry = readdir(curDir)))
        {
        //stockage des nom de fichiers dans tab_nom si  different des dossiers system
            if ((strcmp(curEntry->d_name,"."))&& (strcmp(curEntry->d_name,".."))&& (strcmp(curEntry->d_name,exp.c_str()))&& (strcmp(curEntry->d_name,exp2.c_str()))&&(strcmp(curEntry->d_name,exp3.c_str())))
            tab_nom.push_back(curEntry->d_name);
        }

        if (errno)
        {
            b_error = true;
        }
    }

    //return
    if (b_error)
    {
        tab_nom.clear();
        return (tab_nom);
    }
    else
    {

        return tab_nom;
    }
}




