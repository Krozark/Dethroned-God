#include "MenuSauvegarde.hpp"

using namespace sf;
using namespace std;

////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
MenuSauvegarde::MenuSauvegarde(_touches_sys* touches_Sys, Sons* so, bool load)
{
    is_load = load;
    load_game = false;
    save_game = false;
    exit=false;

    texteEmplacementLibre = "~Emplacement libre~";

    //structures de données
    touchesSys = touches_Sys; //touches clavier
    sons = so; //sons

    save_state = STATE_NONE;
    save_slot = 1;
    InitImages();
    InitStrings();
}

////////////////////////////////////////////////////////////
/// Events gestion
////////////////////////////////////////////////////////////
void MenuSauvegarde::getEvents(Event event)
{
    if (event.Key.Code == touchesSys->bas)
        keyDown();
    else if (event.Key.Code == touchesSys->haut)
        keyUp();
    else if (event.Key.Code == touchesSys->droite  or  event.Key.Code == touchesSys->gauche)
        keyRightLeft();
    else if (event.Key.Code == touchesSys->entree)
        keyReturn();
}

////////////////////////////////////////////////////////////
/// Events : down
////////////////////////////////////////////////////////////
inline void MenuSauvegarde::keyDown()
{
    if (save_state == STATE_NONE)
    {
        if (++save_slot > NB_SAVE_SLOTS)
        {
            save_slot = 1;
        }
        sprSaveSlotCursor.SetY(save_slot*FENETREY/4);
        sons->playSon(MENUBOUGER);
    }
}

////////////////////////////////////////////////////////////
/// Events : up
////////////////////////////////////////////////////////////
inline void MenuSauvegarde::keyUp()
{
    if (save_state == STATE_NONE)
    {
        if (--save_slot < 1)
        {
            save_slot = NB_SAVE_SLOTS;
        }
        sprSaveSlotCursor.SetY(save_slot*FENETREY/4);
        sons->playSon(MENUBOUGER);
    }
}

////////////////////////////////////////////////////////////
/// Events : right or left
////////////////////////////////////////////////////////////
inline void MenuSauvegarde::keyRightLeft()
{
    if (save_state == STATE_QUESTION)
    {
        sons->playSon(MENUBOUGER);
        confirm_choice = !confirm_choice;
        if (confirm_choice)
        {
            sprSaveButtonCursor.SetX(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/4-sprSaveButtonCursor.GetSize().x/2);
        }
        else
        {
            sprSaveButtonCursor.SetX(sprSaveConfirm.GetPosition().x+3*sprSaveConfirm.GetSize().x/4-sprSaveButtonCursor.GetSize().x/2);
        }
    }
}

////////////////////////////////////////////////////////////
/// Events : validate
////////////////////////////////////////////////////////////
inline void MenuSauvegarde::keyReturn()
{
    //si on essaie pas de charger une partie vide
    if ((string(strSaveSlotNomLevel[save_slot-1].GetText()) != texteEmplacementLibre  and  is_load)  or  !is_load)
        sons->playSon(MENUVALIDER);

    switch (save_state)
    {
        case STATE_QUESTION :
            if (confirm_choice)
            {
                if (is_load)
                {
                    save_state = STATE_NONE;
                    load_game = true;
                }
                else
                {
                    save_state = STATE_GAMESAVED;
                    save_game = true;
                    sprSaveButton.SetX(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/2-sprSaveButton.GetSize().x/2);
                    sprSaveButtonCursor.SetX(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/2-sprSaveButtonCursor.GetSize().x/2);
                    strSaveConfirm.SetText("Partie Sauvegardée.");
                    strSaveConfirm.SetPosition(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/2-strSaveConfirm.GetRect().GetWidth()/2,sprSaveConfirm.GetPosition().y+sprSaveConfirm.GetSize().y/4-strSaveConfirm.GetRect().GetHeight()/2);
                }
            }
            else
            {
                save_state = STATE_NONE;
            }
            break;

        case STATE_GAMESAVED :
            save_state = STATE_NONE;
            exit=true;
            break;

        default :
            //vérif pour savoir si l'emplacement contient une partie
            if ((string(strSaveSlotNomLevel[save_slot-1].GetText()) != texteEmplacementLibre  and  is_load)  or  !is_load)
            {
                save_state = STATE_QUESTION;
                sprSaveButtonCursor.SetX(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/4-sprSaveButtonCursor.GetSize().x/2);
                strSaveConfirm.SetText(is_load?"Charger cette partie ?":"Voulez-vous sauvegarder ?");
                strSaveConfirm.SetPosition(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/2-strSaveConfirm.GetRect().GetWidth()/2,sprSaveConfirm.GetPosition().y+sprSaveConfirm.GetSize().y/4-strSaveConfirm.GetRect().GetHeight()/2);
            }
            else
                sons->playSon(SON_ERROR_MENU);
            break;
    }
    confirm_choice = true;
}

////////////////////////////////////////////////////////////
/// Load the images used in the save menu
////////////////////////////////////////////////////////////
void MenuSauvegarde::InitImages()
{
    imgSaveTitle.LoadFromFile("./resources/Save/title_save.png");
    sprSaveTitle.SetImage(imgSaveTitle);
    sprSaveTitle.Resize(FENETREX,FENETREY/4);
    sprSaveTitle.SetPosition(0,0);

    imgSaveSlot.LoadFromFile("./resources/Save/slot_save.png");
    sprSaveSlot.SetImage(imgSaveSlot);
    sprSaveSlot.Resize(FENETREX,FENETREY/4);
    sprSaveSlot.SetX(0);

    imgSaveSlotCursor.LoadFromFile("./resources/Save/cursor_slot_save.png");
    sprSaveSlotCursor.SetImage(imgSaveSlotCursor);
    sprSaveSlotCursor.Resize(FENETREX,FENETREY/4);
    sprSaveSlotCursor.SetPosition(0,FENETREY/4);

    imgSaveConfirm.LoadFromFile("./resources/Save/confirm_save.png");
    sprSaveConfirm.SetImage(imgSaveConfirm);
    sprSaveConfirm.SetPosition(FENETREX/2-sprSaveConfirm.GetSize().x/2,FENETREY/2-sprSaveConfirm.GetSize().y/2);

    imgSaveButton.LoadFromFile("./resources/Save/button_save.png");
    sprSaveButton.SetImage(imgSaveButton);
    sprSaveButton.SetY(sprSaveConfirm.GetPosition().y+3*sprSaveConfirm.GetSize().y/4-sprSaveButton.GetSize().y/2);

    imgSaveButtonCursor.LoadFromFile("./resources/Save/cursor_button_save.png");
    sprSaveButtonCursor.SetImage(imgSaveButtonCursor);
    sprSaveButtonCursor.SetY(sprSaveConfirm.GetPosition().y+3*sprSaveConfirm.GetSize().y/4-sprSaveButtonCursor.GetSize().y/2);
}

////////////////////////////////////////////////////////////
/// Define the strings used in the save menu
////////////////////////////////////////////////////////////
void MenuSauvegarde::InitStrings()
{
    int size=19;

    strSaveTitle.SetText(is_load?"Menu de Chargement":"Menu de Sauvegarde");
    strSaveTitle.SetPosition(sprSaveTitle.GetPosition().x+sprSaveTitle.GetSize().x/2-strSaveTitle.GetRect().GetWidth()/2,sprSaveTitle.GetPosition().y+sprSaveTitle.GetSize().y/2-strSaveTitle.GetRect().GetHeight()/2);
    strSaveTitle.SetSize(size+10);

    //initialisation of the slots's text
    majSlotsText();

    strSaveConfirm.SetText("");
    strSaveConfirm.SetSize(size);

    strSaveYes.SetText("Oui");
    strSaveYes.SetSize(size);
    strSaveYes.SetPosition(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/4-strSaveYes.GetRect().GetWidth()/2,sprSaveConfirm.GetPosition().y+3*sprSaveConfirm.GetSize().y/4-strSaveYes.GetRect().GetHeight()/2);
    strSaveNo.SetText("Non");
    strSaveNo.SetSize(size);
    strSaveNo.SetPosition(sprSaveConfirm.GetPosition().x+3*sprSaveConfirm.GetSize().x/4-strSaveNo.GetRect().GetWidth()/2,sprSaveConfirm.GetPosition().y+3*sprSaveConfirm.GetSize().y/4-strSaveNo.GetRect().GetHeight()/2);
    strSaveOk.SetText("Ok");
    strSaveOk.SetSize(size);
    strSaveOk.SetPosition(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/2-strSaveOk.GetRect().GetWidth()/2,sprSaveConfirm.GetPosition().y+3*sprSaveConfirm.GetSize().y/4-strSaveOk.GetRect().GetHeight()/2);
};

inline void MenuSauvegarde::majSlotsText()
{
    for (int i=0; i<NB_SAVE_SLOTS; ++i)
    {
        majOneSlot(i);
    }

    //properties of the texts
    for (int i=0; i<NB_SAVE_SLOTS; ++i)
    {
        strSaveSlotMythologie[i].SetSize(20);
        strSaveSlotMythologie[i].SetY(((i+1)*FENETREY/4)+sprSaveSlot.GetSize().y/4);
        strSaveSlotMythologie[i].SetStyle(String::Bold);
        strSaveSlotNomLevel[i].SetSize(19);
        strSaveSlotNomLevel[i].SetY(strSaveSlotMythologie[i].GetPosition().y+strSaveSlotMythologie[i].GetRect().GetHeight()+(15*FENETREY/900.0));
        strSaveSlotTypeLevel[i].SetSize(19);
        strSaveSlotTypeLevel[i].SetY(strSaveSlotNomLevel[i].GetPosition().y+strSaveSlotNomLevel[i].GetRect().GetHeight()+(15*FENETREY/900.0));
        strSaveSlotTime[i].SetSize(19);
        strSaveSlotTime[i].SetY(strSaveSlotTypeLevel[i].GetPosition().y+strSaveSlotNomLevel[i].GetRect().GetHeight()+(15*FENETREY/900.0));
    }
}

inline void MenuSauvegarde::majOneSlot(int i,bool init)
{
    //opening the current save file
    stringstream out;
    out << "./data/sav" << i+1;
    FILE* file = NULL;
    file = fopen(out.str().c_str(),"rb");
    out.str(string());

    //if it exists
    if (file)
    {
        _sauvegarde save;
        fread(&save,sizeof(_sauvegarde),1,file);
        stringstream out;

        //mythologie
        out << "Mythologie : " << save.mythologieLevel;
        strSaveSlotMythologie[i].SetText(out.str());
        if (init)
        {
            strSaveSlotMythologie[i].SetCenter(strSaveSlotMythologie[i].GetCenter().x,strSaveSlotMythologie[i].GetRect().GetHeight()/2.0);
            strSaveSlotMythologie[i].SetX(FENETREX/2.0+40-strSaveSlotMythologie[i].GetRect().GetWidth()/2.0);
        }
        out.str(string());

        //level
        out << "Level " << save.nomVerifLevel << " - " << save.nomLevel;
        strSaveSlotNomLevel[i].SetText(out.str());
        if (init)
        {
            strSaveSlotNomLevel[i].SetX(50);
            strSaveSlotNomLevel[i].SetStyle(String::Regular);
            strSaveSlotNomLevel[i].SetCenter(strSaveSlotNomLevel[i].GetCenter().x,strSaveSlotNomLevel[i].GetRect().GetHeight()/2.0);
        }
        out.str(string());

        //type de level
        out << "Type de niveau : " << save.typeLevel;
        strSaveSlotTypeLevel[i].SetText(out.str());
        if (init)
        {
            strSaveSlotTypeLevel[i].SetX(50);
            strSaveSlotTypeLevel[i].SetCenter(strSaveSlotTypeLevel[i].GetCenter().x,strSaveSlotTypeLevel[i].GetRect().GetHeight()/2.0);
        }
        out.str(string());

        //temps de jeu
        majTimeGame(save,i);
        if (init)
        {
            strSaveSlotTime[i].SetX(50);
            strSaveSlotTime[i].SetCenter(strSaveSlotTime[i].GetCenter().x,strSaveSlotTime[i].GetRect().GetHeight()/2.0);
        }

        fclose(file);
    }
    //if it doesn't exists
    else
    {
        strSaveSlotNomLevel[i].SetText(texteEmplacementLibre);
        strSaveSlotNomLevel[i].SetStyle(3); //italic
        strSaveSlotNomLevel[i].SetX(FENETREX/2.0+40-strSaveSlotNomLevel[i].GetRect().GetWidth()/2.0);
        strSaveSlotMythologie[i].SetText("");
        strSaveSlotMythologie[i].SetX(-1);
        strSaveSlotTypeLevel[i].SetText("");
        strSaveSlotTypeLevel[i].SetX(-1);
        strSaveSlotTime[i].SetText("");
        strSaveSlotTime[i].SetX(-1);
    }
}

void MenuSauvegarde::majTimeGame(_sauvegarde save, int i)
{
    int secondes = save.totalGameTime;
    int minutes = secondes / 60;
    int heures = minutes / 60;
    secondes = secondes%60;
    stringstream out;
    out << "Temps de jeu :  " << heures << "h" << minutes << "m" << secondes << "s";
    strSaveSlotTime[i].SetText(out.str());
}


////////////////////////////////////////////////////////////
/// Draw on the screen the menu title and the save slots
////////////////////////////////////////////////////////////
void MenuSauvegarde::Draw(RenderTarget& target)
{
    target.Clear(Color(50,0,0));

    //Draw the title
    target.Draw(sprSaveTitle);
    target.Draw(strSaveTitle);

    //Draw the slots
    for (int i = 1 ; i <= NB_SAVE_SLOTS ; ++i)
    {
        sprSaveSlot.SetY(i*FENETREY/4);
        target.Draw(sprSaveSlot);
        target.Draw(strSaveSlotMythologie[i-1]);
        target.Draw(strSaveSlotNomLevel[i-1]);
        target.Draw(strSaveSlotTypeLevel[i-1]);
        target.Draw(strSaveSlotTime[i-1]);
    }
    target.Draw(sprSaveSlotCursor);

    //Draw the message box
    switch (save_state)
    {
        case STATE_QUESTION :
            DrawMessageBox(target);
            break;

        case STATE_GAMESAVED :
            DrawMessageBox(target, false);
            break;

        default :
            //Doesn't do anything
            break;
    }
}

////////////////////////////////////////////////////////////
/// Draw on the screen a message box
////////////////////////////////////////////////////////////
void MenuSauvegarde::DrawMessageBox(RenderTarget& target, bool question)
{
    target.Draw(sprSaveConfirm);
    target.Draw(strSaveConfirm);
    if (question)
    {
        //Draw the "Yes" and "No" buttons
        sprSaveButton.SetX(sprSaveConfirm.GetPosition().x+sprSaveConfirm.GetSize().x/4-sprSaveButton.GetSize().x/2);
        target.Draw(sprSaveButton);
        sprSaveButton.SetX(sprSaveConfirm.GetPosition().x+3*sprSaveConfirm.GetSize().x/4-sprSaveButton.GetSize().x/2);
        target.Draw(sprSaveButton);
        target.Draw(strSaveYes);
        target.Draw(strSaveNo);
    }
    else
    {
        //Draw the "Ok" button
        target.Draw(sprSaveButton);
        target.Draw(strSaveOk);
    }
    target.Draw(sprSaveButtonCursor);
}

////////////////////////////////////////////////////////////
/// Save the game data with the given index
////////////////////////////////////////////////////////////
void MenuSauvegarde::SaveGame(_sauvegarde* save,_touches_heros* touches,_options* settings)
{
    //open the file relating to the current slot
    stringstream out;
    out << "./data/sav" << save_slot;
    FILE* file = NULL;
    file = fopen(out.str().c_str(),"wb");

    //write into the file
    fwrite(save,sizeof(_sauvegarde),1,file);
    fclose(file);

    FILE* fichier=NULL;
    fichier = fopen("./data/settings","wb");
    if (fichier != NULL)
    {
        //écriture des options de base
        fwrite(settings,sizeof(_options),1,fichier);

        //écriture des touches héros
        fwrite(touches,sizeof(_touches_heros),1,fichier);

        //écriture des touches système
        fwrite(touchesSys,sizeof(_touches_sys),1,fichier);

        //fermeture du fichier
        fclose(fichier);
    }

    //exit the save_game's state
    save_game=false;

    //refresh display
    majOneSlot(save_slot-1,(string(strSaveSlotNomLevel[save_slot-1].GetText()) == texteEmplacementLibre) ? true : false);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// Load the game if there is one at the current slot
///////////////////////////////////////////////////////////////////////////////////////////////
void MenuSauvegarde::LoadGame(_sauvegarde* save, _touches_heros* touches)
{
    //check if there is a game to load from the current slot
    stringstream out;
    out << "./data/sav" << save_slot;
    FILE* file = NULL;
    file = fopen(out.str().c_str(),"rb");
    if (file)
    {
        fread(save,sizeof(_sauvegarde),1,file);
        fclose(file);
    }
    else
        save=NULL;

    file = fopen("./data/settings","rb");
    if (file)
    {
        _options settings;
        fread(&settings,sizeof(_options),1,file);
        fread(touches,sizeof(_touches_heros),1,file);
        fclose(file);
    }
    else
        touches=NULL;

    //exit the load_game's state
    load_game=false;
}
