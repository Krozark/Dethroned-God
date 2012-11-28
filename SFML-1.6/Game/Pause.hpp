#ifndef PAUSE_HPP
#define PAUSE_HPP

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

#include "Taille_fenetre.hpp"
#include "Sons.hpp"
#include "Global_taille_fenetre.hpp"
#include <SFML/Network.hpp>

#define NBSTATS 6
#define NB_RESOLUTIONS 12

enum CHOIX_PAUSE { STATUS=0, SCORES, SKILLS, INVENTAIRE, OPTIONS, RETOUR, QUITTER, TAILLE_CHOIX_PAUSE };
enum CURRENT_SCREEN { LEFT_SCREEN, RIGHT_SCREEN };
enum OPTIONS_GENERALES { VOLUME_MUSIQUE_JEUX=0, VOLUME_EFFETS_SONORES, VOLUME_MUSIQUE_MENUS, VOLUME_SON, FPS, SYNC_VERTICALE, RESOLUTION, FULLSCREEN, TOUCHES_PERSO, TOUCHES_SYS, TAILLE_OPTIONS_GENERALES };
enum OPTIONS_PERSO { TOUCHE_GAUCHE=0, TOUCHE_DROITE, TOUCHE_SAUTER, TOUCHE_FRAPPER, TOUCHE_COURRIR, TOUCHE_RAMASSER, TOUCHE_PAUSE, TAILLE_OPTIONS_PERSO };
enum OPTIONS_SYS { TOUCHE_HAUT_SYS=0, TOUCHE_BAS_SYS, TOUCHE_GAUCHE_SYS, TOUCHE_DROITE_SYS, TOUCHE_VALIDER, TOUCHE_RETOUR, TAILLE_OPTONS_SYS };
enum CTRL { CTRL_GAUCHE=0, CTRL_DROITE, CTRL_SAUTER, CTRL_COURRIR, CTRL_TAPER, CTRL_PAUSE, CTRL_RAMASSE, CTRL_SCREENSHOT, CTRL_GUI1, CTRL_GUI2, CTRL_GUI3, CTRL_GUI4, CTRL_GUI5,CTRL_GUI6,CTRL_GUI7,CTRL_GUI8,CTRL_GUI9,CTRL_GUI10, CTRL_RETOUR, TAILLE_CTRL };
enum CTRLSYS { CTRLSYS_GAUCHE, CTRLSYS_DROITE, CTRLSYS_HAUT, CTRLSYS_BAS, CTRLSYS_RETOUR, CTRLSYS_ENTREE, CTRLSYS_BOUTTON_RETOUR, TAILLE_CTRLSYS };
enum BOITE_CONNECTION { CHAMP_PSEUDO, CHAMP_MDP, CHAMP_CANCEL, CHAMP_OK };

class Pause
{
    public :
        Pause(int stats[], int scores[], int xp[], int vie, int mana, sf::String tab_nbobjets[TAILLE_OBJETS_LOOT], bool skills[], _options* settings, _touches_heros* touchesHeros, _touches_sys* touchesSys, Sons* son, std::vector<_skills>* vectorSkills, int level);
        void run();
        void show(sf::RenderTarget &app);
        void getEvents(sf::Event event);
        void reset();
        void majaData(int stats[], int scores[], int xp[], int vie, int mana, sf::String tab_nbobjets[TAILLE_OBJETS_LOOT], bool skills[]);
        void majBS(sf::Sprite guiSpr[]);

        void        setExit(bool val)  { exit=val;     };
        const bool  getExit()          { return exit;  };
        const bool  getExitGame()      { return gameExited; };
        const bool  getChangeOptions() { return changeOptions; };
        const bool getVerticalSync() { return (std::string(optionsGeneralesValues[SYNC_VERTICALE].GetText()) == "oui") ? true : false; };
        const bool getFullScreen() { return (std::string(optionsGeneralesValues[FULLSCREEN].GetText()) == "oui") ? true : false; };
        const int getNbPtsSkill() { return atoi(std::string(statsPersoValues1[NB_POINTS_SKILLS].GetText()).c_str()); };
        const std::string getResol() { return resolutions[cptResolution]; };

        int tabPlusMoins[NBSTATS-1];


    private:
        void initVariables();
        void initChoix();
        void initImages();
        void initMenuStatus();
        void initMenuScores();
        void initMenuSkills();
        void initMenuInventaire();
        void initMenuOption();
        void initSousMenuTouchesHeros();
        void initSousMenuTouchesSys();
        void initSousMenuBarreSort();
        void initBoiteConnection();
        void moveCursor(int selection);
        inline void moveCursorStatus(int selection);
        inline void moveCursorOptions(int selection);
        inline void moveCursorInventaire(int selection, bool sautLigne=false);
        inline void moveCursorConnection(int selection);
        void changeScreen(CURRENT_SCREEN dir);
        void validationChoix();
        void changeCarac(bool add);
        void drawStatus(sf::RenderTarget &app);
        void drawScores(sf::RenderTarget &app);
        void drawSkills(sf::RenderTarget &app);
        void drawInventaire(sf::RenderTarget &app);
        void drawOptions(sf::RenderTarget &app);
        void drawRetour(sf::RenderTarget &app);
        void drawQuitter(sf::RenderTarget &app);
        void drawConnectionSite(sf::RenderTarget &app);
        std::string convertText(std::string);
        inline void insertRetourLigne(std::string *res);
        void majTextesSkills();
        void changeCurrentSkill();
        void retour();
        inline void exitMenuPause();
        inline void exitGame();
        inline void initTitreMenu(sf::String *titre,sf::String txt);
        inline void placeCursor(sf::String chaine, int width=0);
        inline void placeCursor(sf::Vector2f pos, int width, float height=0.15);
        void rightKey();
        void leftKey();
        std::string findKey(sf::Key::Code &e);
        void saveChanges(sf::Event event);
        inline void redefineTouches(sf::Event event);
        inline void majRaccourcisTemp();
        inline void majRaccourcisObjTemp();
        inline void redefineRaccourcisBS(sf::Event event);
        inline void redefineRaccourcisBSObj(sf::Event event);
        inline int getCptFromResol();
        inline void majTextesConnection(std::string txt, bool pseudo=false);
        inline void writeTxt(int txt);
        inline void deleteText();
        inline void sendDataToSite();
        inline void quitConnectionSite();

        bool        exit,
                    gameExited,
                    changeOptions,
                    connexionSite,
                    responseSite;

        sf::String  texteSkill[TAILLE_SKILLS],
                    titreSkill[TAILLE_SKILLS],
                    manaSkill[TAILLE_SKILLS],
                    dureeSkill[TAILLE_SKILLS],
                    rechargeSkill[TAILLE_SKILLS],
                    manaSkillTitle,
                    dureeSkillTitle,
                    rechargeSkillTitle;
        sf::Image   imgSkill[TAILLE_SKILLS];
        sf::Sprite  sprSkill[TAILLE_SKILLS];
        int         idSkill[TAILLE_SKILLS],
                    levelCourant;
        bool        displaySkill[TAILLE_SKILLS];

        std::string toucheTemp,
                    txtExplicatifRaccourcisSkills,
                    txtExplicatifInventaire,
                    txtExplicatifRaccourcisObj,
                    descObjetsInventaire[TAILLE_OBJETS_LOOT],
                    resolutions[NB_RESOLUTIONS],
                    pseudoConnection,
                    mdpConnection;

        sf::String choix[TAILLE_CHOIX_PAUSE],
                   statsPerso1[TAILLE_STATS],
                   statsPersoValues1[TAILLE_STATS],
                   statsPerso2[TAILLE_STATS-XP+2],
                   statsPersoValues2[TAILLE_STATS-XP+2],
                   scoresPerso[TAILLE_SCORES],
                   scoresPersoValues[TAILLE_SCORES],
                   nbObjetsInventaire[TAILLE_OBJETS_LOOT],
                   tabPlusMoinsTxt[NBSTATS-1],
                   optionsGenerales[TAILLE_OPTIONS_GENERALES],
                   optionsGeneralesValues[TAILLE_OPTIONS_GENERALES],
                   optionsPerso[TAILLE_OPTIONS_PERSO],
                   optionsPersoValues[TAILLE_OPTIONS_PERSO],
                   optionsSys[TAILLE_OPTONS_SYS],
                   optionsSysValues[TAILLE_OPTONS_SYS],
                   touchesPersoDesc[TAILLE_CTRL],
                   touchesPersoValues[TAILLE_CTRL],
                   touchesSysDesc[TAILLE_CTRLSYS],
                   touchesSysValues[TAILLE_CTRLSYS],
                   objetsLootables[TAILLE_OBJETS_LOOT],
                   titleScores,
                   titleInventaire,
                   titleSkills,
                   titleOption,
                   titleTouchesHeros,
                   titleTouchesSys,
                   txtCurrentSkill,
                   raccourcisTemp,
                   raccourcisObjTemp,
                   descObjTemp,
                   avertissementOptions,
                   upload,
                   pseudoConnectionStr,
                   mdpConnectionStr,
                   responseSiteStr,
                   txtNoSkills;

        sf::Font    font;

        sf::Image   imgBackPause,
                    imgCursor,
                    imgPortrait,
                    imgPortrait2,
                    imgRetour,
                    imgPrevious,
                    imgNext,
                    imgQuitter,
                    imgBottomSeparation,
                    imgInventaire[TAILLE_OBJETS_LOOT],
                    imgBoiteConnexion,
                    imgLogin,
                    imgResponseSite;

        sf::Sprite  sprBackPause,
                    sprCursor,
                    sprPortrait,
                    sprPortrait2,
                    sprRetour,
                    sprPrevious,
                    sprNext,
                    sprQuitter,
                    sprBottomSeparation,
                    inventaire[TAILLE_OBJETS_LOOT],
                    sprBoiteConnexion,
                    sprLogin;

        sf::Shape cursor;

        int     choixActuel,
                choixGauche,
                widgetGaucheX,
                separationDeuxTabs,
                idRaccourcisBSTemp,
                taillePolice,
                cptResolution,
                nbSkillsMaitrises,
                idReelSkillCourant;

        bool    touchesHerosMenued,
                touchesSysMenued,
                barreSortMenued,
                waitForTouche,
                waitForTouchePressed;

        unsigned int    compteurSkillReel,
                        compteurSkillDisplayed,
                        nbCharParLigne;

        Sons* son;
        _options* settings;
        _touches_heros* touches;
        _touches_sys* touchesSys;

        CURRENT_SCREEN screen;
};



#endif
