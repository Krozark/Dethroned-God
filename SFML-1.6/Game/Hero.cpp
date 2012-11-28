#include "Hero.hpp"

#include <sstream>
#include <math.h>
#define PORTEE_LARMES 800
#define PORTEE_LARMES2 PORTEE_LARMES*PORTEE_LARMES
#define VITESSE_OMBRE 350

using namespace sf;
using namespace std;

Hero::Hero(Vector2f pos,_saveHeros *oldHero,Sons* sons,_touches_heros* touches,statusJeu* ETA,vector<_skills>* vectorSkills) : Entite(pos)
{
    son = sons;
    ctrl = touches;
    ETAT = ETA;
    skills = vectorSkills;

    portail=NULL;
    //initialsiation du perso
    initPerso();

    //si on commence une nouvelle partie
    if (oldHero==NULL)
        initStats();
    else //si on etait déjà en partie ou si le hero existe déjà
        initStats(oldHero);

    srand(time(NULL));

    berzerk_force=3;
}


//************************************************************************
//      Initialisation du personnage
//************************************************************************
void Hero::initPerso()
{
    dep_dir=0;
    dep_touche=1;
    touche_droite=3;
    touche_gauche=5;
    mouv=200;
    addRunSpeed=100;
    quette = 0;
    animation_frappe_number = 0;
    mana=vie=viemax;
    vies=3;

    displacePerso=false;
    alreadyHit=false;
    sonTapeAir1Played = false;
    lvlFinish=false;
    DEAD = false;
    enable = true;
    allowKeys = true;
    display = true;
    meetDialogue = false;
    second_saut = true;
    milathea = false;
    touche_saut_is_pressed = touche_saut_was_pressed = true;
    berzerk = false;

    //chance de looter un objet x à partir d'un coffre
    chanceToLoot[OR_LOOT] = 100;
    chanceToLoot[POTION_VIE_LOOT] = 50;
    chanceToLoot[POTION_MANA_LOOT] = 50;
    chanceToLoot[PARCHEMIN_LOOT] = 0;
    chanceToLoot[SAUVEGARDES_LOOT] = 15;
    initObjectsGeneratedByChest();

    Resize(0.6,0.6);
    SetFrameTime(0.08);
    chargementAnimations();
    SetLoop(false);

    //Initialisation des objets
    font.LoadFromFile("./fonts/AngelicWar.ttf");

    //on d�finie une animation de recharge
    int temps=TEMPS_REGEN_POPO;//temps de recharge
    img_recharge.LoadFromFile("./resources/GUI/recharge.png");
    for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
        gui_recharges[i] = new AnimRecharge(img_recharge,8,temps);

    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
    {
        tab_objets[i]=0;
        tab_nbobjets[i].SetText("0");
        tab_nbobjets[i].SetColor(Color(255,255,255));
        tab_nbobjets[i].SetSize(15);
    }

    //Initialistion des données propres aux enchaînements de coups
    clock_frappe.SetTime(TEMPS_ENTRE_COUPS);
    clock_frappe.Stop();
};

//////////////////////////////////////////////////////////////////////////////////////////////////
///initialisation d'un tableau contenant des objets lootables simples à faire sortir des coffres
//////////////////////////////////////////////////////////////////////////////////////////////////
inline void Hero::initObjectsGeneratedByChest()
{
    imgObjectsGeneratedByChest.LoadFromFile("./resources/image/allItems.png");

    //or
    _OBJ_ obj;
    obj.tx = 4;
    obj.ty = 11;
    obj.direction = (_type_direction)OR;
    obj.type = OR;
    obj.x = GetPosition().x;
    obj.y = GetPosition().y;
    obj.quette = obj.idSkill = obj.destx = obj.desty = 0;
    obj.dialogue[0] = '\0';
    objectsGeneratedByChest[OR_LOOT] = new Objet(imgObjectsGeneratedByChest,obj);

    //potion vie
    obj.direction = (_type_direction)POTION_VIE;
    obj.type = POTION_VIE;
    objectsGeneratedByChest[POTION_VIE_LOOT] = new Objet(imgObjectsGeneratedByChest,obj);

    //potion mana
    obj.direction = (_type_direction)POTION_MANA;
    obj.type = POTION_MANA;
    objectsGeneratedByChest[POTION_MANA_LOOT] = new Objet(imgObjectsGeneratedByChest,obj);

    //parchemin
    obj.direction = (_type_direction)PARCHEMIN;
    obj.type = PARCHEMIN;
    objectsGeneratedByChest[PARCHEMIN_LOOT] = new Objet(imgObjectsGeneratedByChest,obj);

    //sauvegarde
    obj.direction = (_type_direction)SAUVEGARDE;
    obj.type = SAUVEGARDE;
    objectsGeneratedByChest[SAUVEGARDES_LOOT] = new Objet(imgObjectsGeneratedByChest,obj);

    for (int i=0;i<TAILLE_OBJETS_LOOT;++i)
    {
        objectsGeneratedByChest[i]->SetFrottement(frottement);
        objectsGeneratedByChest[i]->SetGravite(gravitee);
    }
};

void Hero::initStats()
{
    //stats
    statsPerso[NB_POINTS_SKILLS]=0;
    statsPerso[ATTAQUE]=10;
    statsPerso[DEFENSE]=1;
    statsPerso[VITESSE]=150;
    statsPerso[ENDURANCE]=10;
    statsPerso[INTELLIGENCE]=10;
    statsPerso[LEVEL]=1;
    calculeStats();
    vie=viemax;
    mana=manamax;

    //expérience
    calculeXpLvlSvt();
    xpPerso[XP]=0;

    //scores
    scoresPerso[NB_MOBS_KILL]=0;
    scoresPerso[NB_MOBS_BOSS_KILL]=0;
    scoresPerso[XP_TOTAL]=0;
    scoresPerso[VIE_LOSE]=0;
    scoresPerso[MANA_LOSE]=0;
    scoresPerso[DEGATS_TOTAL]=0;
    scoresPerso[DEGATS_BEST]=0;

    //initialisation des skills
    for (int i=0; i<TAILLE_SKILLS; ++i)
        skillsPerso[i] = false;
}


void Hero::initStats(_saveHeros *oldHero)
{
    //chargement des stats
    for (int i=0; i<TAILLE_STATS; ++i)
        statsPerso[i] = oldHero->statsPerso[i];
    vie = oldHero->vie;
    mana = oldHero->mana;
    calculeStats();

    //chargement de l'exp
    calculeXpLvlSvt();
    xpPerso[XP]=oldHero->xpPerso[XP];

    //chargement des scores
    for (int i=0; i<TAILLE_SCORES; ++i)
        scoresPerso[i] = oldHero->scoresPerso[i];

    stringstream out;
    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
    {
        tab_objets[i] = oldHero->tab_objets[i];
        out << tab_objets[i];
        tab_nbobjets[i].SetText(out.str());
        out.str(string());
    }

    //initialisation des skills
    for (int i=0; i<TAILLE_SKILLS; ++i)
        skillsPerso[i] = oldHero->tabSkills[i];
}


void Hero::calculeStats()
{
    viemax = pow(statsPerso[ENDURANCE],1.72) * 2.57 - 34;
    manamax = pow(statsPerso[INTELLIGENCE],1.72) * 2.57 - 34;

    speedMin = statsPerso[VITESSE];
    speedMax = statsPerso[VITESSE] + 100;
    speedBorneMax = statsPerso[VITESSE] + 200;
}


void Hero::calculeXpLvlSvt()
{
    xpPerso[XPLEVEL]=1.25*pow(statsPerso[LEVEL]+1,3);
}


void Hero::addXp(int xp)
{
    //on ajout au score d'xp gagné au total
    scoresPerso[XP_TOTAL]+=xp;

    int xpManquant = xpPerso[XPLEVEL]-xpPerso[XP];

    //si on a level up
    if (xpManquant < xp)
    {
        while (xp > xpManquant)
        {
            xp -= xpManquant;
            levelUp(xpManquant);
            calculeXpLvlSvt();
            xpManquant = xpPerso[XPLEVEL]-xpPerso[XP];
        }

        //lecture du son
        son->playSon(LVLUP);

        //reset vie mana
        setVie(viemax);
        setMana(manamax);
    }
    else
        xpPerso[XP]+=xp;

}


inline void Hero::levelUp(int xp)
{
    ++(statsPerso[LEVEL]); //on augmente le level
    statsPerso[NB_POINTS_SKILLS]+=5; //on donne 5 skills points au perso
    xpPerso[XP]=0;
    animations[ANIM_LVLUP].Play(); //on joue l'animation
}

void Hero::modifieVie(int nb)
{
    //pour que les monstres ne s'acharnent pas sur un cadavre
    if (!DEAD)
    {
        vie += nb;

        //si ce n'est pas de la regen
        if (nb!=0)
        {
            stringstream out;
            String txt;

            //bonus
            if (nb>0)
            {
                txt.SetColor(Color(0,255,0));
                out << "+";
            }
            else //malux
            {
                //epic fail
                if (abs(nb) >= viemax)
                    son->playSon(EPIC_FAIL);

                txt.SetColor(Color(255,0,128));
                scoresPerso[VIE_LOSE]+=nb; //ajout au score
                //out << "-";
            }
            out << nb;

            txt.SetText(out.str());
            txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
            bonusMalus.push_back(txt);
            bonusMalusPosDepartY.push_back(GetPosition().y);
        }

        if      (vie<=0)      vie=0;
        else if (vie>viemax) vie=viemax;
    }
};

void Hero::addVie(int nb)
{
    if (nb>0)
    {
        vie+=nb;
        if (vie>viemax) vie=viemax;
    }
};

void Hero::addMana(int nb)
{
    if (nb>0)
    {
        mana+=nb;
        if (mana>manamax) mana=manamax;
    }
};

void Hero::modifieMana(int nb)
{
    mana += nb;

    //si ce n'est pas de la regen
    if (nb!=0)
    {
        stringstream out;
        String txt;

        //bonus
        if (nb>0)
        {
            txt.SetColor(Color(0,0,255));
            out << "+";
        }
        else //malux
        {
            txt.SetColor(Color(0,0,255));
            scoresPerso[MANA_LOSE]+=nb; //ajout au score
            //out << "-";
        }
        out << nb;

        txt.SetText(out.str());
        txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
        bonusMalus.push_back(txt);
        bonusMalusPosDepartY.push_back(GetPosition().y);
    }

    if      (mana<0)         mana=0;
    else if (mana>manamax)   mana=manamax;
}


void Hero::bouger_perso(RenderWindow* Window,float time,int nombre_division,bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX],std::vector<Objet> *objets,std::vector<Pnj> *mobs)
{
    displacePerso = true;
    avancement = NUL;
    SAUTER = TAPER = RIEN = RAMASSER = false;

    //on détermine ce que fait le perso en ce moment si il est vivant
    if (!DEAD  and  allowKeys)
        getEvent(Window);

    // on traite l'état actuel du perso
    if (RAMASSER)
        getCollisionObjets(objets);


    //si il saute, on applique la gravité
    if (SAUTER)
    {
        if (!TAPER)
        {
            if (direction == DROITE)
            {
                if (GetAnim()!=&JumpRight)
                    SetAnim(&JumpRight);
            }
            else
            {
                if (GetAnim()!=&JumpLeft)
                    SetAnim(&JumpLeft);
            }
        }

        if (getFloorCollision() or (second_saut and !touche_saut_was_pressed and touche_saut_is_pressed and skillsPerso[SKILL_DOUBLE_SAUT]))
        {
            second_saut = getFloorCollision();
            SetForceY(-400);
        }
        else if (!TAPER)
        {
            if (m_vector.y < oldPos.y) {
                SetFrame(0);
            }
            else if (m_vector.y >= oldPos.y)  {
                SetFrame(1);
            }
        }
    }


    //S'il tape, on tape dans la bonne direction
    if (TAPER)
    {
        //SetFrameTime(0.1);

        if (avancement == NUL)
            SetForce(sf::Vector2f(0,getForce().y));

        bool modif=false;
        if (direction == DROITE  and  GetAnim()!=&attackRight[animation_frappe_number])
        {
            modif=true;
            SetAnim(&attackRight[animation_frappe_number]);
        }
        else if (direction == GAUCHE  and  GetAnim()!=&attackLeft[animation_frappe_number])
        {
            modif=true;
            SetAnim(&attackLeft[animation_frappe_number]);
        }

        if (modif)
        {
            if (!sonTapeAir1Played) son->playSon(TAPE_AIR1);
            else son->playSon(TAPE_AIR2);
            sonTapeAir1Played = !sonTapeAir1Played;
        }


        if ((unsigned int) GetCurrentFrame() == GetAnim()->Size()-1)
        {
              clock_frappe.Replay();
        }

        if ((unsigned int)GetCurrentFrame() == GetAnim()->Size()-1  and  !alreadyHit)
        {
            CollisionsMobs(mobs);
            alreadyHit=true;
        }
        else if (alreadyHit && (unsigned int)GetCurrentFrame() != GetAnim()->Size()-1)
            alreadyHit=false;
    }


    //S'il marche ou court, on l'avance
    if (avancement != NUL)
        avance(avancement,time*nombre_division);
    else
    {
        regenClockSprint(time*nombre_division);
    }


    //lecture de l'animation
    if (IsPaused()  and  !DEAD) Play();

    if (SAUTER  and  !TAPER)
        Pause();

    //si il est en téléportation
    if (!animations[ANIM_TELEPORT].IsPaused())
    {
        if (animations[ANIM_TELEPORT].GetCurrentFrame()>=7  and  isEnable())
            display=false;
        if (animations[ANIM_TELEPORT].GetCurrentFrame()>=animations[ANIM_TELEPORT].Size()-1)
            teleport();
    }
    else if (!animations[ANIM_TELEPORT_ARRIVED].IsPaused())
    {
        if (animations[ANIM_TELEPORT_ARRIVED].GetCurrentFrame()>=7  and  isEnable())
            display=true;
    }

    //S'il ne fait rien, on ne l'anime pas
    if (RIEN)
    {
        Pause();
        if (direction == GAUCHE)
            SetAnim(&StandLeft);
        else
            SetAnim(&StandRight);
        displacePerso=false;
        //SetFrameTime(0.08);
    }

    anim(time);


    if((!displacePerso  and  getFloorCollision())) //si le perso touche/vient de toucher le sol
        Stopping_Frotement(time);


    if (!DEAD)
        Update(time,nombre_division, tab_collision);

    //ajout des ombres
    if (berzerk)
        SetColor(Color::Red);
    if ((abs((int)m_vector.x) >= VITESSE_OMBRE) or (abs((int)m_vector.y) >= VITESSE_OMBRE))
        Push_Back_Ombre(*this);
    else if (Ombre.size() >0)
        Ombre.pop_front();

    if(GetPosition().y>=MAPSIZEY-TAILLEBLOC-10)
        DEAD=true;

    if (milathea)
        magieLarmesMilathea(mobs);
    if (clock_larmes.GetRemainingTime() <= 0)
    {
        for (std::vector<Pnj*>::iterator i=mobsPeace.begin(); i!=mobsPeace.end(); ++i)
            (*i)->SetAgressif(true);
        mobsPeace.clear();
        animations[ANIM_MILATHEA].Stop();
    }
}


void Hero::getEvent(RenderWindow* Window)
{
    const sf::Input& Input = Window->GetInput();

    if (Input.IsKeyDown(ctrl->droite)) {
        if (GetAnim()!=&attackRight[animation_frappe_number]  and  GetAnim()!=&attackLeft[animation_frappe_number]) {
            direction = DROITE;
        }
        else
            m_vector.x = 0;
        avancement = MARCHER;
    }
    if (Input.IsKeyDown(ctrl->gauche)) {
        if (GetAnim()!=&attackRight[animation_frappe_number]  and  GetAnim()!=&attackLeft[animation_frappe_number]) {
            direction = GAUCHE;
        }
        else
            m_vector.x = 0;
        avancement = MARCHER;
    }
    if (Input.IsKeyDown(ctrl->ramasse))
        RAMASSER=true;

    int percent = (clockSprint.GetRemainingTime()==-1) ? 0 : (int)((100*((*skills)[SKILL_COURIR].duree-clockSprint.GetRemainingTime()))/(*skills)[SKILL_COURIR].duree);
    if (percent>=99  and  clockSurchauffe.GetRemainingTime()==-1)
        clockSurchauffe.SetTime(SPRINT_SURCHAUFFE);
    if (Input.IsKeyDown(ctrl->courrir)  and  avancement==MARCHER  and  percent<99  and  clockSurchauffe.GetRemainingTime()==-1 and skillsPerso[SKILL_COURIR])
    {
        //controle du temps de sprint
        avancement=COURIR;
    }
    if ((Input.IsKeyDown(ctrl->sauter) and skillsPerso[SKILL_SAUT])  or  ((GetAnim()==&JumpLeft  or  GetAnim()==&JumpRight)  and  !getFloorCollision()))
        SAUTER=true;
    // Pour vérifier que le héros ne fasse pas ses deux sauts en deux frames suivantes
    touche_saut_was_pressed = touche_saut_is_pressed;
    touche_saut_is_pressed = Input.IsKeyDown(ctrl->sauter);
    if ((GetAnim()==&attackLeft[animation_frappe_number]  and  !IsPaused())  or  (GetAnim()==&attackRight[animation_frappe_number]  and  !IsPaused())  or  ((Input.IsKeyDown(ctrl->taper)) and skillsPerso[SKILL_FRAPPE]))
    {
        TAPER = true;
        if ((clock_frappe.IsPlaying()))
        {
            if (Input.IsKeyDown(ctrl->taper))
            {
                ++animation_frappe_number;
                clock_frappe.Stop();
                if (animation_frappe_number == NB_ANIM_ATTAQUE)
                    animation_frappe_number = 0;
            }
        }
    }
    else if (!clock_frappe.IsPlaying())
        animation_frappe_number=0;

    //vol
    if (Input.IsKeyDown(Key::M))
        AddForce(Vector2f(0,-100));
    if (Input.IsKeyDown(Key::P))
        modifieVie(-5);

    //barre de raccourcis
    if (Input.IsKeyDown(ctrl->gui1))
        useGui(1);
    if (Input.IsKeyDown(ctrl->gui2))
        useGui(2);
    if (Input.IsKeyDown(ctrl->gui3))
        useGui(3);
    if (Input.IsKeyDown(ctrl->gui4))
        useGui(4);
    if (Input.IsKeyDown(ctrl->gui5))
        useGui(5);
    if (Input.IsKeyDown(ctrl->gui6))
        useGui(6);
    if (Input.IsKeyDown(ctrl->gui7))
        useGui(7);
    if (Input.IsKeyDown(ctrl->gui8))
        useGui(8);
    if (Input.IsKeyDown(ctrl->gui9))
        useGui(9);
    if (Input.IsKeyDown(ctrl->gui10))
        useGui(10);

    if (avancement==NULL  and  !TAPER  and  !SAUTER  and  !DEAD)
        RIEN=true;
};


void Hero::useGui(int nbCase)
{
    nbCase--;

    if (ctrl->tabBarreSortsId[nbCase] > -1) //si le raccourcis n'est pas vide
    {
        if (ctrl->tabBarreSortsObj[nbCase]) //c'est un objet
        {
            canUse[nbCase] = (gui_recharges[nbCase]->IsPaused() and tab_objets[ctrl->tabBarreSortsId[nbCase]]>0) ? true : false;

            if (canUse[nbCase])
                utiliserObj(ctrl->tabBarreSortsId[nbCase]); //on l'utilise
        }
        else //un skin
        {
            canUse[nbCase] = (gui_recharges[nbCase]->IsPaused() and (getMana() >= (*skills)[ctrl->tabBarreSortsId[nbCase]].mana)) ? true : false;
            if (canUse[nbCase])
                utiliserSkill(ctrl->tabBarreSortsId[nbCase]);
        }

    }
};


void Hero::avance(AVANCE etat, float time)
{
    //on modifie la vitesse suivant si on court ou pas
    if (etat == COURIR)
    {
        if (speedMax<speedBorneMax)
            speedMax = speedMax+addRunSpeed;
        if (speedMin<speedBorneMax-addRunSpeed)
            speedMin = speedMin+addRunSpeed;

        //horloge de sprint
        if (clockSprint.IsPaused())
        {
            if (clockSprint.GetRemainingTime()==-1)
                clockSprint.SetTime((*skills)[SKILL_COURIR].duree);
            else
                clockSprint.Play();
        }
    }
    else
    {
        if (speedMax==speedBorneMax)
            speedMax = speedMax-addRunSpeed;
        if (speedMin==speedBorneMax-addRunSpeed)
            speedMin = speedMin-addRunSpeed;

        regenClockSprint(time);
    }

    //on applique la vitesse au perso suivant la direction
    if (direction == DROITE)
    {
        if(getForce().x < speedMax)
        {
            if (getFloorCollision()  and  getForce().x < speedMin)
                m_vector.x = speedMin;
            else if(getFloorCollision())
                AddForce(sf::Vector2f (time * 1000, 0));
            else
                AddForce(sf::Vector2f (time * 500, 0));
        }
        else
            m_vector.x = speedMax;

        if (!TAPER  and  !SAUTER)
        {
            if (etat==MARCHER  and  GetAnim()!=&GoRight)
                SetAnim(&GoRight);
            else if (etat==COURIR  and  GetAnim()!=&RunRight )
                SetAnim(&RunRight);
        }
    }
    else
    {
        if(getForce().x > -speedMax)
        {
            if(getFloorCollision()  and  getForce().x > -speedMin)
                m_vector.x = -speedMin;
            else if (getFloorCollision())
                AddForce(sf::Vector2f (-time * 1000, 0));
            else
                AddForce(sf::Vector2f (-time * 500, 0));
        }
        else
            m_vector.x = -speedMax;

        if (!TAPER  and  !SAUTER)
        {
            if (etat==MARCHER  and  GetAnim()!=&GoLeft)
                SetAnim(&GoLeft);
            else if (etat==COURIR  and  GetAnim()!=&RunLeft)
                SetAnim(&RunLeft);
        }
    }

    listener.SetPosition(GetPosition().x, GetPosition().y,0);
}

inline void Hero::regenClockSprint(const float time)
{
    if (clockSprint.GetRemainingTime() <= (*skills)[SKILL_COURIR].duree)
    {
        if (clockSurchauffe.GetRemainingTime() <= 0.5)
        {
            clockSprint.SetTime(clockSprint.GetRemainingTime()+time*(((float)(*skills)[SKILL_COURIR].duree)/((float)(*skills)[SKILL_COURIR].recharge)));
        }
        if (clockSprint.IsPlaying())
            clockSprint.Pause();
    }
    else
    {
        if (!clockSprint.IsPaused())
            clockSprint.Stop();
    }
}

void Hero::udpateCmd(_touches_heros* newCtrl)
{
    ctrl = newCtrl;
}

//********************************************************************
//  On vérifie s'il y a un mob à portée quand on tape
//********************************************************************
void Hero::CollisionsMobs(std::vector<Pnj> *mobs)
{
    unsigned int _size=mobs->size();

    for (unsigned int i=0;i<_size;++i)
    {
        if ((*mobs)[i].isEnable())
        {
            if ((*this)^(*mobs)[i])
            {
                mobDead(&(*mobs)[i]);
                i=_size;
            }
        }
    }
}

//Mort du mob s'il n'a plus de vie
void Hero::mobDead(Pnj* pnj_iter)
{
    //epic fail
    if (Randomizer::Random(1,100) <= 80+(-pow(pnj_iter->getLvl()-statsPerso[LEVEL],3))/14)
    {
        int attaque = statsPerso[ATTAQUE];
        int borne = attaque/3;
        int forceMax = attaque+borne;
        int forceMin = attaque-borne/2.0;
        int degats = rand()%((forceMax+1)-forceMin) + forceMin;
        bool critique = (degats>=(forceMax-(forceMax/9))) ? true : false;
        bool epicFail = (degats<=(forceMin+(forceMin/9))) ? true : false;

        //scores
        scoresPerso[DEGATS_TOTAL]+=degats;
        if (degats >= scoresPerso[DEGATS_BEST])
            scoresPerso[DEGATS_BEST]=degats;

        pnj_iter->modifieVie(-degats,critique,epicFail);
        animations[ANIM_MOBS_DEGATS].Play();

        //si on tue un monstre
        if (pnj_iter->getVie() <= 0)
        {
            if (pnj_iter->getQuette() > 0) //monstre boss tué
            {
                ++(scoresPerso[NB_MOBS_BOSS_KILL]);
                incQuette();

                //affichage du level Up
                String txt("Monstre Boss Tue");
                txt.SetColor(Color(66,0,255));
                txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
                bonusMalus.push_back(txt);
                bonusMalusPosDepartY.push_back(GetPosition().y);

                son->playSon(BOSS_TUE);
            }

            //dialogue
            if (pnj_iter->GetDialogue() != "")
            {
                meetDialogue = true;
                metDialogue = pnj_iter->GetDialogue();
            }

            ++scoresPerso[NB_MOBS_KILL];
            addXp(pnj_iter->getExp());
        }

        //lecture du son
        son->playSon(TAPE_MOB1+animation_frappe_number); //joue le son correspondant à l'animation de frappe en cours
    }
    else
    {
        //affichage du level Up
        String txt("Miss");
        txt.SetColor(Color(180,180,180));
        txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
        bonusMalus.push_back(txt);
        bonusMalusPosDepartY.push_back(GetPosition().y);
        son->playSon(MISS_MOB);
    }
}

void Hero::getCollisionObjets(std::vector<Objet> *objets)
{
    for(unsigned int i=0;i<objets->size();++i)
        /*if (((GetPosition().x - GetSize().x/2) <= (*objets)[i].GetPosition().x)  and ((GetPosition().x + GetSize().x/2) >= (*objets)[i].GetPosition().x))
            if ((int)(GetPosition().y/TAILLEBLOC) == (int)(*objets)[i].GetPosition().y/TAILLEBLOC)*/
            if((*this)^(*objets)[i])
                ramasseObj(objets,i);
}


void Hero::ramasseObj(std::vector<Objet> *objets, int cpt)
{
    if ((*objets)[cpt].isEnable())
    {
        bool sond_ever_made=false;      //pour la redefinition des sons
        _type_objets type=(*objets)[cpt].getType(); //pour objets normaux
        int typeLootable;

        //ici on détermine quel est le type de l'objet LOOTABLE pour incrémenter la valeur dans l'inventaire du héros
        if (type==POTION_VIE)
            typeLootable=POTION_VIE_LOOT;
        else if (type==POTION_MANA)
            typeLootable=POTION_MANA_LOOT;
        else if (type==SAUVEGARDE)
            typeLootable=SAUVEGARDES_LOOT;
        else
            typeLootable=PARCHEMIN_LOOT;


        switch(type)
        {
            case POTION_VIE:
            case POTION_MANA:
            case SAUVEGARDE:
            {
                if (tab_objets[typeLootable] < NB_PAR_OBJETS)
                {
                    ++(tab_objets[typeLootable]);
                    stringstream out;
                    out<<tab_objets[typeLootable];
                    tab_nbobjets[typeLootable].SetText(out.str());
                    if (!sond_ever_made) son->playSon(POTIONRAMASSE);        //si on avais pas ramasser de parchemin
                    (*objets)[cpt].setEnable(false);
                }
             }break;

            case COFFRE:
            {
                (*objets)[cpt].setEnable(false);
                generationObjetsCoffre(objets);
            }break;

            case PARCHEMIN:
                typeLootable=PARCHEMIN_LOOT;
                //si il est visible
                if ((*objets)[cpt].getQuette() <= quette+1)
                {
                    addXp((*objets)[cpt].getExperience());
                    listeParchemins.push_back((*objets)[cpt]);
                    sond_ever_made=true;
                    incQuette();
                    if ((*objets)[cpt].getIdSkill()>0)
                        skillsPerso[(*objets)[cpt].getIdSkill()-1]=true;
                    utiliserObj(PARCHEMIN);
                    ++(tab_objets[typeLootable]);
                    stringstream out;
                    out<<tab_objets[typeLootable];
                    tab_nbobjets[typeLootable].SetText(out.str());
                    if (!sond_ever_made) son->playSon(POTIONRAMASSE);        //si on avais pas ramasser de parchemin
                    (*objets)[cpt].setEnable(false);
                }break;

             case SAUVEGARDE_FIX :
             {
                 //on regen le heros
                 modifieVie(viemax-vie);
                 modifieMana(manamax-mana);

                 //on utilise l'objet de sauvegarde
                 useSaveObject();
             }break;

             case TELEPORTEUR :
             {
                 //on joue l'animation
                animations[ANIM_TELEPORT].Play();
                //son
                son->playSon(TELEPORT_ARRIVED);
                //on enlève toute force
                SetForce(Vector2f(0,0));
                //on stocke la destination
                destinationTeleport = (*objets)[cpt].GetDestination();
             }break;

            case TELEPORTEUR_QUETTE :
            {
                //si il est visible
                if ((*objets)[cpt].getQuette() <= quette+1)
                {
                     //on joue l'animation
                    animations[ANIM_TELEPORT].Play();
                    //son
                    son->playSon(TELEPORT_ARRIVED);
                    //on enlève toute force
                    SetForce(Vector2f(0,0));
                    //on stocke la destination
                    destinationTeleport = (*objets)[cpt].GetDestination();
                }
            }break;

            case OR:
            {
                //on ajoute l'or à l'inventaire
                //cout << objets->getOr() << endl;
                tab_objets[OR_LOOT] += (*objets)[cpt].getOr();
                stringstream out;
                out << tab_objets[OR_LOOT];
                tab_nbobjets[OR_LOOT].SetText(out.str());

                //on joue le son
                son->playSon(SON_OR);

                //on affiche le nmobre d'or gagné
                out.str(string());
                out << "+" << (*objets)[cpt].getOr();
                String txt(out.str());
                txt.SetColor(Color::Green);
                txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
                bonusMalus.push_back(txt);
                bonusMalusPosDepartY.push_back(GetPosition().y);

                (*objets)[cpt].setEnable(false);

            }break;

            case PORTAIL:
                if (quette >= quetteLvl)
                {
                    setLvlFinish(true);
                }
                break;

            case MARCHAND :
            {
                (*ETAT)=MARCHANDISE;
            }break;

            case OBJET_QUETTE:
            {
//                cout << "objet quette" << endl;
                addXp((*objets)[cpt].getExperience());
                break;
            }

            case OBJET_RIEN:
            {
                string dial="";
                if ((dial=(*objets)[cpt].GetDialogue())!="")
                    setMetDialogue(dial);
            }break;




            default:
                cout << "What is Daz?" << endl;
                break;
        }

    }
};



////////////////////////////////////////////////////////////////////////
/// Génère des objets aléatoirement après l'utilisation d'un coffre
////////////////////////////////////////////////////////////////////////
inline void Hero::generationObjetsCoffre(std::vector<Objet> *objets)
{
    //on parcours les objets lootables
    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
    {
        //si le coffre à délivrer l'objet courant
        int percent = Randomizer::Random(1,100);

        if (percent < chanceToLoot[i])
        {
            //on trouve lequel c'est
            if (i == OR_LOOT)
                objectsGeneratedByChest[i]->setOr(Randomizer::Random(15,255));

            objets->push_back(*objectsGeneratedByChest[i]);
            (*objets)[objets->size()-1].SetPosition(GetPosition().x,GetPosition().y-100);
            (*objets)[objets->size()-1].AddForce(Vector2f(Randomizer::Random(-150,150),-Randomizer::Random(150,250)));
        }
    }
};

void Hero::utiliserSkill(int skill)
{
    modifieMana(-(*skills)[skill].mana);
    playRecharge(skill,false);

    switch (skill)
    {
        case SKILL_SOIN:
            modifieVie(100);
            son->playSon(POTIONUTIL);
            animations[ANIM_HEAL].Play();
            break;

        case SKILL_CHANT_DE_MILATHEA:
            milathea=true;
            animations[ANIM_MILATHEA].Play();
            son->playSon(SON_MILATHEA);
            break;

        case SKILL_BERZERK:
            berzerk = !berzerk;
            enableBerzerkMode();
            break;

        default:
            break;
    }
};

void Hero::enableBerzerkMode()
{
    if (berzerk)
    {
        Resize(GetSize().x*1.5,GetSize().y*1.5);
        SetColor(Color::Red);
    }
    else
    {
        Resize(GetSize().x/1.5,GetSize().y/1.5);
        SetColor(Color::Black);
    }
}

void Hero::magieLarmesMilathea(vector<Pnj> *mobs)
{
    unsigned int _size = mobs->size();
    for (unsigned int i = 0 ; i < _size ; ++i)
    {
        if (!(*mobs)[i].isBoss())
        {
            // Teste la présence des monstres dans un cercle de rayon PORTEE_LARMES autour du héros
            // S'ils sont présents, on met leur agressivité à false
            //float pos=(*mobs)[i].GetPosition().x-GetPosition().x*(*mobs)[i].GetPosition().x-GetPosition().x + (*mobs)[i].GetPosition().y-GetPosition().y*(*mobs)[i].GetPosition().y-GetPosition().y;
           /* if (pos < PORTEE_LARMES2)
            {*/
                (*mobs)[i].SetAgressif(false);
                mobsPeace.push_back(&((*mobs)[i]));
            //}

        }
    }
    clock_larmes.SetTime((*skills)[SKILL_CHANT_DE_MILATHEA].duree);
    milathea=false;
}

void Hero::utiliserObj(int obj)
{
    switch (obj)
    {
        case POTION_VIE_LOOT:
        {
            tab_objets[POTION_VIE_LOOT]--;
            stringstream out;
            out<<tab_objets[POTION_VIE_LOOT];
            tab_nbobjets[POTION_VIE_LOOT].SetText(out.str());
            modifieVie(50);
            playRecharge(obj);
            son->playSon(POTIONUTIL);
            animations[ANIM_HEAL].Play();
         }break;

        case POTION_MANA_LOOT:
        {
            tab_objets[POTION_MANA_LOOT]--;
            stringstream out;
            out<<tab_objets[POTION_MANA_LOOT];
            tab_nbobjets[POTION_MANA_LOOT].SetText(out.str());
            modifieMana(50);
            if (getMana()>manamax)
                setMana(manamax);
            playRecharge(obj);
            son->playSon(POTIONUTIL);
            animations[ANIM_MANA].Play();
        }break;

        case PARCHEMIN_LOOT:
        {
            son->playSon(PARCHEMIN_UTIL);
            //afficheParchemin=true;
            (*ETAT) = PARCHEMINED;
        }break;

        case SAUVEGARDES_LOOT:
        {
            tab_objets[SAUVEGARDES_LOOT]--;
            stringstream out;
            out<<tab_objets[SAUVEGARDES_LOOT];
            tab_nbobjets[SAUVEGARDES_LOOT].SetText(out.str());
            playRecharge(obj);
            useSaveObject();
        }break;

        default :
            cout << "Utilisation d'objet inconnu" << endl;
            break;
    }
};

inline void Hero::playRecharge(int id,bool obj)
{
    //on parcours la barre de racourcis
    for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
    {
        //on a trouvé l'objet/sort à recharger
        if (ctrl->tabBarreSortsId[i] == id and ctrl->tabBarreSortsObj[i] == obj)
        {
            if (!ctrl->tabBarreSortsObj[i]) //si c'est un sort il a un temps de recharge spécial
                gui_recharges[i]->setTime((*skills)[id].recharge);
            gui_recharges[i]->Play();
        }
    }
};


inline void Hero::useSaveObject()
{
    son->playSon(SAVE);
    (*ETAT) = SAVED;
};


///*******************************************************
///      Fonctions abstraites de Entite
///*******************************************************
void Hero::chargementAnimations()
{
    //animations de déplacements
    //marche
    imgGo[0].LoadFromFile("images/heros/walking-1.png");
    imgGo[1].LoadFromFile("images/heros/walking-2.png");
    imgGo[2].LoadFromFile("images/heros/walking-3.png");
    imgGo[3].LoadFromFile("images/heros/walking-4.png");
    loadAnim(imgGo[0], 2, 2, &GoRight, &GoLeft);
    loadAnim(imgGo[1], 2, 2, &GoRight, &GoLeft);
    loadAnim(imgGo[2], 2, 2, &GoRight, &GoLeft);
    loadAnim(imgGo[3], 1, 2, &GoRight, &GoLeft);

    //sprint
    imgRun1.LoadFromFile("images/heros/Run-1.png");
    imgRun2.LoadFromFile("images/heros/Run-2.png");
    imgRun3.LoadFromFile("images/heros/Run-3.png");
    loadAnim(imgRun1, 2, 2, &RunRight, &RunLeft);
    loadAnim(imgRun2, 2, 2, &RunRight, &RunLeft);
    loadAnim(imgRun3, 2, 2, &RunRight, &RunLeft);

    //debout
    imgStand.LoadFromFile("images/heros/stand.png");
    loadAnim(imgStand, 1, 2, &StandRight, &StandLeft);

    //saut
    imgJump.LoadFromFile("images/heros/jump.png");
    loadAnim(imgJump, 2, 2, &JumpRight, &JumpLeft);

    //animations de frappe
    //frappe 1
    imgAttack1_1.LoadFromFile("images/heros/attack1-1.png");
    imgAttack1_2.LoadFromFile("images/heros/attack1-2.png");
    loadAnim(imgAttack1_1, 2, 2, &attackRight[0], &attackLeft[0]);
    loadAnim(imgAttack1_2, 2, 2, &attackRight[0], &attackLeft[0]);

    //frappe 2
    imgAttack2_1.LoadFromFile("images/heros/attack2-1.png");
    imgAttack2_2.LoadFromFile("images/heros/attack2-2.png");
    loadAnim(imgAttack2_1, 2, 2, &attackRight[1], &attackLeft[1]);
    loadAnim(imgAttack2_2, 2, 2, &attackRight[1], &attackLeft[1]);


    imgAttack3_1.LoadFromFile("images/heros/attack3-1.png");
    imgAttack3_2.LoadFromFile("images/heros/attack3-2.png");
    imgAttack3_3.LoadFromFile("images/heros/attack3-3.png");
    loadAnim(imgAttack3_1, 2, 2, &attackRight[2], &attackLeft[2]);
    loadAnim(imgAttack3_2, 2, 2, &attackRight[2], &attackLeft[2]);
    loadAnim(imgAttack3_3, 1, 2, &attackRight[2], &attackLeft[2]);

    imgAttack4_1.LoadFromFile("images/heros/attack4-1.png");
    imgAttack4_2.LoadFromFile("images/heros/attack4-2.png");
    loadAnim(imgAttack4_1, 3, 2, &attackRight[3], &attackLeft[3]);
    loadAnim(imgAttack4_2, 3, 2, &attackRight[3], &attackLeft[3]);

    //dead
    imgDead[0].LoadFromFile("images/heros/dead-1.png");
    imgDead[1].LoadFromFile("images/heros/dead-2.png");
    imgDead[2].LoadFromFile("images/heros/dead-3.png");
    imgDead[3].LoadFromFile("images/heros/dead-4.png");
    imgDead[4].LoadFromFile("images/heros/dead-5.png");
    loadAnim(imgDead[0], 2, 2, &animDeadRight, &animDeadLeft);
    loadAnim(imgDead[1], 2, 2, &animDeadRight, &animDeadLeft);
    loadAnim(imgDead[2], 2, 2, &animDeadRight, &animDeadLeft);
    loadAnim(imgDead[3], 2, 2, &animDeadRight, &animDeadLeft);
    loadAnim(imgDead[4], 2, 2, &animDeadRight, &animDeadLeft);

    //état initial
    SetAnim(&StandRight);
    direction = DROITE;
    SetCenter(GetSize().x/1.2,GetSize().y*1.65);

    initAnimTemp();
}

void Hero::initAnimTemp()
{
    //animations pour taper les mobs
    imgDegats.LoadFromFile("resources/animations/attack.png");
    loadAnim(imgDegats, 3, 1, &animDegats);
    animations[ANIM_MOBS_DEGATS].SetAnim(&animDegats);
    animations[ANIM_MOBS_DEGATS].SetFrameTime(0.001);

    //animations pour le heal
    imgHeal.LoadFromFile("resources/animations/heal.png");
    loadAnim(imgHeal, 5, 1, &animHeal);
    animations[ANIM_HEAL].SetAnim(&animHeal);

    //animations pour le mana
    imgMana.LoadFromFile("resources/animations/mana.png");
    loadAnim(imgMana, 13, 1, &animMana);
    animations[ANIM_MANA].SetAnim(&animMana);

    //anim level up
    imgLevelup.LoadFromFile("resources/animations/levelup2.png");
    loadAnim(imgLevelup, 7, 1, &animLevelup);
    animations[ANIM_LVLUP].SetAnim(&animLevelup);

    //anim de téléportation
    imgTeleport.LoadFromFile("resources/animations/teleport.png");
    loadAnim(imgTeleport, 8, 1, &animTeleport);
    animations[ANIM_TELEPORT].SetAnim(&animTeleport);

    //anim de téléportation d'arrivée
    imgTeleportArrived.LoadFromFile("resources/animations/teleportArrived.png");
    loadAnim(imgTeleportArrived, 8, 1, &animTeleportArrived);
    animations[ANIM_TELEPORT_ARRIVED].SetAnim(&animTeleportArrived);

    //anim indiquant des points de compétences disponibles
    imgPtsComp.LoadFromFile("resources/animations/up.png");
    loadAnim(imgPtsComp, 6, 1, &animPtsComp);
    animations[ANIM_PTS_COMP].SetAnim(&animPtsComp);
    animations[ANIM_PTS_COMP].Resize(50*FENETREX/800.0,50*FENETREY/600.0);

    //animations de sort pour milathéa*
    imgMilathea1.LoadFromFile("resources/animations/milathea1.png");
    loadAnim(imgMilathea1, 5, 1, &animMilathea);
    imgMilathea2.LoadFromFile("resources/animations/milathea2.png");
    loadAnim(imgMilathea2, 5, 1, &animMilathea);
    imgMilathea3.LoadFromFile("resources/animations/milathea3.png");
    loadAnim(imgMilathea3, 5, 1, &animMilathea);
    imgMilathea4.LoadFromFile("resources/animations/milathea4.png");
    loadAnim(imgMilathea4, 5, 1, &animMilathea);
    imgMilathea5.LoadFromFile("resources/animations/milathea5.png");
    loadAnim(imgMilathea5, 5, 1, &animMilathea);
    animations[ANIM_MILATHEA].SetAnim(&animMilathea);

    //animation de respawn
    imgRespawn1.LoadFromFile("resources/animations/respawn1.png");
    loadAnim(imgRespawn1, 5, 1, &animRespawn);
    imgRespawn2.LoadFromFile("resources/animations/respawn2.png");
    loadAnim(imgRespawn2, 5, 1, &animRespawn);
    imgRespawn3.LoadFromFile("resources/animations/respawn3.png");
    loadAnim(imgRespawn3, 5, 1, &animRespawn);
    animations[ANIM_RESPAWN].SetAnim(&animRespawn);

    for (int i=0; i<ANIM_TAILLE; ++i)
    {
        animations[i].SetCenter(animations[i].GetSubRect().GetWidth()/2,animations[i].GetSubRect().GetHeight());
        if (i != ANIM_MOBS_DEGATS) animations[i].SetFrameTime(0.07);
        animations[i].SetLoop(false);
        animations[i].Stop();
    }

    animations[ANIM_PTS_COMP].SetLoop(true);
    animations[ANIM_PTS_COMP].SetFrameTime(0.1);
    animations[ANIM_MILATHEA].SetLoop(true);
}

void Hero::showAnimations(RenderWindow *App)
{
    for (int i=0; i<ANIM_TAILLE; ++i)
    {
        if (!animations[i].IsPaused())
        {
            if (i != ANIM_PTS_COMP)
            {
                int relatif = -20;

                //si on doit jouer l'anim sur le mob
                if (i==ANIM_MOBS_DEGATS)
                    relatif = (direction==GAUCHE) ? -70 : 70;

                animations[i].SetPosition(GetPosition().x+relatif,GetPosition().y+20);
            }

            App->Draw(animations[i]);
            animations[i].anim(App->GetFrameTime());
        }
    }

    int res=0;
    for (std::list<sf::Sprite>::iterator i= Ombre.begin();i!= Ombre.end();++i)
    {
        res+=DEGRADE_OMBRE;
        i->SetColor(Color(res,res,res,res));
        App->Draw(*i);

    }
}

bool Hero::operator ^(Entite &e)
{
    bool res=(Entite)(*this)^e;
    float add=statsPerso[ATTAQUE]*berzerk_force;
    if(res)
    {
        e.AddForce(Vector2f((direction == GAUCHE)? -add:add,-add));
        e. SetColor(Color(255,50,50,190));
    }
    return res;

};

bool Hero::operator ^(Objet &e)
{
    Vector2f Pos=this->GetPosition(),
             EPos=e.GetPosition(),
             Esize(e.GetSize().x/2,e.GetSize().y/2);
    return ((Pos.x - Esize.x <= EPos.x)  and  (Pos.x + Esize.x >= EPos.x)  and  ((Pos.y - Esize.y <= EPos.y+5)  and  (Pos.y + Esize.y >= EPos.y)));
};

void Hero::collisionObjet(Objet &e)
{
    if (e^(*this))
    {
        //décolage avec trempo
        if (e.getType()==TREMPOLINE)
        {
            AddForceY(e.getRebond() - (getForce().y * 1.3));
            son->playSon(SON_TREMPOLINE);
        }
        //accélération
        else if (e.getType()==ACCELERATEUR)
        {
            AddForceX(e.getAcceleration());
            son->playSon(SON_ACCELERATEUR);
        }
        //piège
        else if (e.getType()==PIEGE  and  clock_regen_piege.IsPaused())
        {
            modifieVie(-e.getDegats());
            AddForceY(-e.getDegats()*10);
            son->playSon(SON_PIEGE);
            clock_regen_piege.SetTime(1); //pour que le perso ne puisse pas se reprendre des dégats par les pièges tout de suite
        }
    }
};

void Hero::incQuette()
{
    if (++quette >= quetteLvl){
        if (portail!=NULL)
            portail->Play();
        else
          cout<<"fin du jeu"<<endl;
    }
}


/////////////////////////////////////////
/// Indique au héros qu'il est mort
/////////////////////////////////////////
void Hero::youAreDead()
{
    DEAD = true;
    if (direction == GAUCHE)
        SetAnim(&animDeadLeft);
    else
        SetAnim(&animDeadRight);

    SetLoop(false);
    SetFrameTime(0.15);
    Play();
}


void Hero::respawn(sf::Vector2f posRespawn)
{
   DEAD = false;
   vie = viemax / 2.0;
   mana = manamax / 2.0;
   SetPosition(posRespawn);
   vies --;
   xpPerso[XP] = xpPerso[XP] - xpPerso[XPLEVEL]/10;
   if (xpPerso[XP]<0) xpPerso[XP]=0;
}

////////////////////////////////////////
/// Téléporte le héros
////////////////////////////////////////
inline void Hero::teleport()
{
    //on change sa position
    SetPosition(destinationTeleport);
    //on met à jour les sons spatialisés
    listener.SetPosition(GetPosition().x, GetPosition().y,0);
    //on joue l'animation d'arrivée
    animations[ANIM_TELEPORT_ARRIVED].Play();
}

inline void Hero::Push_Back_Ombre(Sprite s)
{
    if (Ombre.size() < MAX_TAILLE_OMBRE)    //il il n'y a pas le nombre max d'ombre
    {
        /*int Res=255-DEGRADE_OMBRE;
        s.SetColor(Color(Res,Res,Res,Res));*/
        Ombre.push_back(s);
    }
    else    //il y a deja le nombre max d'image
    {
        Ombre.pop_front();
        Ombre.push_back(s);

    }
};
