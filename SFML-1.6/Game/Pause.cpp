#include "Pause.hpp"
#include "string.h"

using namespace sf;
using namespace std;

Pause::Pause(int stats[], int scores[], int xp[], int vie, int mana, String tab_nbobjets[NB_OBJETS], bool skills[], _options* setting, _touches_heros* touchesHeros, _touches_sys* touches_Sys, Sons* so, vector<_skills>* vectorSkills, int level)
{
    son = so;
    settings = setting;
    touches = touchesHeros;
    touchesSys = touches_Sys;
    levelCourant = level;

    font.LoadFromFile("./fonts/BlackCastleMF.ttf");

    //skills
    initVariables();
    for (unsigned int i=0; i<vectorSkills->size(); ++i)
    {
        //le titre
        titreSkill[i]=String(convertText((*vectorSkills)[i].nom));

        //l'image
        imgSkill[i] = Image ();
        imgSkill[i].LoadFromFile((*vectorSkills)[i].img);
        sprSkill[i] = Sprite ();
        sprSkill[i].SetImage(imgSkill[i]);

        //description
        texteSkill[i] = String(convertText((*vectorSkills)[i].description));

        //id
        idSkill[i] = (*vectorSkills)[i].ID;

        //mana
        stringstream out;
        out << (*vectorSkills)[i].mana;
        manaSkill[i] = String(out.str());

        //duree
        out.str(string());
        if ((*vectorSkills)[i].duree == 0)
            out << "Aucune";
        else
            out << (*vectorSkills)[i].duree << " sec";
        dureeSkill[i] = String(out.str());

        //recharge
        out.str(string());
        if ((*vectorSkills)[i].recharge == 0)
            out << "Immediate";
        else
            out << (*vectorSkills)[i].recharge << " sec";
        rechargeSkill[i] = String(out.str());
    }

    majaData(stats,scores,xp,vie,mana,tab_nbobjets,skills);

    initChoix();
    initImages();

    initMenuStatus();
    initMenuScores();
    initMenuInventaire();
    initMenuSkills();
    initMenuOption();
    initBoiteConnection();
};

/*****************************************************************************
 * Met à jour les sprites de la barre de raccourcis, notemment pour les skills
 *****************************************************************************/
void Pause::majBS(Sprite guiSpr[TAILLE_GUI])
{
    //on parcours le tableau de la barre de raccourcis
    for (int i=0; i<TAILLE_RACCOURCIS_BARRE_SORTS; ++i)
    {
        //si c'est un skill
        if (!touches->tabBarreSortsObj[i])
        {
            //on redéfinit l'image
            guiSpr[F1_GUI+i].SetImage(imgSkill[touches->tabBarreSortsId[i]]);
        }
    }
};

void Pause::majaData(int stats[], int scores[], int xp[], int vie, int mana, String tab_nbobjets[NB_OBJETS], bool skills[])
{
    initVariables();

    //tab des valeurs des stats
    for (int i=0; i<TAILLE_STATS; ++i)
    {
        stringstream out;
        out << stats[i];
        statsPersoValues1[i].SetText(out.str());
    }

    stringstream out;
    out << vie;
    statsPersoValues2[VIE].SetText(out.str());
    out.str(string());
    out << mana;
    statsPersoValues2[MANA].SetText(out.str());
    out.str(string());

    for (int i=XP; i<TAILLE_EXP; ++i)
    {
        stringstream out;
        if (i==XPLEVEL)
            out << xp[XPLEVEL]-xp[XP];
        else
            out << xp[i];
        statsPersoValues2[i].SetText(out.str());
    }

    //inventaire
    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
    {
        nbObjetsInventaire[i].SetText(tab_nbobjets[i].GetText());
        nbObjetsInventaire[i].SetSize(20);
        nbObjetsInventaire[i].SetPosition(inventaire[i].GetPosition());
    }

    //scores
    for (int i=0; i<TAILLE_SCORES; ++i)
    {
        stringstream out;
        out << abs(scores[i]);
        scoresPersoValues[i].SetText(out.str());
    }

    //skills du heros
    nbSkillsMaitrises=0;
    for (int i=0; i<TAILLE_SKILLS; ++i)
    {
        displaySkill[i]=skills[i];
        if (displaySkill[i]) ++nbSkillsMaitrises;
    }
    //on place l'utilisateur sur le premier skill dispo si il y en a
    if (nbSkillsMaitrises>0)
    {
        bool found=false;
        while (!found)
        {
            if (displaySkill[compteurSkillReel])
                found=true;
            else
                ++compteurSkillReel;
        }
        changeCurrentSkill();
    }
};

void Pause::initVariables()
{
    exit=false;
    gameExited=false;
    changeOptions=false;
    touchesHerosMenued=false;
    touchesSysMenued=false;
    waitForTouche=false;
    waitForTouchePressed=false;
    barreSortMenued=false;
    connexionSite=false;
    responseSite=false;
    choixActuel=0;
    compteurSkillReel=0;
    compteurSkillDisplayed=1;
    idReelSkillCourant=0;
    widgetGaucheX=500 * FENETREX/800.0;
    screen=RIGHT_SCREEN;
    taillePolice=17;
    nbCharParLigne=widgetGaucheX/((taillePolice*FENETREX/800.0)*0.4);

    for (int i=0; i<NBSTATS; ++i)
        tabPlusMoins[i]=0;

    //descriptif objets
    txtExplicatifInventaire = "Appuyez sur " + findKey(touchesSys->entree) + " pour selectionner un objet...";
    descObjTemp.SetText(txtExplicatifInventaire);
    descObjTemp.SetStyle(String::Italic);
    descObjTemp.SetFont(font);
    descObjTemp.SetPosition(10,FENETREY-200);
    descObjTemp.SetSize(20);

    //raccourcis barres sorts : skills
    txtExplicatifRaccourcisSkills = "Appuyez sur " + findKey(touchesSys->entree) + " pour placer le skill dans la barre d'action.\n\nRaccourcis : ";
    raccourcisTemp.SetFont(font);
    raccourcisTemp.SetPosition(descObjTemp.GetPosition().x,FENETREY-100);
    raccourcisTemp.SetSize(20);
    raccourcisTemp.SetStyle(String::Italic);
    raccourcisTemp.SetColor(Color::Black);

    txtExplicatifRaccourcisObj = "Appuyez sur " + findKey(touchesSys->entree) + " pour placer l'objet dans la barre d'action.\n\nRaccourcis : ";
    raccourcisObjTemp.SetFont(font);
    raccourcisObjTemp.SetPosition(descObjTemp.GetPosition().x,descObjTemp.GetPosition().y+100);
    raccourcisObjTemp.SetSize(20);
    raccourcisObjTemp.SetStyle(String::Italic);
    raccourcisObjTemp.SetColor(Color::Black);
};

inline void Pause::majTextesConnection(string txt, bool pseudo)
{

}

void Pause::initChoix()
{
    //choix des menus
    choix[STATUS].SetText("Status");
    choix[SCORES].SetText("Scores");
    choix[SKILLS].SetText("Skills");
    choix[INVENTAIRE].SetText("Inventaire");
    choix[OPTIONS].SetText("Options");
    choix[RETOUR].SetText("Retour");
    choix[QUITTER].SetText("Quitter");

    for (int i=0; i<TAILLE_CHOIX_PAUSE; ++i)
        choix[i].SetSize(taillePolice * FENETREX/800.0 + 4);

    int sizePlusGrandChoixX = choix[INVENTAIRE].GetRect().GetWidth();
    int posChoixX = FENETREX-sizePlusGrandChoixX*1.5;
    int posY = 0;

    for (int i=0; i<TAILLE_CHOIX_PAUSE; ++i)
    {
        choix[i].SetFont(font);
        choix[i].SetPosition(posChoixX,posY+=choix[i].GetSize()*1.5);
    }
};


void Pause::initMenuStatus()
{
    //tab des stats
    statsPerso1[NB_POINTS_SKILLS].SetText("Points de competences :");
    statsPerso1[ATTAQUE].SetText("Attaque :");
    statsPerso1[DEFENSE].SetText("Defense :");
    statsPerso1[VITESSE].SetText("Vitesse :");
    statsPerso1[ENDURANCE].SetText("Endurance :");
    statsPerso1[INTELLIGENCE].SetText("Intelligence :");
    statsPerso1[LEVEL].SetText("Level :");

    for (int i=0; i<TAILLE_STATS-1; ++i)
    {
        statsPerso1[i].SetSize(taillePolice * FENETREX/800.0);
        statsPersoValues1[i].SetSize(taillePolice * FENETREX/800.0);
    }

    int sizePlusGrandChoixX = statsPerso1[INTELLIGENCE].GetRect().GetWidth()+50;
    int posChoixX = widgetGaucheX-sizePlusGrandChoixX;
    int posY = 20;

    statsPerso1[NB_POINTS_SKILLS].SetFont(font);
    statsPerso1[NB_POINTS_SKILLS].SetPosition(widgetGaucheX/2,posY);
    statsPersoValues1[NB_POINTS_SKILLS].SetFont(font);
    statsPersoValues1[NB_POINTS_SKILLS].SetPosition(statsPerso1[NB_POINTS_SKILLS].GetPosition().x+statsPerso1[NB_POINTS_SKILLS].GetRect().GetWidth()+10,posY);
    for (int i=1; i<TAILLE_STATS-1; ++i)
    {
        statsPerso1[i].SetFont(font);
        statsPerso1[i].SetPosition(posChoixX,posY+=1.5*statsPerso1[i].GetRect().GetHeight());
        statsPersoValues1[i].SetFont(font);
        statsPersoValues1[i].SetPosition(widgetGaucheX-50,posY);
    }
    statsPerso1[LEVEL].SetFont(font);
    statsPerso1[LEVEL].SetPosition(widgetGaucheX/2,posY+=50);
    statsPerso1[LEVEL].SetSize(45 * FENETREX/800.0);
    statsPersoValues1[LEVEL].SetFont(font);
    statsPersoValues1[LEVEL].SetPosition(statsPerso1[LEVEL].GetPosition().x+statsPerso1[LEVEL].GetRect().GetWidth()+10,posY);
    statsPersoValues1[LEVEL].SetSize(45 * FENETREX/800.0);

    //deuxieme tableau
    statsPerso2[VIE].SetText("Vie :");
    statsPerso2[MANA].SetText("Mana :");
    statsPerso2[XP].SetText("Experience :");
    statsPerso2[XPLEVEL].SetText("Niveau suivant :");

    for (int i=0; i<TAILLE_EXP; ++i)
    {
        statsPerso2[i].SetSize(taillePolice * FENETREX/800.0);
        statsPersoValues2[i].SetSize(taillePolice * FENETREX/800.0);
    }

    sizePlusGrandChoixX = statsPerso2[XPLEVEL].GetRect().GetWidth();
    posChoixX = 50;
    posY += 50;

    for (int i=0; i<(TAILLE_EXP); ++i)
    {
        statsPerso2[i].SetFont(font);
        statsPerso2[i].SetPosition(posChoixX,posY+=1.5*statsPerso2[i].GetRect().GetHeight());
        statsPersoValues2[i].SetFont(font);
        statsPersoValues2[i].SetPosition(posChoixX+sizePlusGrandChoixX+50,posY);
    }

};

void Pause::initMenuScores()
{
    initTitreMenu(&titleScores,String("~ Scores ~"));

    scoresPerso[NB_MOBS_KILL].SetText("Monstres tues :");
    scoresPerso[NB_MOBS_BOSS_KILL].SetText("Monstres boss tues :");
    scoresPerso[XP_TOTAL].SetText("Experience totale :");
    scoresPerso[DEGATS_TOTAL].SetText("Degats infliges :");
    scoresPerso[DEGATS_BEST].SetText("Degats maximum :");
    scoresPerso[VIE_LOSE].SetText("Vie perdue :");
    scoresPerso[MANA_LOSE].SetText("Mana perdue :");

    upload.SetText(findKey(touchesSys->entree) + " pour partager sur le site.");
    upload.SetSize(taillePolice * FENETREX/800.0);
    upload.SetCenter(upload.GetRect().GetWidth()/2.0,upload.GetRect().GetHeight());
    upload.SetPosition(widgetGaucheX/2.0,FENETREY-2*(upload.GetRect().GetHeight()));
    upload.SetFont(font);
    upload.SetColor(Color(140,0,0));
    upload.SetStyle(String::Underlined | String::Italic);

    for (int i=0; i<TAILLE_SCORES; ++i)
    {
        scoresPerso[i].SetSize(taillePolice * FENETREX/800.0);
        scoresPersoValues[i].SetSize(taillePolice * FENETREX/800.0);
    }

    int sizePlusGrandChoixX = scoresPerso[DEGATS_TOTAL].GetRect().GetWidth()+50;
    int posX = 50;
    int posY = 100;

    for (int i=0; i<TAILLE_SCORES; ++i)
    {
        scoresPerso[i].SetFont(font);
        scoresPerso[i].SetPosition(posX,posY+=1.5*scoresPerso[i].GetRect().GetHeight());
        scoresPersoValues[i].SetFont(font);
        scoresPersoValues[i].SetPosition(posX+sizePlusGrandChoixX+50,posY);
    }
};

void Pause::initMenuSkills()
{
    //titre de la catégorie
    initTitreMenu(&titleSkills,String("~ Skills ~"));

    //skill précédent
    imgPrevious.LoadFromFile("./resources/quetes/previous.png");
    sprPrevious.SetImage(imgPrevious);
    sprPrevious.SetPosition(widgetGaucheX/2,(titleSkills.GetPosition().y+50)*FENETREY/600.0);

    //skill courant
    txtCurrentSkill.SetPosition(sprPrevious.GetPosition().x+40,sprPrevious.GetPosition().y);
    txtCurrentSkill.SetFont(font);

    //skill suivant
    imgNext.LoadFromFile("./resources/quetes/next.png");
    sprNext.SetImage(imgNext);
    sprNext.SetPosition(txtCurrentSkill.GetPosition().x + 70, sprPrevious.GetPosition().y);

    manaSkillTitle.SetText("Mana :");
    manaSkillTitle.SetStyle(String::Underlined);
    manaSkillTitle.SetFont(font);
    manaSkillTitle.SetSize(taillePolice * FENETREX/800.0);
    manaSkillTitle.SetPosition(50,txtCurrentSkill.GetPosition().y+1.5*txtCurrentSkill.GetRect().GetHeight()+(32*FENETREX/600.0));

    dureeSkillTitle.SetText("Duree :");
    dureeSkillTitle.SetStyle(String::Underlined);
    dureeSkillTitle.SetFont(font);
    dureeSkillTitle.SetSize(taillePolice * FENETREX/800.0);
    dureeSkillTitle.SetPosition(50,manaSkillTitle.GetPosition().y + manaSkillTitle.GetRect().GetHeight());

    rechargeSkillTitle.SetText("Recharge :");
    rechargeSkillTitle.SetStyle(String::Underlined);
    rechargeSkillTitle.SetFont(font);
    rechargeSkillTitle.SetSize(taillePolice * FENETREX/800.0);
    rechargeSkillTitle.SetPosition(50,dureeSkillTitle.GetPosition().y + dureeSkillTitle.GetRect().GetHeight());

    //initialisation des textes
    majTextesSkills();

    //raccourcis barre sorts
    majRaccourcisTemp();
};

void Pause::initMenuInventaire()
{
    initTitreMenu(&titleInventaire,String("~ Inventaire ~"));

    descObjetsInventaire[POTION_VIE_LOOT] = "Potion de vie : rend instantanement 50pv.";
    descObjetsInventaire[POTION_MANA_LOOT] = "Potion de mana : rend instantanement 50pm.";
    descObjetsInventaire[PARCHEMIN_LOOT] = "Parchemin : objet de quete, permet de suivre l'avancement de l'histoire et \nd'obtenir des skills ou de l'experience.";
    descObjetsInventaire[SAUVEGARDES_LOOT] = "Sauvegarde : sauvegardez la partie quand vous voulez en l'utilisant.\nAttention cependant, c'est un usage unique.";
    descObjetsInventaire[OR_LOOT] = "Or : nombre de pieces d'or que vous possedez.";
};

void Pause::initMenuOption()
{
    //titre du menu
    initTitreMenu(&titleOption,String("~ Options ~"));

    //textes
    optionsGenerales[VOLUME_MUSIQUE_JEUX].SetText("Volume musique jeux :");
    optionsGenerales[VOLUME_MUSIQUE_MENUS].SetText("Volume musique menus :");
    optionsGenerales[VOLUME_EFFETS_SONORES].SetText("Volume effets sonores :");
    optionsGenerales[VOLUME_SON].SetText("Volume son :");
    optionsGenerales[FPS].SetText("Images par secondes :");
    optionsGenerales[SYNC_VERTICALE].SetText("Synchronisation verticale :");
    optionsGenerales[RESOLUTION].SetText("Resolution :");
    optionsGenerales[FULLSCREEN].SetText("Plein ecran :");
    optionsGenerales[TOUCHES_PERSO].SetText("Touches du heros");
    optionsGenerales[TOUCHES_PERSO].SetStyle(String::Underlined);
    optionsGenerales[TOUCHES_SYS].SetText("Touches systeme");
    optionsGenerales[TOUCHES_SYS].SetStyle(String::Underlined);

    //************
    //  Valeurs
    //************
    stringstream out;

    //volume de la musique du jeux
    out << settings->volumeMusiqueJeux;
    optionsGeneralesValues[VOLUME_MUSIQUE_JEUX].SetText(out.str());
    out.str(string()); //pour effacer le contenu du flux

    //volume de la musique des menus
    out << settings->volumeMusiqueMenus;
    optionsGeneralesValues[VOLUME_MUSIQUE_MENUS].SetText(out.str());
    out.str(string());

    //volume effets sonores
    out << settings->volumeEffetsSonores;
    optionsGeneralesValues[VOLUME_EFFETS_SONORES].SetText(out.str());
    out.str(string());

    //volume des sons
    out << settings->volumeSons;
    optionsGeneralesValues[VOLUME_SON].SetText(out.str());
    out.str(string());

    //fps
    out << settings->fps;
    optionsGeneralesValues[FPS].SetText(out.str());
    out.str(string());

    //synchronisation verticale
    optionsGeneralesValues[SYNC_VERTICALE].SetText((settings->verticalSync)?"oui":"non");


    //résolution
    resolutions[0] = "640x480";
    resolutions[1] = "800x600";
    resolutions[2] = "1024x768";
    resolutions[3] = "1152x864";
    resolutions[4] = "1280x600";
    resolutions[5] = "1280x720";
    resolutions[6] = "1280x768";
    resolutions[7] = "1280x800";
    resolutions[8] = "1360x768";
    resolutions[9] = "1366x768";
    resolutions[10] = "1440x900";
    resolutions[11] = "1600x900";
    cptResolution=getCptFromResol();
    optionsGeneralesValues[RESOLUTION].SetText(resolutions[cptResolution]);

    //fullscreen
    optionsGeneralesValues[FULLSCREEN].SetText((settings->fullscreen)?"oui":"non");

    for (int i=0; i<TAILLE_OPTIONS_GENERALES; ++i)
    {
        optionsGenerales[i].SetSize(taillePolice * FENETREX/800.0);
        optionsGeneralesValues[i].SetSize(taillePolice * FENETREX/800.0);
    }

    int sizePlusGrandChoixX = optionsGenerales[SYNC_VERTICALE].GetRect().GetWidth();
    int posX = 50;
    int posY = 100;

    for (int i=0; i<TAILLE_OPTIONS_GENERALES; ++i)
    {
        optionsGenerales[i].SetFont(font);
        optionsGenerales[i].SetPosition(posX,posY+=1.2*optionsGenerales[i].GetRect().GetHeight());
        optionsGeneralesValues[i].SetFont(font);
        optionsGeneralesValues[i].SetPosition(posX+sizePlusGrandChoixX+50,posY);
    }

    //sous menu pour les touches du héros
    initSousMenuTouchesHeros();

    //sous menu pour les touches systèmes
    initSousMenuTouchesSys();

    avertissementOptions.SetText("Attention : certaines options ne seront prises en compte qu'apres redemarrage.");
    avertissementOptions.SetFont(font);
    avertissementOptions.SetPosition(10,FENETREY-avertissementOptions.GetRect().GetHeight());
    avertissementOptions.SetStyle(String::Italic);
    avertissementOptions.SetSize(20);
    avertissementOptions.SetColor(Color::Black);
};

void Pause::initSousMenuTouchesHeros()
{
    initTitreMenu(&titleTouchesHeros,String("~ Touches Heros ~"));

    touchesPersoDesc[CTRL_GAUCHE].SetText("Gauche :");
    touchesPersoDesc[CTRL_DROITE].SetText("Droite :");
    touchesPersoDesc[CTRL_SAUTER].SetText("Sauter :");
    touchesPersoDesc[CTRL_COURRIR].SetText("Courrir :");
    touchesPersoDesc[CTRL_TAPER].SetText("Frapper :");
    touchesPersoDesc[CTRL_PAUSE].SetText("Pause :");
    touchesPersoDesc[CTRL_RAMASSE].SetText("Action (ramasser,utiliser) :");
    touchesPersoDesc[CTRL_SCREENSHOT].SetText("Prendre une photo :");
    touchesPersoDesc[CTRL_GUI1].SetText("Barre de sort 1 :");
    touchesPersoDesc[CTRL_GUI2].SetText("Barre de sort 2 :");
    touchesPersoDesc[CTRL_GUI3].SetText("Barre de sort 3 :");
    touchesPersoDesc[CTRL_GUI4].SetText("Barre de sort 4 :");
    touchesPersoDesc[CTRL_GUI5].SetText("Barre de sort 5 :");
    touchesPersoDesc[CTRL_GUI6].SetText("Barre de sort 6 :");
    touchesPersoDesc[CTRL_GUI7].SetText("Barre de sort 7 :");
    touchesPersoDesc[CTRL_GUI8].SetText("Barre de sort 8 :");
    touchesPersoDesc[CTRL_GUI9].SetText("Barre de sort 9 :");
    touchesPersoDesc[CTRL_GUI10].SetText("Barre de sort 10 :");

    touchesPersoValues[CTRL_GAUCHE].SetText(findKey(touches->gauche));
    touchesPersoValues[CTRL_DROITE].SetText(findKey(touches->droite));
    touchesPersoValues[CTRL_SAUTER].SetText(findKey(touches->sauter));
    touchesPersoValues[CTRL_COURRIR].SetText(findKey(touches->courrir));
    touchesPersoValues[CTRL_TAPER].SetText(findKey(touches->taper));
    touchesPersoValues[CTRL_PAUSE].SetText(findKey(touches->pause));
    touchesPersoValues[CTRL_RAMASSE].SetText(findKey(touches->ramasse));
    touchesPersoValues[CTRL_SCREENSHOT].SetText(findKey(touches->screenshot));
    touchesPersoValues[CTRL_GUI1].SetText(findKey(touches->gui1));
    touchesPersoValues[CTRL_GUI2].SetText(findKey(touches->gui2));
    touchesPersoValues[CTRL_GUI3].SetText(findKey(touches->gui3));
    touchesPersoValues[CTRL_GUI4].SetText(findKey(touches->gui4));
    touchesPersoValues[CTRL_GUI5].SetText(findKey(touches->gui5));
    touchesPersoValues[CTRL_GUI6].SetText(findKey(touches->gui6));
    touchesPersoValues[CTRL_GUI7].SetText(findKey(touches->gui7));
    touchesPersoValues[CTRL_GUI8].SetText(findKey(touches->gui8));
    touchesPersoValues[CTRL_GUI9].SetText(findKey(touches->gui9));
    touchesPersoValues[CTRL_GUI10].SetText(findKey(touches->gui10));

    for (int i=0; i<TAILLE_CTRL; ++i)
    {
        touchesPersoDesc[i].SetSize(taillePolice-1 * FENETREX/800.0);
        touchesPersoValues[i].SetSize(taillePolice-1 * FENETREX/800.0);
    }

    int sizePlusGrandChoixX = touchesPersoDesc[CTRL_RAMASSE].GetRect().GetWidth() + 10;
    int posX = 30;
    int posY = titleTouchesHeros.GetPosition().y + 1.5*titleTouchesHeros.GetRect().GetHeight();

    for (int i=0; i<TAILLE_CTRL; ++i)
    {
        touchesPersoDesc[i].SetFont(font);
        touchesPersoDesc[i].SetPosition(posX,posY+=1.1*touchesPersoDesc[i].GetRect().GetHeight());
        touchesPersoValues[i].SetFont(font);
        touchesPersoValues[i].SetPosition(sizePlusGrandChoixX,posY);
    }

    touchesPersoDesc[CTRL_RETOUR].SetText("Retour");
    touchesPersoDesc[CTRL_RETOUR].SetFont(font);
    touchesPersoDesc[CTRL_RETOUR].SetSize(taillePolice * FENETREX/800.0);
    touchesPersoDesc[CTRL_RETOUR].SetPosition(widgetGaucheX,FENETREY-50);
};

void Pause::initSousMenuTouchesSys()
{
    initTitreMenu(&titleTouchesSys,String("~ Touches menus ~"));

    touchesSysDesc[CTRLSYS_GAUCHE].SetText("Gauche :");
    touchesSysDesc[CTRLSYS_DROITE].SetText("Droite :");
    touchesSysDesc[CTRLSYS_HAUT].SetText("Haut :");
    touchesSysDesc[CTRLSYS_BAS].SetText("Bas :");
    touchesSysDesc[CTRLSYS_RETOUR].SetText("Retour :");
    touchesSysDesc[CTRLSYS_ENTREE].SetText("Valider :");
    touchesSysDesc[CTRLSYS_BOUTTON_RETOUR].SetText("Retour");

    touchesSysValues[CTRLSYS_GAUCHE].SetText(findKey(touchesSys->gauche));
    touchesSysValues[CTRLSYS_DROITE].SetText(findKey(touchesSys->droite));
    touchesSysValues[CTRLSYS_HAUT].SetText(findKey(touchesSys->haut));
    touchesSysValues[CTRLSYS_BAS].SetText(findKey(touchesSys->bas));
    touchesSysValues[CTRLSYS_RETOUR].SetText(findKey(touchesSys->retour));
    touchesSysValues[CTRLSYS_ENTREE].SetText(findKey(touchesSys->entree));

    for (int i=0; i<TAILLE_CTRLSYS; ++i)
    {
        touchesSysDesc[i].SetSize(taillePolice * FENETREX/800.0);
        touchesSysValues[i].SetSize(taillePolice * FENETREX/800.0);
    }

    int sizePlusGrandChoixX = touchesSysDesc[CTRLSYS_ENTREE].GetRect().GetWidth();
    int posX = 30;
    int posY = 100;

    for (int i=0; i<TAILLE_CTRLSYS; ++i)
    {
        touchesSysDesc[i].SetFont(font);
        touchesSysDesc[i].SetPosition(posX,posY+=1.2*touchesSysDesc[i].GetRect().GetHeight());
        touchesSysValues[i].SetFont(font);
        touchesSysValues[i].SetPosition(posX+sizePlusGrandChoixX+100,posY);
    }

    touchesSysDesc[CTRLSYS_BOUTTON_RETOUR].SetText("Retour");
    touchesSysDesc[CTRLSYS_BOUTTON_RETOUR].SetFont(font);
    touchesSysDesc[CTRLSYS_BOUTTON_RETOUR].SetSize(taillePolice * FENETREX/800.0);
    touchesSysDesc[CTRLSYS_BOUTTON_RETOUR].SetPosition(widgetGaucheX,FENETREY-50);
};

void Pause::initBoiteConnection()
{
    pseudoConnection = mdpConnection = "";
    pseudoConnectionStr.SetPosition(sprBoiteConnexion.GetPosition().x+0.38*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.32*sprBoiteConnexion.GetSize().y);
    pseudoConnectionStr.SetSize(0.08*sprBoiteConnexion.GetSize().y);
    mdpConnectionStr.SetPosition(sprBoiteConnexion.GetPosition().x+0.38*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.52*sprBoiteConnexion.GetSize().y);
    mdpConnectionStr.SetSize(pseudoConnectionStr.GetSize());
};

inline int Pause::getCptFromResol()
{
    int cpt=0;

    if (strcmp(settings->resolutionX,"640") == 0)
        cpt=0;
    else if (strcmp(settings->resolutionX,"800") == 0)
        cpt=1;
    else if (strcmp(settings->resolutionX,"1024") == 0)
        cpt=2;
    else if (strcmp(settings->resolutionX,"1152") == 0)
        cpt=3;
    else if (strcmp(settings->resolutionX,"1280") == 0)
    {
        if (strcmp(settings->resolutionY,"600") == 0)
            cpt=4;
        else if (strcmp(settings->resolutionY,"720") == 0)
            cpt=5;
        else if (strcmp(settings->resolutionY,"768") == 0)
            cpt=6;
        else if (strcmp(settings->resolutionY,"800") == 0)
            cpt=7;
    }
    else if (strcmp(settings->resolutionX,"1360") == 0)
        cpt=8;
    else if (strcmp(settings->resolutionX,"1366") == 0)
        cpt=9;
    else if (strcmp(settings->resolutionX,"1440") == 0)
        cpt=10;
    else if (strcmp(settings->resolutionX,"1600") == 0)
        cpt=11;
    else
        cpt=0;

    return cpt;
};

inline void Pause::initTitreMenu(String *titre,String txt)
{
    titre->SetText(txt.GetText());
    titre->SetFont(font);
    titre->SetSize(taillePolice * FENETREX/800.0 + 4);
    titre->SetCenter(titre->GetRect().GetWidth()/2,titre->GetRect().GetHeight()/2);
    titre->SetPosition((2*widgetGaucheX)/3,50);
};

void Pause::majTextesSkills()
{
    for (unsigned int i=0; i<TAILLE_SKILLS; ++i)
    {
        //l'image du skin
        sprSkill[i].SetPosition(50,txtCurrentSkill.GetPosition().y+1.5*txtCurrentSkill.GetRect().GetHeight());
        sprSkill[i].Resize(32*FENETREX/800.0,32*FENETREY/600.0);

        //son titre
        titreSkill[i].SetFont(font);
        titreSkill[i].SetSize(taillePolice*FENETREX/800.0);
        titreSkill[i].SetPosition(sprSkill[i].GetPosition().x + sprSkill[i].GetSize().y + 20, sprSkill[i].GetPosition().y);
        titreSkill[i].SetStyle(String::Underlined);

        //mana
        manaSkill[i].SetFont(font);
        manaSkill[i].SetSize(taillePolice*FENETREX/800.0);
        manaSkill[i].SetPosition(manaSkillTitle.GetPosition().x + manaSkillTitle.GetRect().GetWidth() + 10,manaSkillTitle.GetPosition().y);

        //duree
        dureeSkill[i].SetFont(font);
        dureeSkill[i].SetSize(taillePolice*FENETREX/800.0);
        dureeSkill[i].SetPosition(dureeSkillTitle.GetPosition().x + dureeSkillTitle.GetRect().GetWidth() + 10,dureeSkillTitle.GetPosition().y);

        //recharge
        rechargeSkill[i].SetFont(font);
        rechargeSkill[i].SetSize(taillePolice*FENETREX/800.0);
        rechargeSkill[i].SetPosition(rechargeSkillTitle.GetPosition().x + rechargeSkillTitle.GetRect().GetWidth() + 10,rechargeSkillTitle.GetPosition().y);

        //sa descriptions
        texteSkill[i].SetFont(font);
        texteSkill[i].SetSize(taillePolice*FENETREX/800.0);
        texteSkill[i].SetPosition(50, rechargeSkill[i].GetPosition().y + rechargeSkill[i].GetRect().GetHeight() + 10);
    }

    txtNoSkills.SetText(convertText("Vous n'avez pas encore de skills... Pensez a ramassez les parchemins et divers autres objets que vous rencontrerez durant votre periple !"));
    txtNoSkills.SetFont(font);
    txtNoSkills.SetPosition(50,txtCurrentSkill.GetPosition().y+1.5*txtCurrentSkill.GetRect().GetHeight());
    txtNoSkills.SetSize(taillePolice*FENETREX/800.0);
};

void Pause::initImages()
{
    //fond
    imgBackPause.LoadFromFile("./resources/Pause/back2.jpg");
    sprBackPause.SetImage(imgBackPause);
    sprBackPause.Resize(FENETREX,FENETREY);

    //bottom separation
    imgBottomSeparation.LoadFromFile("./resources/Pause/bottomSeparation.png");
    sprBottomSeparation.SetImage(imgBottomSeparation);
    sprBottomSeparation.Resize(widgetGaucheX,70);
    sprBottomSeparation.SetPosition(0,FENETREY-150);

    //cursor
    imgCursor.LoadFromFile("./resources/Pause/cursor.png");
    sprCursor.SetImage(imgCursor);
    moveCursor(choixActuel);

    //portraits
    imgPortrait.LoadFromFile("./resources/Pause/portrait.png");
    sprPortrait.SetImage(imgPortrait);
    sprPortrait.Resize(200*FENETREX/800.0,200*FENETREY/600.0);
    sprPortrait.SetPosition(25,50);

    imgPortrait2.LoadFromFile("./resources/Pause/portrait2.png");
    sprPortrait2.SetImage(imgPortrait2);
    sprPortrait2.Resize(sprPortrait2.GetSize().x*FENETREX/800.0,sprPortrait2.GetSize().y*FENETREY/600.0);
    sprPortrait2.SetPosition(widgetGaucheX-sprPortrait2.GetSize().x+50,FENETREY-sprPortrait2.GetSize().y);

    //retour au jeu
    imgRetour.LoadFromFile("./resources/Pause/retour.png");
    sprRetour.SetImage(imgRetour);
    sprRetour.SetPosition(0,0);

    //quitter le jeu
    imgQuitter.LoadFromFile("./resources/Pause/quitter.png");
    sprQuitter.SetImage(imgQuitter);
    sprQuitter.SetPosition(0,0);

    //inventaire
    imgInventaire[POTION_VIE_LOOT].LoadFromFile("./resources/image/potionVie.png");
    imgInventaire[POTION_MANA_LOOT].LoadFromFile("./resources/image/potionMana.png");
    imgInventaire[PARCHEMIN_LOOT].LoadFromFile("./resources/image/parcheminMin.png");
    imgInventaire[SAUVEGARDES_LOOT].LoadFromFile("./resources/image/sauvegarde.png");
    imgInventaire[OR_LOOT].LoadFromFile("./resources/image/icones/or_1.png");
    int posY=100;
    int posX=50;
    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
    {
        inventaire[i].SetImage(imgInventaire[i]);
        inventaire[i].Resize(50,50);
        if (posY>=widgetGaucheX-70)
            posY+=(2*inventaire[i].GetSize().x);
        inventaire[i].SetPosition(posX,posY);
        posX+=(2*inventaire[i].GetSize().x);
    }


    //boite de dialogue permettant la connexion au site
    imgBoiteConnexion.LoadFromFile("./resources/Pause/boiteConnexion.png");
    sprBoiteConnexion.SetImage(imgBoiteConnexion);
    sprBoiteConnexion.Resize(FENETREX/2.5,FENETREY/3.0);
    sprBoiteConnexion.SetPosition(FENETREX/2.0 - sprBoiteConnexion.GetSize().x/2.0,FENETREY/2.0 - sprBoiteConnexion.GetSize().y/2.0);
    imgLogin.LoadFromFile("./resources/Pause/insertDataCo.png");

    sprLogin.SetImage(imgLogin);
    sprLogin.Resize(sprBoiteConnexion.GetSize().x/4.0,sprBoiteConnexion.GetSize().y/8.0);

    imgResponseSite.LoadFromFile("./resources/Pause/repConnexion.png");
};


void Pause::run()
{
    //getEvents();
};


void Pause::reset()
{
    choixActuel=0;
    choixGauche=ATTAQUE;
    moveCursor(choixActuel);
};

void Pause::getEvents(Event event)
{
    if (!waitForTouche)
    {
        if ((event.Key.Code == touchesSys->haut)  and  (screen == RIGHT_SCREEN)) moveCursor(--choixActuel);
        if ((event.Key.Code == touchesSys->haut)  and  (screen == LEFT_SCREEN)) moveCursor(--choixGauche);
        if ((event.Key.Code == touchesSys->bas)  and  (screen == RIGHT_SCREEN)) moveCursor(++choixActuel);
        if ((event.Key.Code == touchesSys->bas)  and  (screen == LEFT_SCREEN)) moveCursor(++choixGauche);
        if (event.Key.Code == touchesSys->gauche) leftKey();
        if (event.Key.Code == touchesSys->droite) rightKey();
        if (event.Key.Code == touchesSys->entree) validationChoix();

        if (!connexionSite)  //touches ne devant pas être détectées lors de la connexion au site
        {
            if (event.Key.Code == touchesSys->retour) retour();
        }
        else
        {
            if (event.Type == Event::TextEntered) writeTxt(event.Text.Unicode);
            if (event.Key.Code == Key::Back) deleteText();
        }
    }
    else
    {
        //si on est en train de redéfinir les raccourcis pour les skills
        if (choixActuel == SKILLS)
            redefineRaccourcisBS(event);
        else if (choixActuel == INVENTAIRE) //si on en train
            redefineRaccourcisBSObj(event);
        else //options ...
            redefineTouches(event);
    }
};

inline void Pause::writeTxt(int txt)
{
    if (txt > 30 and (txt < 127 || txt > 159)) //lettres ou chiffres
    {
        //pseudo
        if (choixGauche == CHAMP_PSEUDO)
        {
            pseudoConnection+=txt;
            pseudoConnectionStr.SetText(pseudoConnection);
        }
        else if (choixGauche == CHAMP_MDP)
        {
            mdpConnection+=txt;
            mdpConnectionStr.SetText(string(mdpConnectionStr.GetText()) + "*");
        }
    }
};

inline void Pause::deleteText()
{
    if (choixGauche == CHAMP_PSEUDO and pseudoConnection.size()>0)
    {
        pseudoConnection.erase(pseudoConnection.size()-1);
        pseudoConnectionStr.SetText(pseudoConnection);
    }
    else if (choixGauche == CHAMP_MDP and mdpConnection.size()>0)
    {
        mdpConnection.erase(mdpConnection.size()-1);
        string newMdp = string(mdpConnectionStr.GetText());
        newMdp.erase(newMdp.size()-1);
        mdpConnectionStr.SetText(newMdp);
    }
}

inline void Pause::redefineRaccourcisBS(Event event)
{
    //on vérifie que la touche est bien comprise entre F1 et F10
    string newKey = findKey(event.Key.Code);
    bool found=false;
    int index;

    for (int i=0; i<11; ++i)
    {
        stringstream out;
        out << "F" << i+1;
        if (newKey == out.str())
        {
            index = i;
            found=true;
        }
    }

    //on applique le changements
    if (found)
    {
        raccourcisTemp.SetText(txtExplicatifRaccourcisSkills + newKey);
        touches->tabBarreSortsId[index] = idSkill[compteurSkillReel] -1 ;
        touches->tabBarreSortsObj[index] =  false;

        //si le skill était déjà dans la barre, on vide son ancien emplacment
        if (idRaccourcisBSTemp != -1)
        {
            touches->tabBarreSortsId[idRaccourcisBSTemp] = -1;
            touches->tabBarreSortsObj[idRaccourcisBSTemp] = true;
        }
    }
    else
    {
        raccourcisTemp.SetText(toucheTemp);
    }

    //on réinitialise les variables de redéfinitions
    toucheTemp = "";
    waitForTouche = false;
};

inline void Pause::redefineRaccourcisBSObj(Event event)
{
    //on vérifie que la touche est bien comprise entre F1 et F10
    string newKey = findKey(event.Key.Code);
    bool found=false;
    int index;

    for (int i=0; i<11; ++i)
    {
        stringstream out;
        out << "F" << i+1;
        if (newKey == out.str())
        {
            index = i;
            found=true;
        }
    }

    //on applique le changements
    if (found)
    {
        raccourcisObjTemp.SetText(txtExplicatifRaccourcisObj + newKey);
        touches->tabBarreSortsId[index] = choixGauche;
        touches->tabBarreSortsObj[index] = true;

        //si le skill était déjà dans la barre, on vide son ancien emplacment
        if (idRaccourcisBSTemp != -1) {
            touches->tabBarreSortsId[idRaccourcisBSTemp] = -1;
            touches->tabBarreSortsObj[idRaccourcisBSTemp] = true;
        }
    }
    else
    {
        raccourcisObjTemp.SetText(toucheTemp);
    }

    //on réinitialise les variables de redéfinitions
    toucheTemp = "";
    waitForTouche = false;
};

inline void Pause::redefineTouches(Event event)
{
    waitForTouche=false;
    bool found=false;
    unsigned int taille=0;
    string newKey=findKey(event.Key.Code);

    //on vérifie si il y a des doublons

    if (touchesHerosMenued)
        taille=TAILLE_CTRL;
    else if (touchesSysMenued)
        taille=TAILLE_CTRLSYS;

    for (unsigned int i=0; i<taille; ++i)
    {
        if (touchesHerosMenued)
        {
            if (string(touchesPersoValues[i].GetText()) == newKey)
                found=true;
        }
        else if (touchesSysMenued)
        {
            if (string(touchesSysValues[i].GetText()) == newKey)
                found=true;
        }
    }

    //si la touche n'est pas déjà définie, on la met
    if (!found)
    {
        if (touchesHerosMenued)
            touchesPersoValues[choixGauche].SetText(newKey);
        else if (touchesSysMenued)
            touchesSysValues[choixGauche].SetText(newKey);
    }
    else
    {
        if (touchesHerosMenued)
            touchesPersoValues[choixGauche].SetText(toucheTemp);
        else if (touchesSysMenued)
            touchesSysValues[choixGauche].SetText(toucheTemp);
    }

    toucheTemp = "";

    //on sauvegarde les changements dans la structure
    saveChanges(event);
};

void Pause::leftKey()
{
    //si on est dans les skills
    if (choixActuel==SKILLS and nbSkillsMaitrises>1)
    {
        compteurSkillReel--;
        bool found=false;
        while (!found) //on boucle jusqu'a trouvé un skill à afficher
        {
            if (compteurSkillReel==-1)
                compteurSkillReel=TAILLE_SKILLS-1;
            found = displaySkill[compteurSkillReel];
            if (!found) compteurSkillReel--;
        }

        compteurSkillDisplayed--;
        if (compteurSkillDisplayed==0)
            compteurSkillDisplayed=nbSkillsMaitrises;
        changeCurrentSkill();
    }

    //écran de gauche
    if (screen == LEFT_SCREEN)
    {
        //si on est dans le level up du heros
        if (choixActuel == STATUS)
            changeCarac(false);
        //si on est dans l'inventaire
        else if (choixActuel == INVENTAIRE)
            moveCursor(--choixGauche);
        //si on est dans les options
        else if (choixActuel==OPTIONS)
        {
            stringstream out;

            switch (choixGauche)
            {
                case VOLUME_MUSIQUE_JEUX:
                    if (settings->volumeMusiqueJeux>0)
                    {
                        out << --settings->volumeMusiqueJeux;
                        optionsGeneralesValues[VOLUME_MUSIQUE_JEUX].SetText(out.str());
                    }
                    break;

                case VOLUME_EFFETS_SONORES:
                    if (settings->volumeEffetsSonores>0)
                    {
                        out << --settings->volumeEffetsSonores;
                        optionsGeneralesValues[VOLUME_EFFETS_SONORES].SetText(out.str());
                    }
                    break;

                case VOLUME_MUSIQUE_MENUS:
                    if (settings->volumeMusiqueMenus>0)
                    {
                        out << --settings->volumeMusiqueMenus;
                        optionsGeneralesValues[VOLUME_MUSIQUE_MENUS].SetText(out.str());
                    }
                    break;

                case VOLUME_SON:
                    if (settings->volumeSons>0)
                    {
                        out << --settings->volumeSons;
                        optionsGeneralesValues[VOLUME_SON].SetText(out.str());
                    }
                    break;

                case FPS:
                    if (settings->fps>15)
                    {
                        out << (settings->fps-=15);
                        optionsGeneralesValues[FPS].SetText(out.str());
                    }
                    break;

                case SYNC_VERTICALE:
                    if (string(optionsGeneralesValues[SYNC_VERTICALE].GetText()) == "oui")
                        optionsGeneralesValues[SYNC_VERTICALE].SetText("non");
                    else
                        optionsGeneralesValues[SYNC_VERTICALE].SetText("oui");
                    break;

                case FULLSCREEN:
                    if (string(optionsGeneralesValues[FULLSCREEN].GetText()) == "oui")
                        optionsGeneralesValues[FULLSCREEN].SetText("non");
                    else
                        optionsGeneralesValues[FULLSCREEN].SetText("oui");
                    break;

                case RESOLUTION:
                    cptResolution--;
                    if (cptResolution==-1)
                        cptResolution=NB_RESOLUTIONS-1;
                    optionsGeneralesValues[RESOLUTION].SetText(resolutions[cptResolution]);
                    break;
            }
        }
    }
};

void Pause::rightKey()
{
    if (choixActuel==SKILLS and nbSkillsMaitrises>1)
    {
        ++compteurSkillReel;
        bool found=false;
        while (!found) //on boucle jusqu'a trouvé un skill à afficher
        {
            if (compteurSkillReel==TAILLE_SKILLS)
                compteurSkillReel=0;
            found = displaySkill[compteurSkillReel];
            if (!found) ++compteurSkillReel;
        }

        //++compteurSkillDisplayed;
        if (++compteurSkillDisplayed==nbSkillsMaitrises+1)
            compteurSkillDisplayed=1;
        changeCurrentSkill();
    }

    //écran de droite
    if (screen == LEFT_SCREEN)
    {
        //si on est dans le level up du heros
        if (choixActuel == STATUS)
            changeCarac(true);
        //si on est dans l'inventaire
        else if (choixActuel == INVENTAIRE)
            moveCursor(++choixGauche);
        //si on est dans les options
        else if (choixActuel==OPTIONS)
        {
            stringstream out;

            switch (choixGauche)
            {
                case VOLUME_MUSIQUE_JEUX:
                    if (settings->volumeMusiqueJeux<100)
                    {
                        out << ++settings->volumeMusiqueJeux;
                        optionsGeneralesValues[VOLUME_MUSIQUE_JEUX].SetText(out.str());
                    }
                    break;

                case VOLUME_EFFETS_SONORES:
                    if (settings->volumeEffetsSonores<100)
                    {
                        out << ++settings->volumeEffetsSonores;
                        optionsGeneralesValues[VOLUME_EFFETS_SONORES].SetText(out.str());
                    }
                    break;

                case VOLUME_MUSIQUE_MENUS:
                    if (settings->volumeMusiqueMenus<100)
                    {
                        out << ++settings->volumeMusiqueMenus;
                        optionsGeneralesValues[VOLUME_MUSIQUE_MENUS].SetText(out.str());
                    }
                    break;

                case VOLUME_SON:
                    if (settings->volumeSons<100)
                    {
                        out << ++settings->volumeSons;
                        optionsGeneralesValues[VOLUME_SON].SetText(out.str());
                    }
                    break;

                case FPS:
                    if (settings->fps<60)
                    {
                        out << (settings->fps+=15);
                        optionsGeneralesValues[FPS].SetText(out.str());
                    }
                    break;

                case SYNC_VERTICALE:
                    if (string(optionsGeneralesValues[SYNC_VERTICALE].GetText()) == "oui")
                        optionsGeneralesValues[SYNC_VERTICALE].SetText("non");
                    else
                        optionsGeneralesValues[SYNC_VERTICALE].SetText("oui");
                    break;

                case FULLSCREEN:
                    if (string(optionsGeneralesValues[FULLSCREEN].GetText()) == "oui")
                        optionsGeneralesValues[FULLSCREEN].SetText("non");
                    else
                        optionsGeneralesValues[FULLSCREEN].SetText("oui");
                    break;

                case RESOLUTION:
                    //++cptResolution;
                    if (++cptResolution==NB_RESOLUTIONS)
                        cptResolution=0;
                    optionsGeneralesValues[RESOLUTION].SetText(resolutions[cptResolution]);
                    break;
            }
        }
    }
};

void Pause::retour()
{
    //si on est dans le menu
    if (screen==RIGHT_SCREEN)
    {
        exitMenuPause();
    }
    //si on est dans l'écran de gauche
    else
    {
        //si on est dans un sous menu
        if (touchesHerosMenued)
        {
            touchesHerosMenued=false;
            moveCursor(VOLUME_MUSIQUE_JEUX);
        }
        else if (touchesSysMenued)
        {
            touchesSysMenued=false;
            moveCursor(VOLUME_MUSIQUE_JEUX);
        }
        else if (connexionSite)
        {
            quitConnectionSite();
        }
        else
        {
            if (choixActuel == INVENTAIRE)
                descObjTemp.SetText(txtExplicatifInventaire);
            changeScreen(RIGHT_SCREEN);
        }
    }
};

void Pause::validationChoix()
{
    if (choixActuel == RETOUR)
        exitMenuPause();
    else if (choixActuel == QUITTER)
        exitGame();
    else if (choixActuel == OPTIONS  and  choixGauche == TOUCHES_PERSO  and  !touchesHerosMenued)
    {
        son->playSon(MENUVALIDER);
        touchesHerosMenued=true;
        moveCursor(CTRL_GAUCHE);
    }
    else if (choixActuel == OPTIONS  and  choixGauche == TOUCHES_SYS  and  !touchesSysMenued)
    {
        son->playSon(MENUVALIDER);
        touchesSysMenued=true;
        moveCursor(CTRLSYS_GAUCHE);
    }
    else if (touchesHerosMenued)
    {
        //on veut quitter le sous menu
        if (choixGauche==CTRL_RETOUR)
        {
            son->playSon(MENUVALIDER);
            touchesHerosMenued=false;
            moveCursor(VOLUME_MUSIQUE_JEUX);
        }
        else //redéfinition des touches
        {
            waitForTouchePressed=true;
            toucheTemp=touchesPersoValues[choixGauche].GetText();
            touchesPersoValues[choixGauche].SetText("Appuyez sur une touche...");
        }
    }
    else if (touchesSysMenued)
    {
        //on veut quitter le sous menu
        if (choixGauche==CTRLSYS_BOUTTON_RETOUR)
        {
            son->playSon(MENUVALIDER);
            touchesSysMenued=false;
            moveCursor(VOLUME_MUSIQUE_JEUX);
        }
        else //redéfinition des touches
        {
            waitForTouchePressed=true;
            toucheTemp=touchesSysValues[choixGauche].GetText();
            touchesSysValues[choixGauche].SetText("Appuyez sur une touche...");
        }
    }
    else if (connexionSite)
    {
        son->playSon(MENUVALIDER);
        //traitement de la touche entrée dans la botie de dialogue de connection au site
        if (choixGauche==CHAMP_CANCEL)
            quitConnectionSite();
        else if (choixGauche==CHAMP_OK)
        {
            if (!responseSite)
            {
                if (pseudoConnection.size()>0 and mdpConnection.size()>0)
                    sendDataToSite();
                else
                    son->playSon(SON_ERROR_MENU);
            }
            else quitConnectionSite();
        }
    }
    else if (choixActuel == SKILLS)
    {
        if (nbSkillsMaitrises>0 and (idReelSkillCourant != SKILL_SAUT and idReelSkillCourant != SKILL_DOUBLE_SAUT and idReelSkillCourant != SKILL_COURIR and idReelSkillCourant != SKILL_FRAPPE))
        {
            majRaccourcisTemp();
            changeOptions=true;
            waitForTouchePressed=true;
            toucheTemp = raccourcisTemp.GetText();
            raccourcisTemp.SetText(txtExplicatifRaccourcisSkills + "Appuyez sur une touche (de F1 à F10)...");
        }
    }
    else if (choixActuel == INVENTAIRE  and  screen == LEFT_SCREEN  and  choixGauche != OR_LOOT)
    {
        changeOptions=true;
        waitForTouchePressed = true;
        toucheTemp = raccourcisObjTemp.GetText();
        raccourcisObjTemp.SetText(txtExplicatifRaccourcisObj + "Appuyez sur une touche (de F1 à F10)...");
    }
    else
        changeScreen(LEFT_SCREEN);
};

inline void Pause::quitConnectionSite()
{
    connexionSite = responseSite = false;
    mdpConnection="";
    mdpConnectionStr.SetText(mdpConnection);
    sprBoiteConnexion.SetImage(imgBoiteConnexion);
    choixActuel = STATUS;
    changeScreen(RIGHT_SCREEN);
};

inline void Pause::sendDataToSite()
{
    Http http("http://dethronedgod.hebergratuit.com");

    Http::Request request;
    request.SetURI("signature_jeu.php");
    request.SetMethod(sf::Http::Request::Post);

    //anti hack : vérification des données
    int verif = ((pseudoConnection[0]*atoi(string(statsPersoValues1[LEVEL].GetText()).c_str())+atoi(string(scoresPersoValues[DEGATS_TOTAL].GetText()).c_str())));//-atoi(string(scoresPersoValues[MANA_LOSE].GetText()).c_str()))+atoi(string(scoresPersoValues[NB_MOBS_KILL].GetText()).c_str()))&atoi(string(scoresPersoValues[XP_TOTAL].GetText()).c_str());

    //construction du corps de la requete
    string body;
    body="pseudo="+pseudoConnection;
    body+="&mdp="+mdpConnection;
    body+="&level="+string(statsPersoValues1[LEVEL].GetText()) ;
    body+="&monstresTues="+string(scoresPersoValues[NB_MOBS_KILL].GetText()) ;
    body+="&bossTues="+string(scoresPersoValues[NB_MOBS_BOSS_KILL].GetText()) ;
    body+="&xpTotal="+string(scoresPersoValues[XP_TOTAL].GetText()) ;
    body+="&degatsTotal="+string(scoresPersoValues[DEGATS_TOTAL].GetText()) ;
    body+="&degatsBest="+string(scoresPersoValues[DEGATS_BEST].GetText()) ;
    body+="&viePerdue="+string(scoresPersoValues[VIE_LOSE].GetText()) ;
    body+="&manaPerdue="+string(scoresPersoValues[MANA_LOSE].GetText());
    stringstream out;
    out << verif;
    body+="&verif"+out.str();
    out.str(string());
    out << levelCourant;
    body+="&niveau="+out.str();
    request.SetBody(body);

    Http::Response response = http.SendRequest(request);
    responseSiteStr.SetText(response.GetBody());
    responseSiteStr.SetPosition(sprBoiteConnexion.GetPosition().x+0.02*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.25*sprBoiteConnexion.GetSize().y);
    responseSiteStr.SetSize(pseudoConnectionStr.GetSize());
    responseSiteStr.SetColor(Color::Green);

    //on affiche la reponse
    responseSite=true;
    sprBoiteConnexion.SetImage(imgResponseSite);
};

void Pause::changeScreen(CURRENT_SCREEN dir)
{
    //on change de screen si on est pas déjà dessus
    if (screen != dir)
    {
        screen=dir;

        if (choixActuel == STATUS)
        {
            if (screen == LEFT_SCREEN)
                moveCursor(ATTAQUE);
            else
                moveCursor(choixActuel);
        }
        else if (choixActuel == SCORES)
        {
            connexionSite=true;
            moveCursor(CHAMP_PSEUDO);
        }
        else if (choixActuel == OPTIONS)
        {
            changeOptions=true;
            if (screen == LEFT_SCREEN)
                moveCursor(VOLUME_MUSIQUE_JEUX);
            else
                moveCursor(choixActuel);
        }
        else if (choixActuel == INVENTAIRE)
        {
            if (screen == LEFT_SCREEN)
                moveCursor(0);
            else
                moveCursor(choixActuel);
        }

    }

};

inline void Pause::exitMenuPause()
{
    choixActuel=RETOUR;
    son->playSon(MENUVALIDER);
    exit = true;
};

inline void Pause::exitGame()
{
    son->playSon(MENUVALIDER);
    gameExited = true;
    exit = true;
};

void Pause::changeCurrentSkill()
{
    //si on en a
    if (nbSkillsMaitrises>0)
    {
        son->playSon(MENUBOUGER);
        stringstream out;
        out << compteurSkillDisplayed << " / " << nbSkillsMaitrises;
        txtCurrentSkill.SetText(out.str());
        idReelSkillCourant=idSkill[compteurSkillReel]-1;
        majRaccourcisTemp();
    }

};

inline void Pause::majRaccourcisTemp()
{
    //raccourcis
    string txt = "";
    int i=0;

    //cherche le skill dans la barre de sorts
    while (i<TAILLE_RACCOURCIS_BARRE_SORTS  and  txt=="")
    {
        if (idReelSkillCourant == touches->tabBarreSortsId[i]  and  !touches->tabBarreSortsObj[i])
        {
            stringstream out;
            out << "F" << i+1;
            txt = out.str();
        }

        ++i;
    }

    //si on l'a pas trouvé
    idRaccourcisBSTemp = i-1;
    if (txt=="")
    {
        txt="Aucun";
        idRaccourcisBSTemp = -1;
    }

    raccourcisTemp.SetText(txtExplicatifRaccourcisSkills + txt);
};

inline void Pause::majRaccourcisObjTemp()
{
    //raccourcis
    string txt = "";
    int i=0;

    //cherche le skill dans la barre de sorts
    while (i<TAILLE_RACCOURCIS_BARRE_SORTS  and  txt=="")
    {
        if (choixGauche == touches->tabBarreSortsId[i]  and  touches->tabBarreSortsObj[i])
        {
            stringstream out;
            out << "F" << i+1;
            txt = out.str();
        }

        ++i;
    }

    //si on l'a pas trouvé
    idRaccourcisBSTemp = i-1;
    if (txt=="")
    {
        txt="Aucun";
        idRaccourcisBSTemp = -1;
    }

    raccourcisObjTemp.SetText(txtExplicatifRaccourcisObj + txt);
};

void Pause::show(RenderTarget& app)
{
    //fond
    app.Draw(sprBackPause);

    //cursor
    app.Draw(sprCursor);

    //choix
    for (int i=0; i<TAILLE_CHOIX_PAUSE; ++i)
        app.Draw(choix[i]);

    //widget gauche
    switch ( choixActuel )
    {
        case STATUS :
            drawStatus(app);
            break;

        case SCORES :
            drawScores(app);
            break;

        case SKILLS :
            drawSkills(app);
            break;

        case INVENTAIRE :
            drawInventaire(app);
            break;

        case OPTIONS :
            drawOptions(app);
            break;

        case RETOUR :
            drawRetour(app);
            break;

        case QUITTER :
            drawQuitter(app);
            break;

        default :
            break;
    }

    if (connexionSite)
        drawConnectionSite(app);

    //pour éviter que le jeu passe en une frame du statut de rédéfinition de touche à rédefinit
    if (waitForTouchePressed)
    {
        waitForTouche=true;
        waitForTouchePressed=false;
    }
};

void Pause::drawConnectionSite(RenderTarget& app)
{
    //affichage du fond
    app.Draw(sf::Shape::Rectangle(0,0,FENETREX,FENETREY,Color(0,0,0,200)));
    app.Draw(sprBoiteConnexion);

    //affichage des logs si on est dans la partie ou l'on doit les rentrer
    if (!responseSite)
    {
        app.Draw(pseudoConnectionStr);
        app.Draw(mdpConnectionStr);
    }
    else
        app.Draw(responseSiteStr);

    app.Draw(sprCursor);
};

void Pause::drawStatus(RenderTarget& app)
{
    app.Draw(sprPortrait);

    for (int i=0; i<TAILLE_STATS; ++i)
    {
        app.Draw(statsPerso1[i]);
        app.Draw(statsPersoValues1[i]);
    }

    int posY=statsPerso1[0].GetPosition().y;
    for (int i=0; i<NBSTATS; ++i)
    {
        posY+=1.5*statsPerso1[i].GetRect().GetHeight();
        if (tabPlusMoins[i]>0)
        {
            String str;
            stringstream out;
            out << "+" << tabPlusMoins[i];
            str.SetText(out.str());
            str.SetFont(font);
            str.SetSize(taillePolice * FENETREX/800.0);
            str.SetPosition(widgetGaucheX,posY);
            app.Draw(str);
        }
    }

    for (int i=0; i<TAILLE_EXP; ++i)
    {
        app.Draw(statsPerso2[i]);
        app.Draw(statsPersoValues2[i]);
    }

    app.Draw(sprPortrait2);
};

void Pause::drawScores(RenderTarget& app)
{
    app.Draw(titleScores);

    for (int i=0; i<TAILLE_SCORES; ++i)
    {
        app.Draw(scoresPerso[i]);
        app.Draw(scoresPersoValues[i]);
    }

    app.Draw(upload);
};

void Pause::drawSkills(RenderTarget& app)
{
    app.Draw(titleSkills);

    //on les affiche
    if (nbSkillsMaitrises>0)
    {
        app.Draw(sprPrevious);
        app.Draw(txtCurrentSkill);
        app.Draw(sprNext);
        app.Draw(manaSkillTitle);
        app.Draw(dureeSkillTitle);
        app.Draw(rechargeSkillTitle);
        app.Draw(sprSkill[compteurSkillReel]);
        app.Draw(titreSkill[compteurSkillReel]);
        app.Draw(manaSkill[compteurSkillReel]);
        app.Draw(dureeSkill[compteurSkillReel]);
        app.Draw(rechargeSkill[compteurSkillReel]);
        app.Draw(texteSkill[compteurSkillReel]);

        if (idReelSkillCourant != SKILL_SAUT and idReelSkillCourant != SKILL_DOUBLE_SAUT and idReelSkillCourant != SKILL_COURIR and idReelSkillCourant != SKILL_FRAPPE)
        {
            app.Draw(raccourcisTemp);
        }

    }
    else //si il n'en a pas
        app.Draw(txtNoSkills);
};

void Pause::drawInventaire(RenderTarget& app)
{
    app.Draw(titleInventaire);

    for (int i=0; i<TAILLE_OBJETS_LOOT; ++i)
    {
        app.Draw(inventaire[i]);
        app.Draw(nbObjetsInventaire[i]);
    }

    //app.Draw(sprBottomSeparation);
    if (screen == LEFT_SCREEN  and  choixGauche != OR_LOOT)
        app.Draw(raccourcisObjTemp);
    app.Draw(descObjTemp);
};

void Pause::drawOptions(RenderTarget &app)
{
    //touches héros
    if (touchesHerosMenued)
    {
        app.Draw(titleTouchesHeros);

        for (int i=0; i<TAILLE_CTRL; ++i)
        {
            app.Draw(touchesPersoDesc[i]);
            app.Draw(touchesPersoValues[i]);
        }
    }
    else if (touchesSysMenued)
    {
        app.Draw(titleTouchesSys);

        for (int i=0; i<TAILLE_CTRLSYS; ++i)
        {
            app.Draw(touchesSysDesc[i]);
            app.Draw(touchesSysValues[i]);
        }
    }
    else
    {
        //menu options de base
        app.Draw(titleOption);

        for (int i=0; i<TAILLE_OPTIONS_GENERALES; ++i)
        {
            app.Draw(optionsGenerales[i]);
            app.Draw(optionsGeneralesValues[i]);
        }

        app.Draw(avertissementOptions);
    }
};

void Pause::drawRetour(RenderTarget& app)
{
    app.Draw(sprRetour);
};

void Pause::drawQuitter(RenderTarget& app)
{
    app.Draw(sprQuitter);
};

void Pause::moveCursor(int selection)
{
    //si on est dans le menu
    if (screen == RIGHT_SCREEN)
    {
        if (selection==-1)
            selection=TAILLE_CHOIX_PAUSE-1;
        else
            selection=selection%(TAILLE_CHOIX_PAUSE);

        choixActuel=selection;
        placeCursor(choix[selection]);
    }
    else //si on est à gauche
    {
        //si c'est pour les caractéristiques
        if (choixActuel == STATUS)
            moveCursorStatus(selection);
        else if (choixActuel == SCORES and !responseSite)
            moveCursorConnection(selection);
        else if (choixActuel == OPTIONS)
            moveCursorOptions(selection);
        else if (choixActuel == INVENTAIRE)
            moveCursorInventaire(selection,true);
    }

    son->playSon(MENUBOUGER);
};

inline void Pause::moveCursorStatus(int selection)
{
    if (selection == ATTAQUE-1)
        selection=INTELLIGENCE;
    else if (selection == INTELLIGENCE+1)
        selection=ATTAQUE;

    choixGauche=selection;

    placeCursor(statsPerso1[selection]);
};

inline void Pause::moveCursorInventaire(int selection, bool sautLigne)
{
    if (selection == -1)
        selection=TAILLE_OBJETS_LOOT-1;
    else if (selection == TAILLE_OBJETS_LOOT)
        selection=0;

    choixGauche=selection;

    sprCursor.Resize(inventaire[selection].GetSize().x + 10, inventaire[selection].GetSize().y + 10);
    sprCursor.SetPosition(inventaire[selection].GetPosition().x-5, inventaire[selection].GetPosition().y-5);

    //on met à jour la description
    descObjTemp.SetText(descObjetsInventaire[selection]);
    //on met à jour le raccourcis
    majRaccourcisObjTemp();
};

inline void Pause::moveCursorOptions(int selection)
{
    if (touchesHerosMenued)
    {
        if (selection==CTRL_GAUCHE-1)
            selection=CTRL_RETOUR;
        else if (selection==CTRL_RETOUR+1)
            selection=CTRL_GAUCHE;

        choixGauche=selection;

        placeCursor(touchesPersoDesc[selection]);
    }
    else if (touchesSysMenued)
    {
        if (selection==CTRLSYS_GAUCHE-1)
            selection=CTRLSYS_BOUTTON_RETOUR;
        else if (selection==CTRLSYS_BOUTTON_RETOUR+1)
            selection=CTRLSYS_GAUCHE;

        choixGauche=selection;

        placeCursor(touchesSysDesc[selection]);
    }
    else
    {
        //options par défaut
        if (selection==VOLUME_MUSIQUE_JEUX-1)
            selection=TAILLE_OPTIONS_GENERALES-1;
        else if (selection==TAILLE_OPTIONS_GENERALES)
            selection=VOLUME_MUSIQUE_JEUX;

        choixGauche=selection;

        if (selection>=TOUCHES_PERSO)
            placeCursor(optionsGenerales[selection]);
        else
            placeCursor(optionsGeneralesValues[selection]);
    }
};

inline void Pause::moveCursorConnection(int selection)
{
    if (selection==-1)
        selection=CHAMP_OK;
    else if (selection==CHAMP_OK+1)
        selection=CHAMP_PSEUDO;

    choixGauche=selection;

    switch (selection)
    {
        case CHAMP_PSEUDO:
            placeCursor(Vector2f(sprBoiteConnexion.GetPosition().x+0.36*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.30*sprBoiteConnexion.GetSize().y),0.62*sprBoiteConnexion.GetSize().x);
        break;

        case CHAMP_MDP:
            placeCursor(Vector2f(sprBoiteConnexion.GetPosition().x+0.36*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.50*sprBoiteConnexion.GetSize().y),0.62*sprBoiteConnexion.GetSize().x);
        break;

        case CHAMP_CANCEL:
            placeCursor(Vector2f(sprBoiteConnexion.GetPosition().x+0.60*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.75*sprBoiteConnexion.GetSize().y),0.18*sprBoiteConnexion.GetSize().x,0.18);
        break;

        case CHAMP_OK:
            placeCursor(Vector2f(sprBoiteConnexion.GetPosition().x+0.80*sprBoiteConnexion.GetSize().x,sprBoiteConnexion.GetPosition().y+0.75*sprBoiteConnexion.GetSize().y),0.18*sprBoiteConnexion.GetSize().x,0.18);
        break;

        default:
        break;
    }
};

inline void Pause::placeCursor(sf::Vector2f pos, int width, float height)
{
    sprCursor.Resize(width,height*sprBoiteConnexion.GetSize().y);
    sprCursor.SetPosition(pos);
    son->playSon(MENUBOUGER);
};

inline void Pause::placeCursor(String chaine, int width)
{
    if (width==0) width=chaine.GetRect().GetWidth()+10;
    sprCursor.Resize(width,chaine.GetRect().GetHeight()+10);
    sprCursor.SetPosition(chaine.GetPosition().x-5, chaine.GetPosition().y-5);
};

void Pause::changeCarac(bool add)
{
    //si on veut ajouter un point
    int nbSkills = atoi(string(statsPersoValues1[NB_POINTS_SKILLS].GetText()).c_str());
    int choix = choixGauche-1;

    if (add  and  nbSkills>0) //si il reste des points
    {
        son->playSon(UPCARAC);

        if (choix+1 == VITESSE)
            tabPlusMoins[choix]+=3;
        else
            ++(tabPlusMoins[choix]);
        stringstream out;
        out << "-" << tabPlusMoins[choix];
        tabPlusMoinsTxt[choix].SetText(out.str());
        out.str(string());
        out << nbSkills-1;
        statsPersoValues1[NB_POINTS_SKILLS].SetText(out.str());
    }
    else if (!add  and  tabPlusMoins[choix]>0) //si on veut enlever des points, il faut que la carac en est d'ajoutée précédemment
    {
        son->playSon(DOWNCARAC);

        if (choix+1 == VITESSE)
            tabPlusMoins[choix]-=3;
        else
            tabPlusMoins[choix]--;
        stringstream out;
        out << "+" << tabPlusMoins[choix];
        tabPlusMoinsTxt[choix].SetText(out.str());
        out.str(string());
        out << nbSkills + 1;
        statsPersoValues1[NB_POINTS_SKILLS].SetText(out.str());
    }
};

string Pause::convertText(string res)
{
    size_t cur=0;

    while ((cur=res.find("\\t"))<res.size() )
        res.replace(cur,2,"   ");
    while ((cur=res.find("\\n"))<res.size() )
        res.replace(cur,2,"\n");

    /////////////////////////////////////////////////////////////////////////
    /// Touches définies par l'utilisateur à afficher dans les parchemins
    /////////////////////////////////////////////////////////////////////////
    //saut
    while ((cur=res.find("\\jum")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->sauter) + "\""));
    //frappe
    while ((cur=res.find("\\hit")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->taper) + "\""));
    //courrir
    while ((cur=res.find("\\run")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->courrir) + "\""));
    //menu
    while ((cur=res.find("\\men")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->pause) + "\""));
    //retour
    while ((cur=res.find("\\esc")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touchesSys->retour) + "\""));
    //ramasser - utiliser
    while ((cur=res.find("\\use")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->ramasse) + "\""));

    //si on doit faire des retours à la ligne
    if (res.size() > nbCharParLigne)
        insertRetourLigne(&res);

    return res;
};

inline void Pause::insertRetourLigne(string *res)
{
    const unsigned int taille = res->size();
    unsigned int nbLettres = 0;
    unsigned int nbCharEcrisParLigne = 0;
    unsigned int curseur = 0;

    while (curseur < taille) //pour tous les mots...
    {
        //on va à la fin du mot courant
        unsigned int i=curseur;
        nbLettres=0; //va contenir le nombre de lettres du mot

        while (((*res)[i]!=' ' and (*res)[i]!='\n') and i<taille) //parcours du mot
        {
            ++nbLettres; //compter le nombre de lettres
            ++i;
        }

        if ((*res)[i]=='\n' and nbLettres>=1) //si il y a un retour à la ligne spécifié dans l'éditeur
        {
            nbCharEcrisParLigne=0;
        }

        if (i>=taille) //sécurité : en cas de fin de chaîne, on arrête
            break;

        nbCharEcrisParLigne+=nbLettres; //on rajoute les lettres du mot au nombre de caracères écrits dans la ligne

        if (nbCharEcrisParLigne >= nbCharParLigne) //si on a dépassé la longueur de la ligne
        { //on force un retour à la ligne
            (*res)[curseur-1]='\n';
            nbCharEcrisParLigne=0;
        }
        else //sinon on continue, on passe au mot suivant
            curseur += nbLettres+1;
    }
};

void Pause::saveChanges(Event event)
{
    sf::Key::Code c = event.Key.Code;

    if (touchesHerosMenued)
    {
        switch (choixGauche)
        {
            case CTRL_COURRIR :
                touches->courrir = c;
                break;

            case CTRL_SAUTER :
                touches->sauter = c;
                break;

            case CTRL_DROITE :
                touches->droite = c;
                break;

            case CTRL_GAUCHE :
                touches->gauche = c;
                break;

            case CTRL_TAPER :
                touches->taper = c;
                break;

            case CTRL_PAUSE :
                touches->pause = c;
                break;

            case CTRL_SCREENSHOT :
                touches->screenshot = c;
                break;

            case CTRL_RAMASSE :
                touches->ramasse = c;
                break;

            case CTRL_GUI1 :
                touches->gui1 = c;
                break;

            case CTRL_GUI2 :
                touches->gui2 = c;
                break;

            case CTRL_GUI3 :
                touches->gui3 = c;
                break;

            case CTRL_GUI4 :
                touches->gui4 = c;
                break;

            case CTRL_GUI5 :
                touches->gui5 = c;
                break;

            case CTRL_GUI6 :
                touches->gui6 = c;
                break;

            case CTRL_GUI7 :
                touches->gui7 = c;
                break;

            case CTRL_GUI8 :
                touches->gui8 = c;
                break;

            case CTRL_GUI9 :
                touches->gui9 = c;
                break;

            case CTRL_GUI10 :
                touches->gui10 = c;
                break;

            default :
                break;
        }
    }
    else if (touchesSysMenued)
    {
        switch (choixGauche)
        {
            case CTRLSYS_GAUCHE :
                touchesSys->gauche = c;
                break;

            case CTRLSYS_DROITE :
                touchesSys->droite = c;
                break;

            case CTRLSYS_HAUT :
                touchesSys->haut = c;
                break;

            case CTRLSYS_BAS :
                touchesSys->bas = c;
                break;

            case CTRLSYS_RETOUR :
                touchesSys->retour = c;
                break;

            case CTRLSYS_ENTREE :
                touchesSys->entree = c;
                break;

            default :
                break;
        }
    }
};

std::string Pause::findKey(Key::Code &e)
{
    std::string touche;

    switch (e)
    {
        case Key::A :
            touche = "A";
            break;

        case Key::B :
            touche = "B";
            break;

        case Key::C :
            touche = "C";
            break;

        case Key::D :
            touche = "D";
            break;

        case Key::E :
            touche = "E";
            break;

        case Key::F :
            touche = "F";
            break;

        case Key::G :
            touche = "G";
            break;

        case Key::H :
            touche = "H";
            break;

        case Key::I :
            touche = "I";
            break;

        case Key::J :
            touche = "J";
            break;

        case Key::K :
            touche = "K";
            break;

        case Key::L :
            touche = "L";
            break;

        case Key::M :
            touche = "M";
            break;

        case Key::N :
            touche = "N";
            break;

        case Key::O :
            touche = "O";
            break;

        case Key::P :
            touche = "P";
            break;

        case Key::Q :
            touche = "Q";
            break;

        case Key::R :
            touche = "R";
            break;

        case Key::S :
            touche = "S";
            break;

        case Key::T :
            touche = "T";
            break;

        case Key::U :
            touche = "U";
            break;

        case Key::V :
            touche = "V";
            break;

        case Key::W :
            touche = "W";
            break;

        case Key::X :
            touche = "X";
            break;

        case Key::Y :
            touche = "Y";
            break;

        case Key::Z :
            touche = "Z";
            break;

        case Key::Num0 :
            touche = "0";
            break;

        case Key::Num1 :
            touche = "1";
            break;

        case Key::Num2 :
            touche = "2";
            break;

        case Key::Num3 :
            touche = "3";
            break;

        case Key::Num4 :
            touche = "4";
            break;

        case Key::Num5 :
            touche = "5";
            break;

        case Key::Num6 :
            touche = "6";
            break;

        case Key::Num7 :
            touche = "7";
            break;

        case Key::Num8 :
            touche = "8";
            break;

        case Key::Num9 :
            touche = "9";
            break;

        case Key::Escape :
            touche = "Echap";
            break;

        case Key::LControl :
            touche = "Ctrl gauche";
            break;

        case Key::LShift :
            touche = "Shift gauche";
            break;

        case Key::LAlt :
            touche = "Alt gauche";
            break;

        case Key::LSystem :
            touche = "System gauche";
            break;

        case Key::RControl :
            touche = "Ctrl droite";
            break;

        case Key::RShift :
            touche = "Shift droite";
            break;

        case Key::RAlt :
            touche = "Alt gr";
            break;

        case Key::RSystem :
            touche = "System droite";
            break;

        case Key::Menu :
            touche = "Menu";
            break;

        case Key::LBracket :
            touche = "Bracket gauche";
            break;

        case Key::RBracket :
            touche = "Bracket droite";
            break;

        case Key::SemiColon :
            touche = "SemiColon";
            break;

        case Key::Comma :
            touche = "Virgule";
            break;

        case Key::Period :
            touche = "Period";
            break;

        case Key::Quote :
            touche = "\"";
            break;

        case Key::Slash :
            touche = "/";
            break;

        case Key::BackSlash :
            touche = "\\";
            break;

        case Key::Tilde :
            touche = "~";
            break;

        case Key::Equal :
            touche = "=";
            break;

        case Key::Dash :
            touche = "Dash";
            break;

        case Key::Space :
            touche = "Espace";
            break;

        case Key::Return :
            touche = "Entree";
            break;

        case Key::Back :
            touche = "retour chariot";
            break;

        case Key::Tab :
            touche = "Tab";
            break;

        case Key::PageUp :
            touche = "Page Up";
            break;

        case Key::PageDown :
            touche = "Page Down";
            break;

        case Key::End :
            touche = "Fin";
            break;

        case Key::Home :
            touche = "Home";
            break;

        case Key::Insert :
            touche = "Inserer";
            break;

        case Key::Delete :
            touche = "Delete";
            break;

        case Key::Add :
            touche = "+";
            break;

        case Key::Subtract :
            touche = "-";
            break;

        case Key::Multiply :
            touche = "*";
            break;

        case Key::Divide :
            touche = "/";
            break;

        case Key::Left :
            touche = "Gauche";
            break;

        case Key::Right :
            touche = "Droite";
            break;

        case Key::Up :
            touche = "Haut";
            break;

        case Key::Down :
            touche = "Bas";
            break;

        case Key::Numpad0 :
            touche = "0";
            break;

        case Key::Numpad1 :
            touche = "1";
            break;

        case Key::Numpad2 :
            touche = "2";
            break;

        case Key::Numpad3 :
            touche = "3";
            break;

        case Key::Numpad4 :
            touche = "4";
            break;

        case Key::Numpad5 :
            touche = "5";
            break;

        case Key::Numpad6 :
            touche = "6";
            break;

        case Key::Numpad7 :
            touche = "7";
            break;

        case Key::Numpad8 :
            touche = "8";
            break;

        case Key::Numpad9 :
            touche = "9";
            break;

        case Key::F1 :
            touche = "F1";
            break;

        case Key::F2 :
            touche = "F2";
            break;

        case Key::F3 :
            touche = "F3";
            break;

        case Key::F4 :
            touche = "F4";
            break;

        case Key::F5 :
            touche = "F5";
            break;

        case Key::F6 :
            touche = "F6";
            break;

        case Key::F7 :
            touche = "F7";
            break;

        case Key::F8 :
            touche = "F8";
            break;

        case Key::F9 :
            touche = "F9";
            break;

        case Key::F10 :
            touche = "F10";
            break;

        case Key::F11 :
            touche = "F11";
            break;

        case Key::F12 :
            touche = "F12";
            break;

        case Key::F13 :
            touche = "F13";
            break;

        case Key::F14 :
            touche = "F14";
            break;

        case Key::F15 :
            touche = "F15";
            break;

        case Key::Pause :
            touche = "Pause";
            break;

        default :
            touche = toucheTemp;
            break;
    }

    return touche;
};


