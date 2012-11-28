#ifndef GAME_HPP
#define GAME_HPP

//perso
#include "Taille_fenetre.hpp"
#include "Pause.hpp"
#include "Animated.hpp"
#include "Hero.hpp"
#include "Pnj.hpp"
#include "Objet.hpp"
#include "Pause.hpp"
#include "Parchemin.hpp"
#include "AnimRecharge.hpp"
#include "Sons.hpp"
#include "MenuSauvegarde.hpp"
#include "SaveStruct.hpp"
#include "Floor.hpp"
#include "AffichageTexte.hpp"
#include "Globale_entite_game.h"
#include "Global_taille_fenetre.hpp"
#include "marchand.hpp"
#include "SoundSpatialization.h"
#define ALPHA 120



class Game
{
    public :
        Game(sf::RenderWindow* Window, _options* settings, Sons* son, _touches_heros* touche, _touches_sys* touches_sy, statusJeu* ETA, int *level, _sauvegarde* loadGame=NULL, _saveHeros* oldHero=NULL);

        ~Game();
        void run();
        void show();
        void showgameover();
        void loadMap(const char* nomf,_sauvegarde* loadGame,_saveHeros* oldHero);
        void unpauseGame();
        void unpauseAfterDial();
        void Reset() {clock.Reset();Time=TEMPS_PAR_FRAME;};

        const bool getWaitForKey() { return waitForKey; };
        const bool getExitGame() { return exitGame; };
        const float getTotalGameTime() { return  clockTotalGameTime.GetElapsedTime() + totalGameTime; };
        void setWaitForKey(bool val) { waitForKey = val; };

        //héro
        Hero *perso;
        Pause *menupause;
        MenuSauvegarde *menusauvegarde;
        Parchemin *parchemin;

        sf::Vector2f posRespawn;

    private :
        //procédures
        void clavier_action(); //gestion des évènements claviers
        //void deplacements();
        void controleHorlogesPerso();
        void showGUI();     //dessiner le gui
        void showBonusMalus();  //affiche les bonus/malus si le perso gagne/perd de la vie
        void showBonusMalusMobs(std::vector<Pnj>::iterator mob_iter);  //affiche les bonus/malus des mobs si ils perdent de la vie
        void initGui(_sauvegarde* loadGame);     //initialiser
        void resizeGUI();   //redimansionner en fonction de la taille de la fenettre
        void majGUI();      //mise à jour de la gui (exp, mana et vie)
        inline void majStatsGui();
        inline void majStatsGuiSansResize();
        int nb_case_affiche; //nombre de case à afficher autour du perso
        void musicDeadFondu(int volumeMusique, int volumeDead);
        inline void pauseGame();
        inline void deadOfHero();
        inline void saveGame();
        void saveGameFirst(_sauvegarde *save);
        inline void saveGameHeros(_sauvegarde *save);
        inline void deadFondu();
        void loadPerso(_sauvegarde* loadGame);
        void launchDialogue(std::string text);
        inline void gestionDialoguesHeros();
        inline void loadImgBarreRaccourcisSkills();
        inline void gestionTransparenceGUI();
        inline void respawn();
        inline void saveForRespawn();
        inline void initCamera();

        //gestion des états du jeu
        inline void enPause();
        inline void enParchemin();
        inline void enSauvegarde();
        inline void enJeu();

        std::vector<sf::Image> resources_image_pnj_obj;
        std::vector<Pnj> mobs; //mobs
        std::vector<Objet> objets; //objets
        std::vector<sf::Image> resources_image; //images
        std::vector<Floor> tab_sols; //sols
        std::vector<_skills> vectorSkills; //skills
        std::vector<sf::Music*> effetsSonores; //sons des effets sonores
        std::vector<float> volumesEffetsSonores;

        //fenêtre
        sf::RenderWindow * App;
        _options* settings;
        PausableClock   clock,
                        clockTotalGameTime;
        ReverseClock    clockNiveaux,
                        clockLoadMap;
        AffichageTexte  *displayText;
        Marchand*       marchand;


        float   Time,
                coef_resize_x,
                coef_resize_y,
                totalGameTime;

        bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX],
             verticalSync,
             levelWithChrono,
             pause,
             waitForKey,
             headTransparent,
             exitGame;

        sf::Sprite  tab_sprite[MAPSIZECARTEY][MAPSIZECARTEX],
                    sprite;

        sf::View    camera;

        //interface
        sf::Image       back,
                        backLoad,
                        gui_img[TAILLE_GUI],
                        img_mainObjets,
                        gui_imgObjet[TAILLE_OBJETS_LOOT],
                        imgLivesMob,
                        imgBoss;

        sf::Sprite      SBack,
                        SBackLoad,
                        gui_spr[TAILLE_GUI],
                        sprLivesMob,
                        sprBoss;

        sf::String      level,
                        money,
                        gui_vies,
                        gui_statsNbr[TAILLE_GUI_STATS],
                        gui_time_lvl,
                        strPercent,
                        loading;

        sf::Music       musicLvl,
                        musicLvlCool,
                        musicDead;

        int             varQueteLvl,
                        alpha,
                        (*levelCourant);

        sf::Font        font,font2;

        Sons*           sounds;
        _touches_heros* touches;
        _touches_sys* touchesSys;

        statusJeu *ETAT;

        std::string nomf,
                    infosLevel[TAILLE_LEVEL];
};






#endif

