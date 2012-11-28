#include "WindowGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;


WindowGame::WindowGame(RenderWindow* Window, _options* setting, _touches_heros* touchesHeros, _touches_sys* touches_Sys)
{
    App = Window;
    settings = setting;
    touches = touchesHeros;
    touchesSys = touches_Sys;

    ETAT = MENUED;

    img_gameover.LoadFromFile("./resources/FinJeu/GameOver.png");
    spr_gameover.SetImage(img_gameover);
    spr_gameover.Resize(FENETREX,FENETREY);

    levelCourant = 0;

    son = new Sons(settings);
    menu = new Menu(App,settings,son,touchesSys);
}

void WindowGame::run()
{
    if (ETAT >= GAMED)
    {
        if (!game->getWaitForKey()) {
            App->Clear();
        }

        if (game->getExitGame())
        {
            ETAT = MENUED;
            menu = new Menu(App,settings,son,touchesSys);
            delete game;
            levelCourant=0;
            App->SetView(App->GetDefaultView());
        }
    }
    else
        App->Clear();

    //actualisation du jeu
    if (ETAT >= GAMED  or  ETAT == LOADING_GAME)
    {
        game->run();

        //si on doit changer de level
        if (game->perso->getLvlFinish())
            changeLevel();
    }
    else if (ETAT == GAME_OVER)
    {
        gameOver();
    }
    else if (ETAT == CHEAT)
    {
        cheatByeBye();
    }
    else if (ETAT == MENUED) //si on est dans le menu
    {
        menu->run();
        if (menu->getLaunchGame()) //on lance le jeu
        {
            ETAT = LOADING_GAME;
            game = new Game(App,settings,son,touches,touchesSys,&ETAT,&levelCourant);
            delete menu;
        }
        else if (menu->getLaunchLoad())
        {
            ETAT = LOADED;
            menuSave = new MenuSauvegarde(touchesSys,son,true);
            delete menu;
        }
    }
    else if (ETAT == LOADED)
    {
        menuSave->Draw(*App);
    }

    App->Display();
}

inline void WindowGame::gameOver()
{
    static int color=0;

    App->SetView(App->GetDefaultView());

    //Effet game over : 1ère partie
    if (color < 250)
    {
        color+=3;
        spr_gameover.SetColor(Color(color,color,color));
    }

    App->Draw(spr_gameover);

    //si l'effet est finit
    if (color>=250)
    {
        ETAT = MENUED;
        delete game;
        levelCourant=0;
        menu = new Menu(App,settings,son,touchesSys);
        color=0;
    }
}

inline void WindowGame::cheatByeBye()
{
    static ReverseClock clock;
    static bool display = true;
    static int red = 0;
    static int green = 255;
    static int blue = 0;

    //premier tour : on initialise l'horloge et la vue
    if (clock.IsPaused())
    {
        clock.SetTime(10);
        delete game;
        App->SetView(App->GetDefaultView());
    }

    //écriture de l'insulte
    String insulte("Mother Fuckin' Cheater !");
    insulte.SetColor(Color(red+=10,green-=10,blue+=20));
    insulte.SetCenter(insulte.GetRect().GetWidth()/2,insulte.GetRect().GetHeight()/2);
    insulte.SetPosition(FENETREX/2,FENETREY/2);
    display = !display;
    App->SetCursorPosition(0,0);

    if (display)
    {
        Color colorInsulte = insulte.GetColor();
        App->Clear(Color(255-colorInsulte.r,255-colorInsulte.g,255-colorInsulte.b));
        App->Draw(insulte);
    }

    //si le temps est écoulé, on quitte
    if (clock.IsPlaying()  and clock.GetRemainingTime() < 1)
        App->Close();
}


void WindowGame::eventKey(Event event)
{
    if (ETAT == GAMED  and  event.Key.Code == touches->pause)
    {
        if (!game->getWaitForKey())
            ETAT = PAUSED;
    }

    if (ETAT == GAMED)
    {
        if (game->getWaitForKey()  and  (event.Key.Code == touchesSys->retour  or  event.Key.Code == touchesSys->entree))
        {
            game->setWaitForKey(false);
            game->unpauseAfterDial();
            game->Reset();
        }
    }

    //return key
    if (event.Key.Code == touchesSys->retour)
        returnKey();

    if (ETAT == MENUED)
        menu->eventKey(event);
    else if (ETAT == PAUSED)
    {
        game->menupause->getEvents(event);
    }
    else if (ETAT == PARCHEMINED)
        game->parchemin->getEvents(event);
    else if (ETAT == SAVED)
    {
        game->menusauvegarde->getEvents(event);
    }
    else if (ETAT == LOADED)
    {
        menuSave->getEvents(event);

        //demande de chargement de partie
        if (menuSave->getLoadGame())
        {
            menuSave->LoadGame(&save,touches);
            ETAT = LOADING_GAME;
            game = new Game(App,settings,son,touches,touchesSys,&ETAT,&levelCourant,&save);
            delete menuSave;
        }

    }
}


////////////////////////////////////////////
/// Events : return key
////////////////////////////////////////////
inline void WindowGame::returnKey()
{
    switch (ETAT)
    {
        case SAVED :
            if (game->menusauvegarde->save_state == game->menusauvegarde->STATE_NONE)
            {
                game->unpauseGame();
                ETAT = GAMED;
            }
            break;

        case LOADED :
            ETAT = MENUED;
            delete menuSave;
            menu = new Menu(App,settings,son,touchesSys);
            break;

        case PARCHEMINED :
            ETAT = GAMED;
            game->unpauseGame();
            break;

        case MENUED :
            App->Close();
            break;

        default :
            break;
    }
}

void WindowGame::changeLevel()
{
    //sauvegarde des stats
    for (int i=0; i<TAILLE_STATS; ++i)
        saveForLevelSvt.statsPerso[i] = game->perso->getStats()[i];

    //sauvegarde des scores
    for (int i=0; i<TAILLE_SCORES; ++i)
        saveForLevelSvt.scoresPerso[i] = game->perso->getScores()[i];

    //sauvegarde de l'xp
    for (int i=0; i<TAILLE_EXP; ++i)
        saveForLevelSvt.xpPerso[i] = game->perso->getExp()[i];

    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
        saveForLevelSvt.tab_objets[i] = game->perso->tab_objets[i];
    saveForLevelSvt.tab_objets[PARCHEMIN] = 0;

    saveForLevelSvt.vie = game->perso->getVie();
    saveForLevelSvt.mana = game->perso->getMana();

    for (int i=0; i<TAILLE_SKILLS; ++i)
        saveForLevelSvt.tabSkills[i] = game->perso->skillsPerso[i];

    //temps de jeu
    saveForLevelSvt.totalGameTime = game->getTotalGameTime();

    //quand on a tout sauvegardé, on delete le jeu
    delete game;

    //on relance le jeu, nouvelle partie à false par défaut
    ETAT = LOADING_GAME;
    game = new Game(App,settings,son,touches,touchesSys,&ETAT,&levelCourant,NULL,&saveForLevelSvt);

    //game->loadMap("./lvl/2",&saveForLevelSvt);
}

