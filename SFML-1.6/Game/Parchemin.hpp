#ifndef PARCHEMIN_HPP_INCLUDED
#define PARCHEMIN_HPP_INCLUDED

#include "Objet.hpp"
#include "Sons.hpp"
#include "Global_taille_fenetre.hpp"
#include <sstream>
#include <stdio.h>

enum _dir_change_ { PARCH_GAUCHE=0, PARCH_DROITE };

class Parchemin
{
    public :
        Parchemin(sf::Image *fond, Sons* son, _touches_heros* touchesHeros, _touches_sys* touches_Sys, std::vector<_skills>* skills);

        void maj(std::vector<Objet> &liste, sf::Image *fond);
        void display(sf::RenderTarget& app);
        void getEvents(sf::Event event);
        std::string convertText(std::string,bool appel=true);                 //convertis les \\n et \\t en \n et "   "


    private :
        //bool isMaj(char c);
        void changeCurrentParchemin(_dir_change_ dir);
        void positionneText();
        void initText();
        void initImages(sf::Image *fond);
        inline void insertRetourLigne(std::string *res);
        std::string findKey(sf::Key::Code &e);

        sf::Image               img_parchemin,
                                img_next,
                                img_fond,
                                img_previous;
        sf::Sprite              spr_parchemin,
                                spr_fond,
                                spr_next,
                                spr_previous;

        std::vector<std::string>    texteQuete;
        std::vector<int>       idSkills;

        std::string             texteSkill[TAILLE_SKILLS],
                                titreSkill[TAILLE_SKILLS];

        sf::Image               imageSkill[TAILLE_SKILLS];

        sf::Sprite              spriteSkill[TAILLE_SKILLS];

        sf::String              txtHaut, //texte en haut du parchemin "<= 1/3 =>"
                                quete,
                                txt,
                                skills,
                                skillsTitre,
                                skillsContain;

        sf::Font                font;

        int                     tailleChar,
                                tailleParchemin,
                                currentParchemin;

        unsigned int            nbCharParLigne;

        Sons*                   son;

        _touches_heros* touches;
        _touches_sys* touchesSys;
};

#endif

