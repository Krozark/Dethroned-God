#ifndef HERO_HPP
#define HERO_HPP

#include <math.h>
#include <list>
#include "Objet.hpp"
#include "Pnj.hpp"
#include "ReverseClock.h"
#include "AnimRecharge.hpp"
#include "SoundSpatialization.h"
#include "Global_taille_fenetre.hpp"

#define MAX_TAILLE_OMBRE    10
#define DEGRADE_OMBRE 255/MAX_TAILLE_OMBRE

#define NB_ANIM_ATTAQUE 4
#define TEMPS_ENTRE_COUPS 1

class Hero : public Entite
{
    public :
        Hero(sf::Vector2f pos,_saveHeros *oldHero,Sons* son,_touches_heros* touches,statusJeu* ETA, std::vector<_skills>* vectorSkills);

        void    bouger_perso(sf::RenderWindow* Window,float time,int nombre_division,bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX],std::vector<Objet> *objets,std::vector<Pnj> *mobs);
        void    showAnimations(sf::RenderWindow *App);
        void    modifieVie(int nb);
        void    addVie(int nb);
        void    modifieMana(int nb);
        void    addMana(int nb);
        void    udpateCmd(_touches_heros* ctrl);
        void    calculeStats(); //applique les formules pour calculer les stats suivant les points des skills
        void    getCollisionObjets(std::vector<Objet> *objets);
        void    youAreDead();
        void    initStats(_saveHeros *oldHero);
        void    calculeXpLvlSvt();

        void respawn(sf::Vector2f posRespawn);

        const bool          GetDisplacePerso() {return displacePerso;};
        const int           getXp()   { return xpPerso[XP]; };
        const int           getXpMax() { return xpPerso[XPLEVEL]; };
        const int           getAnimFrappeNumber() { return animation_frappe_number; };
        const bool          getLvlFinish() { return lvlFinish; };
        const bool          getDead() { return DEAD; };
        const int           getSizeDeadAnim() { return animDeadLeft.Size(); };
        const bool          getAllowKeys() { return allowKeys; };
        const bool          getDisplay() { return display; };
        const bool          getMeetDialogue() { return meetDialogue; };
        const std::string   getMetDialogue() { return metDialogue; };
        const int           getVies() { return vies; };
        const bool          getBerzerk() { return berzerk; };
        int*        getStats() { return statsPerso; };
        int*        getScores() { return scoresPerso; };
        int*        getExp()   { return xpPerso; };
        bool*       getSkills() { return skillsPerso; };
        void        setLvlFinish(bool val) { lvlFinish = val; };
        void        setPortail(Objet *port) { portail = port; };
        void        setAnimFrappeNumber(int val) { animation_frappe_number=val; };
        void        setQuetteLvl(int val) { quetteLvl=val; };
        void        setQuette(int val) { quette=val; };
        void        SetDisplacePerso(bool val) {displacePerso=val;};
        void        setAllowKeys(bool val) {allowKeys=val;};
        void        setDisplay(bool val) {display=val;};
        void        setMeetDialogue(bool val) { meetDialogue = val; };
        void        setMetDialogue(std::string dial) { metDialogue = dial; meetDialogue = true; };
        void        regenClockSprint(const float time);
        void        setVies(int lifes) { vies = lifes; };
        void        setDead(bool d) { DEAD = d; };

        void collisionObjet(Objet &e);

        int tab_objets[TAILLE_OBJETS_LOOT];
        sf::String tab_nbobjets[TAILLE_OBJETS_LOOT];
        bool canUse[TAILLE_RACCOURCIS_BARRE_SORTS];

        ReverseClock    clock_frappe,
                        clock_regen_piege,
                        clockSprint,
                        clockStopSprint,
                        clockSurchauffe,
                        clock_larmes;

        PausableClock   clockVie,
                        clockMana;

        AnimRecharge    *gui_recharges[TAILLE_RACCOURCIS_BARRE_SORTS];

        std::vector<sf::String> bonusMalus;
        std::vector<float>  bonusMalusPosDepartY;
        std::vector<Objet> listeParchemins;

        int statsPerso[TAILLE_STATS];

        bool    operator ^(Entite &e),
                operator ^(Objet &e),
                skillsPerso[TAILLE_SKILLS];

        sf::Listener listener;

         Animated    animations[ANIM_TAILLE];


    private :
        void    initPerso();
        void    initStats();
        void    getEvent(sf::RenderWindow* Window);
        void    avance(AVANCE etat, float time);
        void    ramasseObj(std::vector<Objet> *objets, int cpt);
        void    utiliserObj(int obj);
        void    utiliserSkill(int skill);
        void    CollisionsMobs(std::vector<Pnj> *mobs);
        void    addXp(int xp);
        void    chargementAnimations();
        void    initAnimTemp();
        void    levelUp(int xp);
        void    mobDead(Pnj* pnj_iter);
        void    incQuette();
        inline void useSaveObject();
        inline void teleport();
        void    useGui(int nbCase);
        inline void generationObjetsCoffre(std::vector<Objet> *objets);
        inline void initObjectsGeneratedByChest();
        inline void playRecharge(int id, bool obj=true);
        void magieLarmesMilathea(std::vector<Pnj> *mobs);
        inline void enableBerzerkMode();


        sf::Image   imgGo[4],
                    imgRun1,
                    imgRun2,
                    imgRun3,
                    imgStand,
                    imgAttack1_1,
                    imgAttack1_2,
                    imgAttack2_1,
                    imgAttack2_2,
                    imgAttack3_1,
                    imgAttack3_2,
                    imgAttack3_3,
                    imgAttack4_1,
                    imgAttack4_2,
                    imgJump,
                    imgHeal,
                    imgMana,
                    imgDegats,
                    imgLevelup,
                    imgDead[5],
                    imgTeleport,
                    imgTeleportArrived,
                    imgObjectsGeneratedByChest,
                    img_recharge,
                    imgPtsComp,
                    imgMilathea1,
                    imgMilathea2,
                    imgMilathea3,
                    imgMilathea4,
                    imgMilathea5,
                    imgRespawn1,
                    imgRespawn2,
                    imgRespawn3;

        Anim    RunRight,
                RunLeft,
                StandRight,
                StandLeft,
                attackRight[NB_ANIM_ATTAQUE],
                attackLeft[NB_ANIM_ATTAQUE],
                JumpRight,
                JumpLeft,
                animHeal,
                animMana,
                animDegats,
                animLevelup,
                animDeadRight,
                animDeadLeft,
                animTeleport,
                animTeleportArrived,
                animPtsComp,
                animMilathea,
                animRespawn;

        std::string metDialogue;

        std::list<sf::Sprite> Ombre;    //liste des ombres du Hero
        std::vector<_skills>* skills;

        std::vector<Pnj*> mobsPeace;

        inline void Push_Back_Ombre(Sprite s);

        //sprites d'ombres
        std::vector<Sprite> sprRunRight,
                            sprRunLeft,
                            sprJumpRight,
                            sprJumpLeft;

        Objet   *portail,
                *objectsGeneratedByChest[TAILLE_OBJETS_LOOT];

        int     speedBorneMax,
                speedMin,
                speedMax,
                addRunSpeed,
                dep_dir,
                dep_touche,
                touche_droite,
                touche_gauche,
                animation_frappe_number,
                mouv,
                quetteLvl,
                berzerk_force,
                vies,
                xpPerso[TAILLE_EXP],
                scoresPerso[TAILLE_SCORES],
                chanceToLoot[TAILLE_OBJETS_LOOT];

        bool    key_left,
                key_right,
                displacePerso,
                RIEN,
                SAUTER,
                TAPER,
                RAMASSER,
                DEAD,
                alreadyHit,
                sonTapeAir1Played,
                lvlFinish,
                allowKeys,
                display,
                meetDialogue,
                touche_saut_is_pressed,
                touche_saut_was_pressed,
                milathea,
                second_saut,
                berzerk;

        Sons*   son;
        _touches_heros* ctrl;
        statusJeu* ETAT;

        sf::Font    font;

        AVANCE      avancement;

        sf::Key::Code   gauche,droite,courrir,taper,pause,sauter,ramasse;

        sf::Vector2f destinationTeleport;
};


#endif
