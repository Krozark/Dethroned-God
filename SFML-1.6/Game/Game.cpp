#include "Game.hpp"

#include "stdio.h"
#include <string.h>
#include <time.h>
float TEMPS_PAR_FRAME;

#define TIMELOAD 1

using namespace std;
using namespace sf;


//*******************************************************************
//      Initialisation du jeu
//*******************************************************************
Game::Game(RenderWindow* Window, _options* setting, Sons* son, _touches_heros* touche, _touches_sys* touches_sy, statusJeu* ETA, int* level, _sauvegarde* loadGame, _saveHeros* oldHero)
{
    TEMPS_PAR_FRAME=1.5/setting->fps;
    levelCourant = level;

    App = Window;
    settings = setting;
    sounds=son;
    touches=touche;
    touchesSys=touches_sy;
    ETAT = ETA;
    perso=NULL;

    //level
    if ((*levelCourant)==0)
    {
        if (loadGame == NULL)
            (*levelCourant) = 1;
        else
            (*levelCourant) = atoi(loadGame->nomVerifLevel);
    }

    if (oldHero!=NULL) //c'est qu'on est d�j� en jeu
        ++(*levelCourant);


    //chargement du level
    //new game
    if (loadGame == NULL)
    {
        stringstream out;
        out << "./lvl/" << (*levelCourant);
        nomf = out.str();
        loadMap(nomf.c_str(),loadGame,oldHero);

        //raccourcis : valeurs par d�faut si c'est une nouvelle partie
        if (oldHero == NULL)
        {
            touches->tabBarreSortsId[F1_BS] = (int)POTION_VIE_LOOT;
            touches->tabBarreSortsObj[F1_BS] = true;
            touches->tabBarreSortsId[F2_BS] = (int)POTION_MANA_LOOT;
            touches->tabBarreSortsObj[F2_BS] = true;
            touches->tabBarreSortsId[F3_BS] = (int)PARCHEMIN_LOOT;
            touches->tabBarreSortsObj[F3_BS] = true;
            touches->tabBarreSortsId[F4_BS] = (int)SAUVEGARDES_LOOT;
            touches->tabBarreSortsObj[F4_BS] = true;
            for (int i=4; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
            {
                touches->tabBarreSortsId[i] = -1;
                touches->tabBarreSortsObj[i] =  true;
            }
        }
    }
    //load game
    else
    {
        stringstream out;
        out << "./lvl/" << loadGame->nomVerifLevel;
        nomf = out.str();
        loadMap(nomf.c_str(),loadGame,oldHero);
    }

    //cam�ra
    camera.SetFromRect(sf::FloatRect(0, 0, FENETREX-TAILLEBLOC, FENETREY-TAILLEBLOC));
    initCamera();
    camera.Zoom(1);
    App->SetView(camera);

    parchemin = new Parchemin(&back,sounds,touches,touchesSys,&vectorSkills);

    menupause = new Pause(perso->getStats(),perso->getScores(),perso->getExp(),perso->getVie(),perso->getMana(),perso->tab_nbobjets,perso->getSkills(),settings,touches,touchesSys,sounds,&vectorSkills,*levelCourant);
    menusauvegarde = new MenuSauvegarde(touchesSys, sounds);
    marchand = new Marchand(App,touchesSys,perso);

    //temps total de jeu
    totalGameTime = 0;
    if (loadGame!=NULL) totalGameTime = loadGame->totalGameTime;
    else if (oldHero!=NULL) {
        totalGameTime = oldHero->totalGameTime;
    }
    clockTotalGameTime.Play();

    //musique
    musicLvl.SetLoop(true);
    musicLvl.Play();
    musicLvl.SetVolume(settings->volumeMusiqueJeux);
    musicLvlCool.OpenFromFile("./musique/lvl1cool.ogg");
    musicLvlCool.SetLoop(true);
    musicLvlCool.SetVolume(settings->volumeMusiqueMenus);
    musicDead.OpenFromFile("./musique/dead.ogg");
    musicDead.SetVolume(0);

    //police
    font.LoadFromFile("./fonts/BlackCastleMF.ttf");

    //alpha
    alpha=ALPHA;

    //affichage du texte
    pause = false;
    waitForKey = false;
    headTransparent = false;
    exitGame = false;
    displayText = new AffichageTexte(App);
        //GUI
    initGui(loadGame);


    //horloge
    clock.Play();
    clockLoadMap.SetTime(TIMELOAD);

    //initialisation des images des skills
    menupause->majBS(gui_spr);

    //�cran de chargement
    loading.SetText("Chargement...");
    loading.SetCenter(loading.GetRect().GetWidth()/1.5,loading.GetRect().GetHeight()/2);
    backLoad.LoadFromFile("./resources/Chargement/contour.png");
    SBackLoad.SetImage(backLoad);
    SBackLoad.Resize(gui_spr[FOND_GUI].GetSize().x/2,50 * FENETREY/600.0);

    //sauvegarde du point de respawn
    saveForRespawn();
 }

 inline void Game::initCamera()
 {
     int camera_x=0,camera_y=0;

    if (perso->GetPosition().y > camera.GetHalfSize().y+TAILLEBLOC  and  perso->GetPosition().y < (MAPSIZEY - camera.GetHalfSize().y-TAILLEBLOC))
        camera_y=perso->GetPosition().y;
    else if (perso->GetPosition().y < camera.GetHalfSize().y + TAILLEBLOC)
        camera_y= camera.GetHalfSize().y + TAILLEBLOC;
    else if (perso->GetPosition().y > MAPSIZEY - camera.GetHalfSize().y - TAILLEBLOC)
        camera_y=MAPSIZEY - camera.GetHalfSize().y - TAILLEBLOC;
    //else if ()


    //si le perso est au milieu en x
    if (perso->GetPosition().x > camera.GetHalfSize().x+TAILLEBLOC  and  perso->GetPosition().x < (MAPSIZEX - camera.GetHalfSize().x-TAILLEBLOC))
        camera_x=perso->GetPosition().x;
    else if (perso->GetPosition().x < camera.GetHalfSize().x + TAILLEBLOC )
        camera_x=camera.GetHalfSize().x + TAILLEBLOC;
    else if (perso->GetPosition().x > MAPSIZEX - camera.GetHalfSize().x-TAILLEBLOC)
        camera_x=MAPSIZEX - camera.GetHalfSize().x-TAILLEBLOC;


    camera.SetCenter(camera_x,camera_y);
 }


//*******************************************************************
//      Ce qui est fait � chaque tour de boucle du main
//*******************************************************************
void Game::run()
{
    if ((*ETAT) == PAUSED) //menu pause
        enPause();
    else if ((*ETAT) == PARCHEMINED) //parchemin
        enParchemin();
    else if ((*ETAT) == SAVED) //sauvegarde
        enSauvegarde();
    else if ((*ETAT) == MARCHANDISE)
        marchand->Draw();
    else //jeu
        enJeu();
}

inline void Game::enPause()
{
    //si on vient de lancer le menu pause
    if (musicLvl.GetStatus() == Sound::Playing) //on arrete la musique
    {
        pauseGame();
        menupause->majaData(perso->getStats(), perso->getScores(), perso->getExp(), perso->getVie(), perso->getMana(), perso->tab_nbobjets, perso->getSkills());
    }

    //on fait tourner la pause
    menupause->run();
    menupause->show(*App);


    if (menupause->getExit()) //on quitte la pause
    {
        //avant on enregistre les stats du perso si elles ont �t� modifi�es
        for (int i=0; i<5; ++i)
            if (menupause->tabPlusMoins[i] > 0)
                perso->statsPerso[i+1] += menupause->tabPlusMoins[i];
        perso->statsPerso[NB_POINTS_SKILLS] = menupause->getNbPtsSkill();
        perso->calculeStats();

        //on prend en compte les options si elles ont �t� chang�es
        if (menupause->getChangeOptions())
        {
            musicLvl.SetVolume(settings->volumeMusiqueJeux); //musique du jeux
            musicLvlCool.SetVolume(settings->volumeMusiqueMenus); //musique des menus
            sounds->updateVolumeSons(settings); //volume des sons
            for (unsigned int i=0; i<effetsSonores.size();++i) effetsSonores[i]->SetVolume(volumesEffetsSonores[i] * settings->volumeEffetsSonores/100.0); //volume effets sonores
            App->SetFramerateLimit(settings->fps); //fps
            TEMPS_PAR_FRAME=1.5/settings->fps;
            settings->verticalSync = (menupause->getVerticalSync()) ? true : false; //synchronisation vertical
            App->UseVerticalSync(settings->verticalSync);
            settings->fullscreen = (menupause->getFullScreen()) ? true : false; //fullscreen

            //resolution de l'ecran
            string resolution = menupause->getResol();
            int posSeparateur = resolution.find('x',0);
            string resolutionX = resolution.substr(0,posSeparateur);
            string resolutionY = resolution.substr(posSeparateur+1,resolution.size()-(posSeparateur+1));
            for (unsigned int i=0; i<10; ++i)
            {
                settings->resolutionX[i] = (i<resolutionX.size()) ? resolutionX[i] : '\0';
                settings->resolutionY[i] = (i<resolutionY.size()) ? resolutionY[i] : '\0';
            }


            //*********************************
            //  �critures dans le fichier
            //*********************************
            //ouverture
            FILE* file = NULL;
            file = fopen("./data/settings","wb");

            if (file != NULL)
            {
                //�criture des options de base
                fwrite(settings,sizeof(_options),1,file);

                //�criture des touches h�ros
                fwrite(touches,sizeof(_touches_heros),1,file);

                //�criture des touches syst�me
                fwrite(touchesSys,sizeof(_touches_sys),1,file);

                //fermeture du fichier
                fclose(file);
            }
       }

        //on regarde si on doit quitter le jeu ou pas
        if (menupause->getExitGame())
            exitGame=true;
        else
        {
            //mise � jour de la barre des sorts
            majStatsGuiSansResize();
            //skills
            menupause->majBS(gui_spr);

             //on quitte la pause pour revenir au jeu
            (*ETAT) = GAMED;
            menupause->setExit(false); //on r�initialise le bool�en de sortie a false pour la prochaine pause
            menupause->reset();
            unpauseGame();
        }
    }
}

inline void Game::enParchemin()
{
    //si on �tait dans le jeu, on arr�te la musique
    if (musicLvl.GetStatus() == Sound::Playing)
    {
        pauseGame();

        //on met � jour le menu parchemin
        parchemin->maj(perso->listeParchemins, &back);
    }

    //on affiche le menu parchemin
    parchemin->display(*App);
}

inline void Game::enSauvegarde()
{
    //si on vient de lancer le menu pause
    if (musicLvl.GetStatus() == Sound::Playing) //on arrete la musique
        pauseGame();

    //on demande de sauvegarder
    if (menusauvegarde->getSaveGame())
        saveGame();

    if (menusauvegarde->getExit())
    {
        menusauvegarde->setExit(false);
        (*ETAT) = GAMED;
        unpauseGame();
    }

    menusauvegarde->Draw(*App);
}

inline void Game::saveGame()
{
    //structure de sauvegarde
    _sauvegarde save;

    saveGameFirst(&save);

    menusauvegarde->SaveGame(&save,touches,settings);

    saveForRespawn();
}

void Game::saveGameFirst(_sauvegarde *save)
{
    //temps de jeu
    save->totalGameTime = clockTotalGameTime.GetElapsedTime() + totalGameTime;

    //infos level
    //nom
    for (unsigned int i=0; i<100; ++i)
    {
        save->nomLevel[i] = (i<infosLevel[NOM_LEVEL].size()) ? infosLevel[NOM_LEVEL][i] : '\0';
        save->mythologieLevel[i] = (i<infosLevel[MYTHOLOGIE_LEVEL].size()) ? infosLevel[MYTHOLOGIE_LEVEL][i] : '\0';
        save->typeLevel[i] = (i<infosLevel[TYPE_LEVEL].size()) ? infosLevel[TYPE_LEVEL][i] : '\0';
    }
    for (unsigned int i=0; i<16; ++i)
        save->nomVerifLevel[i] = (i<infosLevel[NOM_VERIF_LEVEL].size()) ? infosLevel[NOM_VERIF_LEVEL][i] : '\0';

    //h�ros
    saveGameHeros(save);

    //variable de qu�te du h�ros
    save->varQuete = perso->getQuette();

    //direction du h�ros
    save->direction = perso->GetDirection();

    //mobs
    for (unsigned int i=0; i<100; ++i)
        save->mobs[i] = (i<mobs.size()) ? mobs[i].isEnable() : false;

    //objets
    for (unsigned int i=0; i<100; ++i)
        save->objets[i] = (i<objets.size()) ? objets[i].isEnable() : false;

    //animations de recharge des sorts
    for (unsigned int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
        save->currentFrameAnimRecharge[i] = perso->gui_recharges[i]->GetCurrentFrame();
}

inline void Game::saveGameHeros(_sauvegarde *save)
{
    //sauvegarde des stats
    for (int i=0; i<TAILLE_STATS; ++i)
        save->saveHeros.statsPerso[i] = perso->getStats()[i];

    //sauvegarde des scores
    for (int i=0; i<TAILLE_SCORES; ++i)
        save->saveHeros.scoresPerso[i] = perso->getScores()[i];

    //sauvegarde de l'xp
    for (int i=0; i<TAILLE_EXP; ++i)
        save->saveHeros.xpPerso[i] = perso->getExp()[i];

    //sauvegarde des objets
    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
        save->saveHeros.tab_objets[i] = perso->tab_objets[i];

    //vie & mana
    save->saveHeros.vie = perso->getVie();
    save->saveHeros.mana = perso->getMana();

    //skills
    for (int i=0; i<TAILLE_SKILLS; ++i)
        save->saveHeros.tabSkills[i] = perso->skillsPerso[i];

    //position
    save->posXheros = perso->GetPosition().x;
    save->posYheros = perso->GetPosition().y;
}

inline void Game::enJeu()
{
    if (musicLvl.GetStatus() == Sound::Paused  and  !pause)
    {
        musicLvlCool.Stop();
        musicLvl.Play();
    }

    //gestion de la mort
    deadOfHero();

    //camera
    if (perso->GetPosition().y > camera.GetHalfSize().y+TAILLEBLOC  and  perso->GetPosition().y < (MAPSIZEY - camera.GetHalfSize().y-TAILLEBLOC))
    {
        camera.SetCenter(camera.GetCenter().x, perso->GetPosition().y);
    }

    if (perso->GetPosition().x > camera.GetHalfSize().x+TAILLEBLOC  and  perso->GetPosition().x < (MAPSIZEX - camera.GetHalfSize().x-TAILLEBLOC)) {
        camera.SetCenter(perso->GetPosition().x, camera.GetCenter().y);
    }

    gestionTransparenceGUI();

    controleHorlogesPerso();
   // deplacements();
    show();
};

inline void Game::gestionTransparenceGUI()
{
    if ((perso->GetPosition().y - perso->GetSize().y/2.0) < (camera.GetCenter().y-FENETREY/2.0) + gui_spr[FOND_GUI].GetSize().y)
    {
        if (!headTransparent)
        {
            for (int i=0; i<TAILLE_GUI; ++i)
                gui_spr[i].SetColor(Color(255,255,255,75));
        }
        headTransparent = true;
    }
    else
    {
        if (headTransparent)
        {
            for (int i=0; i<TAILLE_GUI; ++i)
                gui_spr[i].SetColor(Color(255,255,255,255));
        }
        headTransparent = false;
    }
};

inline void Game::gestionDialoguesHeros()
{
    //le perso � rencontrer une entite lui proposant un dialogue
    if (perso->getMeetDialogue())
    {
        //on dit au h�ros qu'il n'a plus de dialogue � afficher
        perso->setMeetDialogue(false);
        //on affiche les messages
        launchDialogue(perso->getMetDialogue());
        //Time=TEMPS_PAR_FRAME;
        clock.Reset();
    }
}

inline void Game::pauseGame()
{
    musicLvlCool.Play();
    musicLvl.Pause();
    for (unsigned int i=0; i<effetsSonores.size(); ++i)
        effetsSonores[i]->Pause();
    for (unsigned int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
        perso->gui_recharges[i]->Pause();
    App->SetView(App->GetDefaultView());

    //on arr�te l'horloge du jeu
    if (clock.IsPlay())
        clock.Pause();

    //on arrete le chrono si il y en a
    if (levelWithChrono)
        clockNiveaux.Pause();

    pause = true;
}

inline void Game::deadOfHero()
{
    bool dead = (perso->getVies()<=0);

    //dead by physical actions
    if (perso->getVie()==0)
    {
        //on baisse le volume du jeu
        if (dead)
            musicDeadFondu(4,3);

        if (!perso->getDead()) //si le perso n'a pas encore vu qu'il est mort
        {
            perso->youAreDead(); //on le lui dit
            if (dead)
                musicDead.Play();
        }
        else if (perso->GetCurrentFrame() >= perso->getSizeDeadAnim()-1)
        {
            if (dead)
                musicLvl.Stop();
            perso->Pause();
            perso->SetFrame(perso->GetCurrentFrame());
        }
    }
    //dead by falling
    else if (perso->getDead())
    {
        //on baisse le volume du jeu
        if (dead)
            musicDeadFondu(20,5);

        //le perso n'a pas encore vu qu'il est mort
        if (!perso->getDead())
        {
            sounds->playSon(MISS_MOB);
            musicDead.Play();
            perso->setEnable(false);
        }
    }
}


void Game::musicDeadFondu(int volumeMusique, int volumeDead)
{
    (musicLvl.GetVolume()>volumeMusique) ? musicLvl.SetVolume(musicLvl.GetVolume()-volumeMusique) : musicLvl.Stop();
    (musicDead.GetVolume()<(100-volumeDead)) ? musicDead.SetVolume(musicDead.GetVolume()+volumeDead) : musicDead.SetVolume(100);
}

void Game::unpauseGame()
{
    App->SetView(camera);
    for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
        if (perso->gui_recharges[i]->GetCurrentFrame() > 0)
            perso->gui_recharges[i]->Play();

    for (unsigned int i=0; i<effetsSonores.size(); ++i)
        effetsSonores[i]->Play();

    //on arrete le chrono si il y en a
    if (levelWithChrono)
        clockNiveaux.Play();

    pause = false;
}

//**************************************************************
//      Affichage de la map
//**************************************************************
void Game::show()
{
    if (!pause)
    {
        Time = clock.GetElapsedTime();
        clock.Reset();
        //fond d'ecran
        Vector2i centreCamera=Vector2i(camera.GetCenter().x,camera.GetCenter().y);

        App->Draw(SBack);


        Vector2i min((centreCamera.x-camera.GetHalfSize().x)/TAILLEBLOC,(centreCamera.y-camera.GetHalfSize().y)/TAILLEBLOC),    //coin supérieur gauche en coordonée de bloc
                 max((centreCamera.x+camera.GetHalfSize().x)/TAILLEBLOC,(centreCamera.y+camera.GetHalfSize().y)/TAILLEBLOC);    //coin inferieur droit en coordonée de block

        for (int y=min.y; y<=max.y; ++y)
        {
            for(int x=min.x; x<=max.x; ++x)
            {
                App->Draw(tab_sprite[y][x]);
            }
        }

        /*for(vector<Floor>::iterator i=tab_sols.begin();i!=tab_sols.end();++i)
            App->Draw(*i);*/
        int nombre_division=1;
        float time_total=Time;
        while (Time>TEMPS_PAR_FRAME)
        {
            Time/=2;
            nombre_division*=2;
        }

        //d�placement du perso
        if (perso->isEnable()  and  !pause  and  clockLoadMap.IsPaused())
            perso->bouger_perso(App,Time,nombre_division,tab_collision,&objets,&mobs);

        //affichage des objets
        int cpt=0;
        for(vector<Objet>::iterator obj_iter=objets.begin();obj_iter<objets.end();++obj_iter)
        {
            if (obj_iter->isEnable()  and  clockLoadMap.IsPaused())
            {
                obj_iter->Update(Time,nombre_division,tab_collision);
                //obj_iter->animObjet(App);
                obj_iter->anim(time_total);
                if (!((obj_iter->getType()==PARCHEMIN or obj_iter->getType()==TELEPORTEUR_QUETTE)  and  obj_iter->getQuette() > perso->getQuette()+1))
                    App->Draw(*obj_iter);

                if (obj_iter->getType() <= PIEGE)
                    perso->collisionObjet(*obj_iter);

               // App->Draw(sf::Shape::Rectangle(obj_iter->GetPosition().x,obj_iter->GetPosition().y,obj_iter->GetPosition().x+2,obj_iter->GetPosition().y+2,Color::White));
            }
            ++cpt;
        }

        //affichage des mobs
        for (vector<Pnj>::iterator mobs_iter= mobs.begin();mobs_iter< mobs.end();++mobs_iter)
        {
            if (mobs_iter->isEnable()  and  clockLoadMap.IsPaused())
            {
                mobs_iter->bouger_mob(App,Time,nombre_division,tab_collision);
                mobs_iter->trouveEntite(*perso);
                App->Draw(*mobs_iter);

                //level
                String mobLevel;
                mobLevel.SetPosition(mobs_iter->GetPosition().x,mobs_iter->GetPosition().y-mobs_iter->GetSize().y-20);
                stringstream out;
                out << mobs_iter->getLvl();
                mobLevel.SetText(out.str());
                mobLevel.SetSize(15);
                App->Draw(mobLevel);

                //boss
                if (mobs_iter->isBoss())
                {
                    sprBoss.SetPosition(mobLevel.GetPosition().x+10,mobLevel.GetPosition().y);
                    App->Draw(sprBoss);
                }

                //vie
                String mobVie;
                mobVie.SetPosition((mobs_iter->isBoss()) ? Vector2f(sprBoss.GetPosition().x+10, sprBoss.GetPosition().y) : Vector2f(mobLevel.GetPosition().x+10,mobLevel.GetPosition().y));
                out.str(string());
                out << " - " << mobs_iter->getVie();
                mobVie.SetText(out.str());
                mobVie.SetSize(15);
                if (mobs_iter->getVie()>=(mobs_iter->getVieMax()-mobs_iter->getVieMax()/5))
                    mobVie.SetColor(Color(0,255,0));
                else if (mobs_iter->getVie()<=(mobs_iter->getVieMax()/5))
                    mobVie.SetColor(Color(255,0,0));
                else
                    mobVie.SetColor(Color(255,120,0));
                App->Draw(mobVie);
                sprLivesMob.SetPosition(mobVie.GetPosition().x+mobVie.GetRect().GetWidth()+5,mobVie.GetPosition().y);
                App->Draw(sprLivesMob);

                showBonusMalusMobs(mobs_iter);
            }
        }

        //fondu
        if (perso->getDead())
            deadFondu();

        //affichage du perso
        if (perso->isEnable()  and  perso->getDisplay() and (*ETAT)!=LOADING_GAME)
        {
            if (perso->getBerzerk())
                perso->SetColor(Color::Red);
            App->Draw(*perso);
        }

       // App->Draw(sf::Shape::Rectangle(perso->GetPosition().x,perso->GetPosition().y,perso->GetPosition().x+2,perso->GetPosition().y+2,Color::Yellow));

        //animations du perso
        perso->showAnimations(App);

        //affichage de la GUI
        showGUI();

        //affichage des bonus malus sur la tete du perso si besoin
        showBonusMalus();

        if ((*ETAT)!=LOADING_GAME)
            //gestion des dialogues
            gestionDialoguesHeros();
        //chargement lors du d�but du jeu
        else
        {
            int percent = (int)((100*clockLoadMap.GetElapsedTime())/TIMELOAD);
            stringstream out;
            out << percent << "%";
            strPercent.SetText(out.str());
            SBackLoad.SetPosition(gui_spr[FOND_GUI].GetPosition().x+gui_spr[FOND_GUI].GetSize().x/4,gui_spr[FOND_GUI].GetPosition().y+FENETREY/2);
            strPercent.SetPosition(SBackLoad.GetPosition().x+SBackLoad.GetSize().x/2.0,SBackLoad.GetPosition().y + SBackLoad.GetSize().y/2.0 - strPercent.GetRect().GetHeight()/2.0);
            App->Draw(sf::Shape::Rectangle(SBackLoad.GetPosition().x,SBackLoad.GetPosition().y,SBackLoad.GetPosition().x+((percent * (SBackLoad.GetSize().x))/100.0),SBackLoad.GetPosition().y+SBackLoad.GetSize().y,Color(172,0,0,140)));
            App->Draw(SBackLoad);
            App->Draw(strPercent);

            //condition d'arr�t
            if (clockLoadMap.IsPaused())
            {
                (*ETAT) = GAMED;
                for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
                {
                    if (perso->gui_recharges[i]->GetCurrentFrame() != 0)
                        perso->gui_recharges[i]->Play();
                    clockNiveaux.Play();
                }
            }
        }
    }


    ///////////// gestion des screens
    if (App->GetInput().IsKeyDown(touches->screenshot))
    {
        struct tm Today;
        time_t maintenant;

        time(&maintenant);
        Today = *localtime(&maintenant);

        sf::Image Screen = App->Capture();     // creation de l'image de screen

        stringstream out;
        out<<"screen/"<<Today.tm_year+1900<<"-"<<Today.tm_mday<<"-"<<Today.tm_mon+1<<"_"<<Today.tm_hour<<"h"<<Today.tm_min<<"m"<<Today.tm_sec<<"s.jpg"; //nom du screen avec la date et heure du moment

        Screen.SaveToFile(out.str());         //enregistrement de l'image
    }

    if (levelWithChrono  and  clockNiveaux.GetRemainingTime() < 0)
        (*ETAT) = GAME_OVER;
};

void Game::launchDialogue(string text)
{
    if (levelWithChrono)
        clockNiveaux.Pause();
    pause = true;
    text = parchemin->convertText(text,false);
    if (perso->GetPosition().y>camera.GetCenter().y+ camera.GetHalfSize().y)
        displayText->SetPosition(camera.GetCenter().x-camera.GetHalfSize().x , gui_spr[FOND_GUI].GetPosition().y, true);
    else
        displayText->SetPosition(camera.GetCenter().x-camera.GetHalfSize().x  , camera.GetCenter().y+camera.GetHalfSize().y - 120*coef_resize_y, false);
    displayText->ShowText(text);
    waitForKey = true;
};

void Game::unpauseAfterDial()
{
    //si il y a une suite, on reprend le dialogue
    if (displayText->getSuite())
        launchDialogue(displayText->getTextFollowing());
    else
    {
        if (levelWithChrono)
            clockNiveaux.Play();
        pause=false;
    }
};

inline void Game::deadFondu()
{
    App->Draw(sf::Shape::Rectangle(gui_spr[FOND_GUI].GetPosition().x,gui_spr[FOND_GUI].GetPosition().y,gui_spr[FOND_GUI].GetPosition().x+gui_spr[FOND_GUI].GetSize().x,gui_spr[FOND_GUI].GetPosition().y+FENETREY,Color(0,0,0,alpha+=2)));
    if (alpha >= 240)
    {
        //si il lui reste des vies
        if (perso->getVies()>0)
        {
            respawn();
        }
        else
        {
            //si il est mort
            (*ETAT)=GAME_OVER;
            musicLvl.Stop();
            for (unsigned int i=0; i<effetsSonores.size(); ++i)
                effetsSonores[i]->Stop();
        }
    }
}

inline void Game::respawn()
{
   perso->respawn(posRespawn);
   alpha = ALPHA;
   initCamera();
   perso->animations[ANIM_RESPAWN].Play();
   sounds->playSon(SON_RESPAWN);
}

inline void Game::saveForRespawn()
{
    posRespawn = perso->GetPosition();
}

void Game::showgameover()
{
    App->Draw(SBack);
}

void Game::showBonusMalusMobs(std::vector<Pnj>::iterator mob_iter)
{
    int ecart;

    //on les affiche tous et on augmente leur position de 5px
    for (unsigned int i=0; i<mob_iter->bonusMalus.size(); ++i)
    {
        ecart = mob_iter->bonusMalusPosDepartY[i] - mob_iter->bonusMalus[i].GetPosition().y-(2*mob_iter->GetSize().y)/3;
        App->Draw(mob_iter->bonusMalus[i]);
        mob_iter->bonusMalus[i].SetPosition(mob_iter->GetPosition().x,mob_iter->bonusMalus[i].GetPosition().y-2);

        if (mob_iter->isDead()  and  ecart>20)
            mob_iter->setEnable(false);

        //si il a parcourut 50px au dessus du joueur, on le supprime
        if (ecart > 50)
        {
            mob_iter->bonusMalus.erase(mob_iter->bonusMalus.begin() + i);
            mob_iter->bonusMalusPosDepartY.erase(mob_iter->bonusMalusPosDepartY.begin() + i);
        }
    }
}

void Game::showBonusMalus()
{
    //si il y a des bonus/malus � afficher
    //on les affiche tous et on augmente leur position de 5px
    for (unsigned int i=0; i<perso->bonusMalus.size(); ++i)
    {
        App->Draw(perso->bonusMalus[i]);
        perso->bonusMalus[i].SetPosition(perso->GetPosition().x,perso->bonusMalus[i].GetPosition().y-2);
        //si il a parcourut 50px au dessus du joueur, on le supprime
        if ((perso->bonusMalusPosDepartY[i] - perso->bonusMalus[i].GetPosition().y-(2*perso->GetSize().y)/3) > 50)
        {
            perso->bonusMalus.erase(perso->bonusMalus.begin() + i);
            perso->bonusMalusPosDepartY.erase(perso->bonusMalusPosDepartY.begin() + i);
        }
    }
}


void Game::controleHorlogesPerso()
{
    //regen de la vie
    if (perso->getVie() < perso->getVieMax())
    {
        if(perso->clockVie.IsPause())  //si le perso a perdu de la vie et que l'horloge n'est pas en route
            perso->clockVie.Play();     //on la met en route
        else  //si elle est d�j� en route
        {
            if (perso->clockVie.GetElapsedTime() >= 1  and  !perso->getDead())  //toutes les 1 secondes, on ajoute 1 pts de vie et mana
            {
                perso->addVie(perso->statsPerso[DEFENSE]);
                perso->clockVie.Reset();
            }
        }
    }
    else
    {
        if (perso->clockVie.IsPlay())
            perso->clockVie.Stop();
    }

    //regen du mana
    if (perso->getMana() < perso->getManaMax())
    {
        if(perso->clockMana.IsPause())  //si le perso a perdu du mana et que l'horloge n'est pas en route
            perso->clockMana.Play();     //on la met en route
        else  //si elle est d�j� en route
        {
            if (perso->clockMana.GetElapsedTime() >= 1)  //toutes les 1 secondes, on ajoute 1 pt de mana
            {
                perso->addMana(perso->statsPerso[INTELLIGENCE]-7);
                perso->clockMana.Reset();
            }
        }
    }
    else
    {
        if (perso->clockMana.IsPlay())
            perso->clockMana.Stop();
    }

}

void Game::showGUI()
{
    majGUI();       //mise � jour de l'emplacement avent l'affichage

    for (unsigned int i=0;i<TAILLE_GUI-1;++i)
        App->Draw( gui_spr[i]);

    //on regarde quels objets sont a afficher pour afficher leur nombre dans l'inventaire
    for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
    {
        if (touches->tabBarreSortsId[i] > -1  and  touches->tabBarreSortsObj[i])
        {
            perso->tab_nbobjets[touches->tabBarreSortsId[i]].SetPosition(gui_spr[F1_GUI+i].GetPosition().x,gui_spr[F1_GUI+i].GetPosition().y-3);
            App->Draw(perso->tab_nbobjets[touches->tabBarreSortsId[i]]);
        }
    }

    //affichage du level
    stringstream out;
    out << "Lvl : " << perso->statsPerso[LEVEL];
    level.SetText(out.str());
    App->Draw(level);

    //affichage des vies
    out.str(string());
    out << perso->getVies();
    gui_vies.SetText(out.str());
    App->Draw(gui_vies);

    //affichage du compte à rebour si il y en a un
    if (levelWithChrono)
    {
        float time = ((int)(clockNiveaux.GetRemainingTime() * 100)) / 100.0;
        stringstream out;
        out << time;
        gui_time_lvl.SetText(out.str());
    }
    App->Draw(gui_time_lvl);

    //pièces d'or
    out.str(string());
    out <<perso->tab_objets[OR_LOOT];
    money.SetText(out.str());
    App->Draw(money);

    //affichage du contour de la gui
    App->Draw(gui_spr[CONTOUR_GUI]);

    //affichage des stats
    for (int i=0; i<TAILLE_GUI_STATS; ++i)
    {
        App->Draw(gui_statsNbr[i]);
    }

    for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
    {
        if (!perso->gui_recharges[i]->IsPaused())
        {
            App->Draw(*perso->gui_recharges[i]);
            perso->gui_recharges[i]->run(Time);
        }
    }
};


void Game::initGui(_sauvegarde* loadGame)     //initialiser
{
    SBack.SetImage(back);

    for (unsigned int i=0;i<TAILLE_GUI;++i)     //image par defaut dans la gui
        gui_img[i].LoadFromFile("./resources/image/vide.png");

    gui_img[VIE_GUI].LoadFromFile("./resources/GUI/barre_vie.png");
    gui_img[MANA_GUI].LoadFromFile("./resources/GUI/barre_mana.png");
    gui_img[SPRINT_GUI].LoadFromFile("./resources/GUI/barre_sprint.png");
    gui_img[EXP_GUI].LoadFromFile("./resources/GUI/barre_exp.png");
    gui_img[CONTOUR_GUI].LoadFromFile("./resources/GUI/gui_bords.png");
    gui_img[FOND_GUI].LoadFromFile("./resources/GUI/gui_fond.png");

    font2.LoadFromFile("./fonts/BlackCastleMF.ttf");
    //niveau
    //level.SetFont(font2);
    level.SetSize(17);
    level.SetColor(Color(255,255,255));
    //vies
   // gui_vies.SetFont(font2);
    gui_vies.SetSize(25);
    gui_vies.SetColor(Color(255,255,255));
    //temps du niveau
    gui_time_lvl.SetColor(Color(255,255,255));
   // gui_time_lvl.SetFont(font2);
    gui_time_lvl.SetSize(20);
    gui_time_lvl.SetText("Inf");
    //money.SetFont(font);
    money.SetSize(17);
    money.SetColor(Color(255,255,255));


    //*************************************
    //images de la barre de raccourcis
    //*************************************
    //objets
    gui_imgObjet[POTION_VIE_LOOT].LoadFromFile("./resources/image/potionVie.png");
    gui_imgObjet[POTION_MANA_LOOT].LoadFromFile("./resources/image/potionMana.png");
    gui_imgObjet[PARCHEMIN_LOOT].LoadFromFile("./resources/image/parcheminMin.png");
    gui_imgObjet[SAUVEGARDES_LOOT].LoadFromFile("./resources/image/sauvegarde.png");
    gui_imgObjet[OR_LOOT].LoadFromFile("./resources/image/icones/or_1.png");

    for (unsigned int i=0;i<TAILLE_GUI;++i)
        //on attribue une image � chaque sprite de la gui
        gui_spr[i].SetImage(gui_img[i]);

    //loaded game : on rejoue l'anim si elle �tait en cours
    if (loadGame!=NULL)
    {
        for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
        {
            if (loadGame->currentFrameAnimRecharge[i] > 0)
            {
                perso->gui_recharges[i]->SetFrame(loadGame->currentFrameAnimRecharge[i]);
            }
        }
    }

    //vies d'un monstre
    imgLivesMob.LoadFromFile("./resources/image/icones/heart-icon.png");
    sprLivesMob.SetImage(imgLivesMob);
    sprLivesMob.Resize(20,20);

    //boss
    imgBoss.LoadFromFile("./resources/image/icones/star.png");
    sprBoss.SetImage(imgBoss);
    sprBoss.Resize(20,20);

    majStatsGui();
};

//stats affich�es en nombre sur la gui
inline void Game::majStatsGui()
{
    majStatsGuiSansResize();
    resizeGUI();
};

inline void Game::majStatsGuiSansResize()
{
    for (int i=0; i<10; ++i) //de F1 � F10
    {
        //objets
        if (touches->tabBarreSortsId[i] >= 0)
        {
            if (touches->tabBarreSortsObj[i])
                gui_spr[F1_GUI+i].SetImage(gui_imgObjet[touches->tabBarreSortsId[i]]);
        }
        else
            gui_spr[F1_GUI+i].SetImage(gui_img[VIDE_GUI]);
    }
};

void Game::resizeGUI()   //redimansionner en fonction de la taille de la fenettre
{
    float _largeur=gui_img[FOND_GUI].GetWidth(), //706 =>100 % => 1/5 du total
          _hauteur=gui_img[FOND_GUI].GetHeight();//170 => 100% => 1/5 du total

    coef_resize_x=camera.GetRect().GetWidth()/_largeur;
    coef_resize_y=camera.GetRect().GetHeight()/4.0/_hauteur;

    displayText->SetResize(_largeur*coef_resize_x,coef_resize_y);

    SBack.Resize(FENETREX*3/2.0,FENETREY*3/2.0);

    gui_spr[FOND_GUI].Resize(_largeur *coef_resize_x,_hauteur*coef_resize_y);
    gui_spr[CONTOUR_GUI].Resize(_largeur *coef_resize_x,_hauteur*coef_resize_y);

    gui_spr[VIE_GUI].Resize(201.0*coef_resize_x+2,20.0*coef_resize_y+2);
    gui_spr[MANA_GUI].Resize(201.0*coef_resize_x+2,20.0*coef_resize_y+2);
    gui_spr[SPRINT_GUI].Resize(93.0*coef_resize_x+2,20.0*coef_resize_y+2);
    gui_spr[EXP_GUI].Resize(525.0*coef_resize_x+2,6.0*coef_resize_y+3);

    for (unsigned int i=F1_GUI;i<=F10_GUI;++i)
        gui_spr[i].Resize(39.0*coef_resize_x,38.0*coef_resize_y);
    for (int i=F1_GUI; i<=F10_GUI;++i)
        perso->gui_recharges[i-F1_GUI]->Resize(gui_spr[i].GetSize().x+2,gui_spr[i].GetSize().y+2);
};


void Game::majGUI()      //mise � jour de la gui (exp, mana et vie)
{
    //mise a jour des valeur
    //mise � jour de la vie
    int percent =(100*perso->getVie()) / perso->getVieMax();
    gui_spr[VIE_GUI].SetSubRect(IntRect(0,0,(percent*226.0)/100.0,40.0));
    //MAJ du mana
    percent= (100*perso->getMana())/perso->getManaMax();
    gui_spr[MANA_GUI].SetSubRect(IntRect(0,0,(percent*226.0)/100.0,40.0));
    //MAJ du sprint
    int percentClock = (perso->clockSprint.GetRemainingTime()==-1)?0:(int)((100*(vectorSkills[SKILL_COURIR].duree-perso->clockSprint.GetRemainingTime()))/vectorSkills[SKILL_COURIR].duree);
    gui_spr[SPRINT_GUI].SetSubRect(IntRect(0,0,(percentClock*226.0)/100.0,40.0));
    //MAJ de l'exp
    percent=(100*perso->getXp())/perso->getXpMax();
    gui_spr[EXP_GUI].SetSubRect(IntRect(0,0,(percent*560.0)/100.0,6.0));

    //point servant d'origine de la gui
    Vector2f origine_gui=Vector2f(camera.GetCenter().x-camera.GetHalfSize().x-1,camera.GetCenter().y-camera.GetHalfSize().y); //position du coin superieur gauche
    //fond de la gui
    gui_spr[FOND_GUI].SetPosition(origine_gui);
    SBack.SetPosition(origine_gui.x - (FENETREX/3.0*(perso->GetPosition().x/MAPSIZEX)),origine_gui.y - (FENETREY/3.0*(perso->GetPosition().y/MAPSIZEY)));

    gui_spr[VIE_GUI].SetPosition(origine_gui.x+168.0*coef_resize_x-1,origine_gui.y+12.0*coef_resize_y);
    gui_spr[MANA_GUI].SetPosition(origine_gui.x+384.0*coef_resize_x-1,origine_gui.y+12.0*coef_resize_y);
    gui_spr[SPRINT_GUI].SetPosition(origine_gui.x+600.0*coef_resize_x,origine_gui.y+12.0*coef_resize_y);
    gui_spr[EXP_GUI].SetPosition(origine_gui.x+168.0*coef_resize_x-1,origine_gui.y+101.0*coef_resize_y);

    for (unsigned int i= F1_GUI; i<=F10_GUI;++i)
    {
        gui_spr[i].SetPosition(origine_gui.x+(168.0+(39+15)*(i-F1_GUI))*coef_resize_x,origine_gui.y+48.0*coef_resize_y);
        perso->gui_recharges[i-F1_GUI]->SetPosition(gui_spr[i].GetPosition());
    }

    gui_spr[CONTOUR_GUI].SetPosition(origine_gui);

    //anim indiquant des points de compétences disponibles
    if (perso->statsPerso[NB_POINTS_SKILLS]>0)
    {
        perso->animations[ANIM_PTS_COMP].SetPosition(gui_spr[FOND_GUI].GetPosition().x+gui_spr[FOND_GUI].GetSize().x-50,gui_spr[FOND_GUI].GetPosition().y+gui_spr[FOND_GUI].GetSize().y+30);
        if (perso->animations[ANIM_PTS_COMP].IsPaused())
            perso->animations[ANIM_PTS_COMP].Play();
    }
    else if (!perso->animations[ANIM_PTS_COMP].IsPaused())
        perso->animations[ANIM_PTS_COMP].Stop();


    //affichage des stats (vie, mana, xp) sur la gui
    int size=12*(FENETREX/800.0);
    //vie
    stringstream out;
    out << perso->getVie();
    out << "/"<< perso->getVieMax();
    gui_statsNbr[VIE_STR].SetText(out.str());
    gui_statsNbr[VIE_STR].SetSize(size);
    gui_statsNbr[VIE_STR].SetCenter(gui_statsNbr[VIE_STR].GetRect().GetWidth()/2,gui_statsNbr[VIE_STR].GetRect().GetHeight()/2);
    gui_statsNbr[VIE_STR].SetPosition(gui_spr[VIE_GUI].GetPosition().x + 100*coef_resize_x, gui_spr[VIE_GUI].GetPosition().y+10*coef_resize_y);
    out.str(string());

    //mana
    out << perso->getMana();
    out << "/"<< perso->getManaMax();
    gui_statsNbr[MANA_STR].SetText(out.str());
    gui_statsNbr[MANA_STR].SetSize(size);
    gui_statsNbr[MANA_STR].SetCenter(gui_statsNbr[MANA_STR].GetRect().GetWidth()/2,gui_statsNbr[MANA_STR].GetRect().GetHeight()/2);
    gui_statsNbr[MANA_STR].SetPosition(gui_spr[MANA_GUI].GetPosition().x + 100*coef_resize_x, gui_spr[MANA_GUI].GetPosition().y+10*coef_resize_y);
    out.str(string());

    //sprint
    out << percentClock << "%";
    gui_statsNbr[SPRINT_STR].SetText(out.str());
    gui_statsNbr[SPRINT_STR].SetSize(size);
    gui_statsNbr[SPRINT_STR].SetCenter(gui_statsNbr[SPRINT_STR].GetRect().GetWidth()/2,gui_statsNbr[SPRINT_STR].GetRect().GetHeight()/2);
    gui_statsNbr[SPRINT_STR].SetPosition(gui_spr[SPRINT_GUI].GetPosition().x +46.5*coef_resize_x, gui_spr[SPRINT_GUI].GetPosition().y+10*coef_resize_y);
    out.str(string());

    //niveau
    level.SetPosition(origine_gui.x+19*coef_resize_x,origine_gui.y+16*coef_resize_y);
    level.SetSize(size);

    //vies
    gui_vies.SetPosition(origine_gui.x+106*coef_resize_x,origine_gui.y+16*coef_resize_y);
    gui_vies.SetSize(size);

    //affichage du temps du niveau
    gui_time_lvl.SetPosition(origine_gui.x+106*coef_resize_x,origine_gui.y+52*coef_resize_y);
    gui_time_lvl.SetSize(size);

    //or
    money.SetPosition(origine_gui.x+106*coef_resize_x,origine_gui.y+83*coef_resize_y);
    money.SetSize(size);
};


void Game::loadMap(const char* nomf,_sauvegarde* loadGame,_saveHeros *oldHero)
 {
        /****************************************
    *           STRUCTURE DU FICHIER:       *
    *                                       *
    *       -i nombre d'images pour sprite  *
    *       -i*img  chemin relatif du sprite*
    *                                       *
    *       -i nombre de sprite             *
    *       -i*spr  sprite avec "pointer"   *
    *               sur le tableau d'image  *
    *               precedent et coodon�es  *
    *                                       *
    *       -i nombre d'images pour les PNJ *
    *       -i*img chemin relatif des images*
    *                                       *
    *       -i nombre de PNJ                *
    *       -i _PNJ_    PNJ avec pointer sur*
    *                   le tableau d'images *
    *                   precedent           *
    *                                       *
    *       -i nombre d'images pour les OBJ *
    *       -i img  chemin relatif des image*
    *                                       *
    *       -i nombre d'OBJ                 *
    *       -i _OBJ_ OBJ avec pointer sur le*
    *                pointeur su le tableau *
    *                d'image precedent      *
    *                                       *
    *       -i nombre d'Hero                *
    *       -i _PNJ_ PNJ avec pointer sur le*
    *                pointeur su le tableau *
    *                d'image precedent      *
    *                                       *
    *       -option de la carte             *
    ****************************************/
    FILE* Fichier=NULL;
    Fichier=fopen(nomf,"rb");
    std::vector<sf::Sprite> m_floors;

    varQueteLvl=0;

    img_mainObjets.LoadFromFile("./resources/image/allItems.png");

    for (int i=0;i<MAPSIZECARTEY;++i)
    {
        for (int j=0;j<MAPSIZECARTEX;j++)
        {
            tab_collision[i][j]=false;
        }
    }

    //tableau des Sprite initialisation
    for (int i=0;i<MAPSIZECARTEY;++i)
    {
        for (int j=0;j<MAPSIZECARTEX;j++)
        {
            tab_sprite[i][j]=sprite;
        }
    }

    if (Fichier !=NULL)
    {
         spr s; //srites
         img im; //images
         unsigned_int i; //nombre


         /*************************************
         *  Chargement des images du decort   *
         *************************************/
         fread(&i,sizeof(i),1,Fichier); //recuperation du nombre d'image dans le fichier
         resources_image.clear();
         resources_image.resize(100);
         resources_image.clear();
          string nom="";
        for (unsigned int j=0;j<i.i;j++) //recuperation du nom des images
        {
            fread(&im,sizeof(img),1,Fichier);
            nom=im.nom;

            if (nom.find("rien.png") >nom.size()   and  nom.find("X.png") >nom.size() )
            {
                 Image image;
                 image.LoadFromFile(im.nom);

                 resources_image.push_back(image); //faire ici le push back dans le tableau des images
            }
        }
        /****************************************
        *    Chargement des sprite du decort    *
        ****************************************/
        fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sprite dans le fichier
        for (unsigned int j=0;j<i.i;j++) //recuperation des sprites
        {

            fread(&s,sizeof(s),1,Fichier);
            bool rien=false;

            if(s.img>=resources_image.size())
            {
             rien=true;
             s.img=resources_image.size()-1;
            }
            else if (s.img<0)
                s.img=0;


            if (!rien)
            {
                 Sprite floor(resources_image[s.img],Vector2f(s.x,s.y));
                 if (s.d==32 && s.h==0 && s.g==2*32 && s.b==32){ // plateforme du milieu haut
                    if (Randomizer::Random(1,100)>=90)
                    {
                        s.h=3*32;
                        s.b=4*32;
                    }
                 }else if (s.d==32 && s.h==2*32 && s.g==2*32 && s.b==3*32){ // plateforme du milieu 32,2*32,2*32,3*32
                    if (Randomizer::Random(1,100)>=95)
                    {
                        s.h=32;
                        s.b=2*32;
                    }
                 }
                 floor.SetSubRect(IntRect(s.d,s.h,s.g,s.b));
                 m_floors.push_back(floor); //faire ici le push_back du sprite
                 tab_sprite[(int)(s.y/TAILLEBLOC)][(int)(s.x/TAILLEBLOC)]=m_floors[m_floors.size()-1];
            }
            /************************************
            *   tableau de collision            *
            ************************************/
            if (!s.colision)
                tab_collision[(int)(s.y/TAILLEBLOC)][(int)(s.x/TAILLEBLOC)]=true; //mise en place du bool pour dire qu'il y a une colision
        }

        /**************************************
        * Optimisation du tableau de collision*
        **************************************/

        bool old_tab[MAPSIZECARTEY][MAPSIZECARTEX];

        for( int Y=0;Y<MAPSIZECARTEY;Y++)
        {
             for (int X=0;X<MAPSIZECARTEX;X++)
             {
                 old_tab[Y][X]=tab_collision[Y][X];

             }
        }

        for(int Y=MAPSIZECARTEY-1;Y>0;Y--)
        {
            for (int X=0;X<MAPSIZECARTEX;X++)
            {
                if (old_tab[Y][X] ) //si il y a une collision
                    if (old_tab[Y][X-1]  and  X>1  and  old_tab[Y-1][X]  and  old_tab[Y][X+1])   // si il y avait une collision a gauche et une collision en haut et une � droite
                        if(!(!old_tab[Y-1][X+1]  or !old_tab[Y-1][X-1])) // si il y avait pas de colision la case du dessu gauche ou dessus droit  et une collision au dessus
                            tab_collision[Y][X]=false;
            }
        }
        for(int Y=MAPSIZECARTEY-1;Y>0;Y--)
            for (int X=0;X<MAPSIZECARTEX;X++)
            {
                if (tab_collision[Y][X])
                {
                    int _size=tab_sols.size(),x;
                    bool ajout=false;

                   /* if (tab_collision[Y-1][X]  and  Y-1 >0)        //si il y a une colision au dessus
                    {
                        tab_sols.push_back(Floor(Vector2f(X*TAILLEBLOC,Y*TAILLEBLOC),TAILLEBLOC,90)); //on ajoute un sol
                        ajout=true;
                    }*/


                    if(tab_collision[Y][X+1]  and  X+1<MAPSIZECARTEX) //si il y a une collision a droite
                    {
                        _size=tab_sols.size();
                        tab_sols.push_back(Floor(Vector2f(X*TAILLEBLOC,Y*TAILLEBLOC),TAILLEBLOC)); //on ajoute un sol
                        ajout=true;

                        for (x=1;X+x<MAPSIZECARTEX  and  tab_collision[Y][X+x];x++)    //et tant qu'il y a une collision �) droite
                            tab_sols[_size].AgrandirDroit(TAILLEBLOC);   //on agrandis le sol
                        X+=x;

                    }

                   /* if(!ajout)  //si element isol�
                    {
                        if (old_tab[Y][X+1]  and  X+1<MAPSIZECARTEX)
                            tab_sols.push_back(Floor(Vector2f(X*TAILLEBLOC,(Y+1)*TAILLEBLOC),TAILLEBLOC,Color::Red,90));
                        if ((!old_tab[Y][X-1])  and  X-1 >0)
                            tab_sols.push_back(Floor(Vector2f(X*TAILLEBLOC,(Y+1)*TAILLEBLOC),TAILLEBLOC,Color::Red,90));

                        //tab_sols.push_back(Floor(Vector2f(X*TAILLEBLOC,(Y+1)*TAILLEBLOC),TAILLEBLOC,Color::Red,90));
                    }*/
                }

            }
     for(int Y=MAPSIZECARTEY-1;Y>0;Y--)
        for (int X=0;X<MAPSIZECARTEX;X++)
            if (tab_collision[Y][X] and (!tab_collision[Y+1][X] and !tab_collision[Y-1][X] and !tab_collision[Y][X+1] and !tab_collision[Y][X-1])) //si c'est un element seul
                tab_collision[Y][X]=false;
        /*tab_sols.push_back(Floor(Vector2f(5*TAILLEBLOC,(MAPSIZECARTEY-5)*TAILLEBLOC),TAILLEBLOC));
        tab_sols[0].AgrandirDroit(TAILLEBLOC);
        tab_sols[0].AgrandirGauche(TAILLEBLOC);*/


        /********************************
        *   Images des PNJ              *
        ********************************/
        fread(&i,sizeof(i),1,Fichier); //recuperation du nombre d'image dans le fichier
        resources_image_pnj_obj.clear();
        resources_image_pnj_obj.resize(100);
        resources_image_pnj_obj.clear();

        for (unsigned int j=0;j<i.i;j++)
        {
            fread(&im,sizeof(img),1,Fichier);
            nom=im.nom;

            Image image;
            image.LoadFromFile(im.nom);

            resources_image_pnj_obj.push_back(image); //faire ici le push back dans le tableau des images
        }

        /*******************************
        *   Sprites des pnj            *
        *******************************/
        fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sprite dans le fichier
        mobs.clear();
        _PNJ_ struc_mob;

        for (unsigned int j=0;j<i.i;j++) //recuperation des sprites
        {
            fread(&struc_mob,sizeof(_PNJ_),1,Fichier);

            struc_mob.x+=16;

            if (struc_mob.img > resources_image_pnj_obj.size()  or  struc_mob.img <0 )
                struc_mob.img=0;

            mobs.push_back(Pnj (resources_image_pnj_obj[struc_mob.img],struc_mob));


            //si load game
            if (loadGame!=NULL)
                mobs[mobs.size()-1].setEnable(loadGame->mobs[j]);

            //variable de quete du niveau
            if(struc_mob.quette>0)
                ++varQueteLvl;
        }

        /*******************************
        *       Images des obj         *
        *******************************/
        fread(&i,sizeof(i),1,Fichier); //recuperation du nombre d'image dans le fichier

        unsigned int emplacement_obj=resources_image_pnj_obj.size();

        for (unsigned int j=0;j<i.i;j++)
        {
            fread(&im,sizeof(img),1,Fichier);
            nom=im.nom;
            Image image;
            image.LoadFromFile(im.nom);
            resources_image_pnj_obj.push_back(image); //faire ici le push back dans le tableau des image
        }

        /*******************************
        *   sprite des obj             *
        *******************************/
         fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sprite dans le fichier
         objets.clear();
        _OBJ_ struc_obj;

         for (unsigned int j=0;j<i.i;j++)
         {
             fread(&struc_obj,sizeof(struc_obj),1,Fichier);

             struc_obj.x+=16;

             if(emplacement_obj+struc_obj.img > resources_image_pnj_obj.size()  or  emplacement_obj+struc_obj.img <0)
                struc_obj.img =0;


            struc_obj.tx = 4;
            objets.push_back(Objet (resources_image_pnj_obj[emplacement_obj+struc_obj.img],struc_obj));     //sinon

            //variable de qu�te du niveau
            if (struc_obj.quette>0)
                ++varQueteLvl;
         }


         /*******************************
        *            SKILLS            *
        *******************************/
        FILE* file = NULL;
        file = fopen("./data/skills", "rb");
        _skills skills;
        if (file != NULL)
        {
            //on lit dans le fichiers des skills
            fread(&skills,sizeof(skills),1,file);

            while (!feof(file))
            {
                vectorSkills.push_back(skills);
                fread(&skills,sizeof(skills),1,file);
            }
            fclose(file);
        }


         /******************************
         *          HEROS              *
         ******************************/
         fread(&i,sizeof(i),1,Fichier); //lire le nombre de heros (ici normalement � 1)
         heros struc_hero;

         for (unsigned int j=0;j<i.i;j++)
            fread(&struc_hero,sizeof(heros),1,Fichier);

          perso = new Hero(Vector2f(struc_hero.x*TAILLEBLOC,struc_hero.y*TAILLEBLOC + 32),oldHero,sounds,touches,ETAT,&vectorSkills);

          //si load game
          if (loadGame!=NULL)
          {
              loadPerso(loadGame);
          }



        /*******************************
        *       OPTION DE LA CARTE     *
        *******************************/
        options_map map;

        fread(&map,sizeof(options_map),1,Fichier);

        string dial=map.AutoDialogue;
        if (dial !="" and loadGame==NULL)
            perso->setMetDialogue(map.AutoDialogue);

        //image de fond
        if (!back.LoadFromFile(map.fond))
            back.LoadFromFile("./resources/image/fonds/Foret5.jpg");

        //musique
        if (!musicLvl.OpenFromFile(map.sons))
            musicLvl.OpenFromFile("./musique/lvl1.ogg");
        musicLvl.SetRelativeToListener(true);

        //v�rification du nom de level, anti cheat
        //exemple : si le joueur � renommer le level 9 en "1", et bien dans la structure du level on indique le nom r�el du fichier (ici 9)
        string nomReel("./lvl/");
        nomReel += map.nomVerif;
        string nomFichier(nomf);
        if (nomReel != nomFichier) //si le joueur � cheater
        {
            (*ETAT) = CHEAT;
        }

        //enregistrements des noms
        infosLevel[NOM_LEVEL] = map.nomMap;
        infosLevel[NOM_VERIF_LEVEL] = map.nomVerif;
        infosLevel[MYTHOLOGIE_LEVEL] = map.mythologie;
        infosLevel[TYPE_LEVEL] = map.Type;


        for (vector<Pnj>::iterator mobs_iter=mobs.begin();mobs_iter < mobs.end();mobs_iter++)
        {
            mobs_iter->SetDirection(mobs_iter->GetDirection());
            mobs_iter->SetGravite(map.gravitee/10);
            mobs_iter->SetFrottement(map.frottement);
        }

        perso->SetGravite(map.gravitee/10);
        //map.frottement=2000;
        perso->SetFrottement(map.frottement);

        for (unsigned int i=0; i<objets.size(); ++i)
        {
            objets[i].SetAnim(&objets[i].GoLeft);
            objets[i].SetDirection(objets[i].GetDirection());
            if (objets[i] == PORTAIL)
            {
                if (varQueteLvl > 0)
                    objets[i].Pause();
                perso->setPortail(&(objets[i]));
            }
        }

        clockNiveaux.SetTime(map.chrono);
        clockNiveaux.Pause();
        levelWithChrono = clockNiveaux.GetRemainingTime() > -1;



        /*******************************
        *        SONS SPACIALISES      *
        *******************************/
        fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sons dans le fichier
        if (feof(Fichier)) goto end;

            effetsSonores.clear();
            _sons_spacialises snd;

            for (unsigned int j=0;j<i.i;j++)
            {
                //on lit dans la structure
                fread(&snd,sizeof(_sons_spacialises),1,Fichier);

                //push back
                int casee = effetsSonores.size();
                effetsSonores.push_back(new Music());
                effetsSonores[casee]->OpenFromFile(string(snd.dossier) + string(snd.nom));
                effetsSonores[casee]->SetLoop(true);
                effetsSonores[casee]->SetVolume(snd.volume/10.0 * settings->volumeEffetsSonores/100.0);
                volumesEffetsSonores.push_back(float(snd.volume/10.0));
                effetsSonores[casee]->SetMinDistance(snd.minDistance/10.0);
                effetsSonores[casee]->SetAttenuation(snd.attenuation/10.0);
                effetsSonores[casee]->SetPosition(snd.x,snd.y,0);
                effetsSonores[casee]->Play();
                perso->listener.SetPosition(perso->GetPosition().x, perso->GetPosition().y, 0);
            }

        end:
        fclose(Fichier);
    }
    else
    {
        cerr<<"Impossible d'ouvrir le fichier "<<nomf<<endl;
    }

    perso->setQuetteLvl(varQueteLvl);

    //perso->setQuette(varQueteLvl);
}


void Game::loadPerso(_sauvegarde* loadGame)
{
    perso->initStats(&(loadGame->saveHeros));
    perso->SetPosition(loadGame->posXheros,loadGame->posYheros);
    perso->SetDirection(loadGame->direction);

    //parchemins
    for (unsigned int i=0; i<objets.size(); ++i)
    {
      //si c'est un parchemin et qu'il est disable, alors il apartenait au h�ros
      if (objets[i].getType() == PARCHEMIN  and  !objets[i].isEnable())
        perso->listeParchemins.push_back(objets[i]);
    }
    //var quete
    perso->setQuette(loadGame->varQuete);
}

Game::~Game()
{

    delete parchemin;
    delete menupause;
    delete marchand;


    for (unsigned int i=0;i< effetsSonores.size();++i)
        delete effetsSonores[i];
    effetsSonores.clear();

    delete displayText;

    delete perso;


    //delete menusauvegarde;
};
