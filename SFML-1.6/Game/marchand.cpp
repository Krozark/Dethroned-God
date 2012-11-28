#include "marchand.hpp"

using namespace std;
using namespace sf;

////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Marchand::Marchand(RenderWindow *app, _touches_sys* touches_Sys, Hero* h)
{
    this->App = app;
    this->heros = h;
    touchesSys = touches_Sys;
    InitImages();
    InitStrings();
    InitComps();

    state_buy_sell = STATE_QUESTION;
    state_action = STATE_CHOOSE;

    buy_sell_cursor = 0;
    items_cursor = 0;
    choice_cursor = true;

    must_update = false;
}

////////////////////////////////////////////////////////////
/// Events gestion
////////////////////////////////////////////////////////////
void Marchand::getEvents(Event event)
{
    if (event.Key.Code == touchesSys->bas)
        keyDown();
    else if (event.Key.Code == touchesSys->haut)
        keyUp();
    else if (event.Key.Code == touchesSys->droite)
        keyRight();
    else if (event.Key.Code == touchesSys->gauche)
        keyLeft();
    else if (event.Key.Code == touchesSys->entree)
        keyEnter();
    else if (event.Key.Code == touchesSys->retour)
        keyReturn();
}

////////////////////////////////////////////////////////////
/// Event: Down
////////////////////////////////////////////////////////////
inline void Marchand::keyDown()
{
    switch (state_buy_sell)
    {
        case STATE_QUESTION :
        {
            if (++buy_sell_cursor > 2)
            {
                buy_sell_cursor = 0;
            }
            break;
        }
        case STATE_BUY :
        case STATE_SELL :
        {
            // Avoid choosing an object we don't have
            do
            {
                if (++items_cursor >= items_number)
                {
                    items_cursor = 0;
                }
            } while ((*current_objects)[items_cursor].show == false);
            strDescription.SetText((*current_objects)[items_number].description);
            break;
        }
    }
}

////////////////////////////////////////////////////////////
/// Event: Up
////////////////////////////////////////////////////////////
inline void Marchand::keyUp()
{
    switch (state_buy_sell)
    {
        case STATE_QUESTION :
        {
            if (--buy_sell_cursor < 0)
            {
                buy_sell_cursor = 2;
            }
            break;
        }
        case STATE_BUY :
        case STATE_SELL :
        {
            // Avoid choosing an object we don't have
            do
            {
                if (--items_cursor < 0)
                {
                    items_cursor = items_number - 1;
                }
            } while ((*current_objects)[items_cursor].show == false);
            strDescription.SetText((*current_objects)[items_number].description);
            break;
        }
    }
}

////////////////////////////////////////////////////////////
/// Event: Left
////////////////////////////////////////////////////////////
inline void Marchand::keyLeft()
{
    switch (state_buy_sell)
    {
        case STATE_BUY :
        case STATE_SELL :
        {
            switch (state_action)
            {
                case STATE_CHOOSE :
                {
                    // Decrease the number of occurences of the object we want to buy/sell
                    if (items_tobuysell_number > 0)
                    {
                        --items_tobuysell_number;
                    }
                    break;
                }
                case STATE_VALIDATION :
                {
                    choice_cursor = !choice_cursor;
                    if (choice_cursor)
                    {
                        sprValidationBoxCursor.SetX(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/4-sprValidationBoxCursor.GetSize().x/2);
                    }
                    else
                    {
                        sprValidationBoxCursor.SetX(sprValidationBox.GetPosition().x+3*sprValidationBox.GetSize().x/4-sprValidationBoxCursor.GetSize().x/2);
                    }
                    break;
                }
                default : break;
            }
        }
        break;
        default : break;
    }
}

////////////////////////////////////////////////////////////
/// Event: Right
////////////////////////////////////////////////////////////
inline void Marchand::keyRight()
{
    switch (state_buy_sell)
    {
        case STATE_BUY :
        case STATE_SELL :
        {
            switch (state_action)
            {
                case STATE_CHOOSE :
                {
                    // Increase the number of occurences of the object we want to buy/sell
                    if (items_tobuysell_number < (*current_objects)[items_cursor].number)
                    {
                        ++items_tobuysell_number;
                    }
                    break;
                }
                case STATE_VALIDATION :
                {
                    choice_cursor = !choice_cursor;
                    if (choice_cursor)
                    {
                        sprValidationBoxCursor.SetX(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/4-sprValidationBoxCursor.GetSize().x/2);
                    }
                    else
                    {
                        sprValidationBoxCursor.SetX(sprValidationBox.GetPosition().x+3*sprValidationBox.GetSize().x/4-sprValidationBoxCursor.GetSize().x/2);
                    }
                    break;
                }
                default : break;
            }
            break;
        }
        default : break;
    }
}

////////////////////////////////////////////////////////////
/// Event: Enter
////////////////////////////////////////////////////////////
inline void Marchand::keyEnter()
{
    if (state_buy_sell == STATE_QUESTION)
    {
        switch (buy_sell_cursor)
        {
            items_cursor = 0;
            case 0 :
                state_buy_sell = STATE_BUY;
                strItemsTitle.SetText("Produits en vente");
                strItemsTitle.SetX(sprItemsBox.GetPosition().x+sprItemsBox.GetSize().x/2-strItemsTitle.GetRect().GetWidth()/2);
                items_number = hero_objects.size();
                for (int i = 0 ; i < items_number ; ++i)
                {
                    (*other_objects)[i] = hero_objects[i];
                }
                items_number = merchant_objects.size();
                for (int i = 0 ; i < items_number ; ++i)
                {
                    (*current_objects)[i] = merchant_objects[i];
                }
                break;
            case 1 :
                state_buy_sell = STATE_SELL;
                strItemsTitle.SetText("Produits en stock");
                strItemsTitle.SetX(sprItemsBox.GetPosition().x+sprItemsBox.GetSize().x/2-strItemsTitle.GetRect().GetWidth()/2);
                items_number = merchant_objects.size();
                for (int i = 0 ; i < items_number ; ++i)
                {
                    (*other_objects)[i] = merchant_objects[i];
                }
                items_number = hero_objects.size();
                for (int i = 0 ; i < items_number ; ++i)
                {
                    (*current_objects)[i] = hero_objects[i];
                }
                // Avoid choosing an object we don't have
                while ((*current_objects)[items_cursor].show == false)
                {
                    if (++items_cursor >= items_number)
                    {
                        items_cursor = 0;
                    }
                }
                break;
            default :
                Quit();
                break;
        } // buy_sell_cursor
    }
    else // state_buy_sell = STATE_BUY or STATE_SELL
    {
        switch (state_action)
        {
            case STATE_CHOOSE :
            {
                Validation(STATE_BUY?"Voulez-vous vraiment acheter cet objet ?":"Voulez-vous vraiment vendre cet objet ?");
                break;
            }
            case STATE_VALIDATION :
            {
                if (choice_cursor)
                {
                    if ((*current_objects)[items_cursor].number != 0)
                    {
                        int _gold = (*current_objects)[items_cursor].price * items_tobuysell_number;
                        if (STATE_BUY?(heros->tab_objets[OR_LOOT] >= _gold):(money >= _gold))
                        {
                            // Modify your money and the merchant's
                            stringstream out;
                            if (state_buy_sell == STATE_BUY)
                            {
                                (*current_objects)[items_cursor].number -= items_tobuysell_number;
                                (*other_objects)[items_cursor].number += items_tobuysell_number;
                                heros->tab_objets[OR_LOOT] -= _gold;
                                money += _gold;
                            }
                            else
                            {
                                money -= _gold;
                                heros->tab_objets[OR_LOOT] += _gold;
                            }
                            out << money;
                            strMerchantMoney.SetText(out.str());
                            out << heros->tab_objets[OR_LOOT];
                            strMyMoney.SetText(out.str());
                        }
                        else
                        {
                            Warning(STATE_BUY?"Vous n'avez pas assez d'argent.":"Je n'ai pas assez d'argent.");
                            break;
                        }
                    }
                    else
                    {
                        Warning("Cet objet n'est plus en vente.");
                        break;
                    }
                }
                choice_cursor = true;
                items_tobuysell_number = 0;
            }
            case STATE_WARNING :
            {
                state_action = STATE_CHOOSE;
                break;
            }
        } // state_action
    }
}

////////////////////////////////////////////////////////////
/// Event: Return
////////////////////////////////////////////////////////////
inline void Marchand::keyReturn()
{
    switch (state_buy_sell)
    {
        case STATE_QUESTION :
        {
            Quit();
            break;
        }
        case STATE_BUY :
        case STATE_SELL :
        {
            switch (state_action)
            {
                case STATE_CHOOSE :
                {
                    state_buy_sell = STATE_QUESTION;
                    items_tobuysell_number = 0;
                    strDescription.SetText("Bienvenue dans notre humble commerce. Que voulez-vous faire ?");
                    break;
                }
                case STATE_VALIDATION : choice_cursor = false;
                case STATE_WARNING :
                {
                    state_action = STATE_CHOOSE;
                    break;
                }
            }
            break;
        }
    }
}

////////////////////////////////////////////////////////////
/// Draw on the screen the different boxes
////////////////////////////////////////////////////////////
void Marchand::Draw()
{
    stringstream out;
    App->Draw(sprMerchantHeadBox);
    App->Draw(sprDescriptionBox);
    App->Draw(strDescription);

    switch (state_buy_sell)
    {
        case STATE_QUESTION :
        {
            App->Draw(sprBuySellBox);
            App->Draw(sprBuySellBoxCursor);
            App->Draw(strBuy);
            App->Draw(strSell);
            App->Draw(strQuit);
            break;
        }
        case STATE_BUY :
        {
            App->Draw(sprCompetencesBox);
            App->Draw(strCompetencesTitle);
            // Draw the competences tab
            int _nb_comps = competences.size();
            for (int i = 0 ; i < _nb_comps ; ++i)
            {
                strCompetenceName.SetText(competences[i].name);
                strCompetenceName.SetY(sprCompetencesBox.GetPosition().y+10+(30+strCompetenceName.GetSize()*i));
                App->Draw(strCompetenceName);
            }
            // TODO: Draw the competences' symbols
        }
        case STATE_SELL :
        {
            App->Draw(sprMerchantMoneyBox);
            App->Draw(strMerchantMoneyTitle);
            App->Draw(strMerchantMoney);
            App->Draw(sprMyMoneyBox);
            App->Draw(strMyMoneyTitle);
            App->Draw(strMyMoney);

            App->Draw(sprItemsBox);
            App->Draw(strItemsTitle);
            //Draw the items tab
            int __y = sprItemsBox.GetPosition().y + 40;
            for (int i = 0 ; i < items_number ; ++i)
            {
                if ((*current_objects)[i].show)
                {
                    strItemName.SetText((*current_objects)[i].name);
                    strItemName.SetY(__y);
                    App->Draw(strItemName);
                    out << (*current_objects)[i].price;
                    strItemPrice.SetText(out.str());
                    strItemPrice.SetY(__y);
                    App->Draw(strItemPrice);
                    __y += 20;
                }
            }
            App->Draw(sprItemsBoxCursor);

            if (state_action == STATE_VALIDATION || state_action == STATE_WARNING)
            {
                App->Draw(sprValidationBox);
                App->Draw(strValidation);
                if (state_action == STATE_VALIDATION)
                {
                    //Draw the "Yes" and "No" buttons
                    sprValidationBoxButton.SetX(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/4-sprValidationBoxButton.GetSize().x/2);
                    App->Draw(sprValidationBoxButton);
                    sprValidationBoxButton.SetX(sprValidationBox.GetPosition().x+3*sprValidationBox.GetSize().x/4-sprValidationBoxButton.GetSize().x/2);
                    App->Draw(sprValidationBoxButton);
                    App->Draw(strValidationYes);
                    App->Draw(strValidationNo);
                }
                else
                {
                    //Draw the "Ok" button
                    App->Draw(sprValidationBoxButton);
                    App->Draw(strValidationOk);
                }
                App->Draw(sprValidationBoxCursor);
            }
            break;
        }
    }
}

////////////////////////////////////////////////////////////
/// Show a warning window with the "Ok" button
////////////////////////////////////////////////////////////
void Marchand::Warning(string s)
{
    state_action = STATE_WARNING;
    strValidation.SetText(s);
    strValidation.SetPosition(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/2-strValidation.GetRect().GetWidth()/2,sprValidationBox.GetPosition().y+sprValidationBox.GetSize().y/4-strValidation.GetRect().GetHeight()/2);
}

////////////////////////////////////////////////////////////
/// Show a validation window with the "Yes" and "No" buttons
////////////////////////////////////////////////////////////
void Marchand::Validation(string s)
{
    state_action = STATE_VALIDATION;
    strValidation.SetText(s);
    strValidation.SetPosition(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/2-strValidation.GetRect().GetWidth()/2,sprValidationBox.GetPosition().y+sprValidationBox.GetSize().y/4-strValidation.GetRect().GetHeight()/2);
}

////////////////////////////////////////////////////////////
/// Update the items possessed by the hero
////////////////////////////////////////////////////////////
void Marchand::UpdateItems()
{
    hero_objects.clear();
    merchant_obj o;
    for (int i = 1 ; i < TAILLE_OBJETS_LOOT ; ++i)
    {
        if (i != PARCHEMIN_LOOT)
        {
            // To make an easier-to-use system to transfer items between the hero and the merchant,
            // all the hero's items will be registered in a vector.
            // The only items to be shown are those to be sold.
            o.show = heros->tab_objets[i] > 0;
            o.type = i;
            o.number = heros->tab_objets[i];
            switch (i)
            {
                case POTION_VIE_LOOT:
                    o.price = 50;
                    o.name = "Potion de Vie";
                    o.description = "Cette Potion de Vie régénère vos HP.";
                    break;
                case POTION_MANA_LOOT:
                    o.price = 150;
                    o.name = "Potion de Mana";
                    o.description = "Cette Potion de Mana restaure vos points de magie.";
                    break;
                case SAUVEGARDES_LOOT:
                    o.price = 250;
                    o.name = "Sauvegardeur";
                    o.description = "Cet objet vous permet de sauvegarder votre partie à tout moment du jeu.";
                    break;
                default:
                    cout << "Bug : Vous essayez de vendre un objet non conforme !" << endl;
            }
            hero_objects.push_back(o);
        }
    }
}

////////////////////////////////////////////////////////////
/// Called when we leave the merchant
////////////////////////////////////////////////////////////
void Marchand::Quit()
{
    items_cursor = 0;
    buy_sell_cursor = 0;
    items_tobuysell_number = 0;
    choice_cursor = false;
    state_buy_sell = STATE_QUESTION;
    state_action = STATE_CHOOSE;

    if (must_update)
    {
        items_number = hero_objects.size();
        if (items_number)
        {
            for (int i = 0 ; i < items_number ; ++i)
            {
                // Update the items' data in the hero
                heros->tab_objets[hero_objects[i].type] = hero_objects[i].number;
            }
        }
        must_update = false;
    }
}

////////////////////////////////////////////////////////////
/// Load the images used by merchants
////////////////////////////////////////////////////////////
void Marchand::InitImages()
{
    imgMerchantHeadBox.LoadFromFile("./resources/Marchand/merchant_head_box.png");
    sprMerchantHeadBox.SetImage(imgMerchantHeadBox);
    sprMerchantHeadBox.Resize(FENETREY/4, FENETREY/4);
    sprMerchantHeadBox.SetPosition(0, 0);

    imgDescriptionBox.LoadFromFile("./resources/Marchand/description_box.png");
    sprDescriptionBox.SetImage(imgDescriptionBox);
    sprDescriptionBox.Resize(FENETREX-FENETREY/4, FENETREY/4);
    sprDescriptionBox.SetPosition(FENETREY/4, 0);

    imgItemsBox.LoadFromFile("./resources/Marchand/items_box.png");
    sprItemsBox.SetImage(imgItemsBox);
    sprCompetencesBox.Resize((FENETREX+1)/3, 3*FENETREY/4-45);
    sprItemsBox.SetPosition(0, FENETREY/4);

    imgItemsBoxCursor.LoadFromFile("./resources/Marchand/items_box_cursor.png");
    sprItemsBoxCursor.SetImage(imgItemsBoxCursor);
    sprItemsBoxCursor.SetX(sprItemsBox.GetPosition().x+4);

    imgCompetencesBox.LoadFromFile("./resources/Marchand/competences_box.png");
    sprCompetencesBox.SetImage(imgCompetencesBox);
    sprCompetencesBox.Resize(FENETREY/4, 3*FENETREY/4-45);
    sprCompetencesBox.SetPosition(FENETREX-sprCompetencesBox.GetSize().x, FENETREY/4);

    imgMoneyBox.LoadFromFile("./resources/Marchand/money_box.png");
    sprMyMoneyBox.SetImage(imgMoneyBox);
    sprMyMoneyBox.Resize(sprCompetencesBox.GetSize().x, 45);
    sprMyMoneyBox.SetPosition(sprCompetencesBox.GetPosition().x, FENETREY-sprMyMoneyBox.GetSize().y);
    sprMerchantMoneyBox.SetImage(imgMoneyBox);
    sprMerchantMoneyBox.Resize(sprItemsBox.GetSize().x, 45);
    sprMerchantMoneyBox.SetPosition(0, FENETREY-sprMerchantMoneyBox.GetSize().y);

    imgValidationBox.LoadFromFile("./resources/Marchand/validation_box.png");
    sprValidationBox.SetImage(imgValidationBox);
    sprValidationBox.SetPosition(FENETREX/2-sprValidationBox.GetSize().x/2,FENETREY/2-sprValidationBox.GetSize().y/2);

    imgValidationBoxButton.LoadFromFile("./resources/Marchand/validation_box_button.png");
    sprValidationBoxButton.SetImage(imgValidationBoxButton);
    sprValidationBoxButton.SetY(sprValidationBox.GetPosition().y+3*sprValidationBox.GetSize().y/4-sprValidationBoxButton.GetSize().y/2);

    imgValidationBoxCursor.LoadFromFile("./resources/Marchand/validation_box_cursor.png");
    sprValidationBoxCursor.SetImage(imgValidationBoxCursor);

    imgBuySellBox.LoadFromFile("./resources/Marchand/buy_sell_box.png");
    sprBuySellBox.SetImage(imgBuySellBox);
    sprBuySellBox.SetPosition(FENETREY/4, FENETREY/4);

    imgBuySellBoxCursor.LoadFromFile("./resources/Marchand/buy_sell_box_cursor.png");
    sprBuySellBoxCursor.SetImage(imgBuySellBoxCursor);
    sprBuySellBoxCursor.SetX(sprBuySellBox.GetPosition().x+4);
}

////////////////////////////////////////////////////////////
/// Load the strings used by merchants
////////////////////////////////////////////////////////////
void Marchand::InitStrings()
{
    int size = 19;

    strDescription.SetText("Bienvenue dans notre humble commerce. Que voulez-vous faire ?");
    strDescription.SetSize(size);

    strItemsTitle.SetSize(size);
    strItemsTitle.SetY(sprItemsBox.GetPosition().y+10);

    strValidationYes.SetText("Oui");
    strValidationYes.SetSize(size);
    strValidationYes.SetPosition(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/4-strValidationYes.GetRect().GetWidth()/2, sprValidationBox.GetPosition().y+3*sprValidationBox.GetSize().y/4-strValidationYes.GetRect().GetHeight()/2);

    strValidationNo.SetText("No");
    strValidationNo.SetSize(size);
    strValidationNo.SetPosition(sprValidationBox.GetPosition().x+3*sprValidationBox.GetSize().x/4-strValidationNo.GetRect().GetWidth()/2, sprValidationBox.GetPosition().y+3*sprValidationBox.GetSize().y/4-strValidationNo.GetRect().GetHeight()/2);

    strValidationOk.SetText("Ok");
    strValidationOk.SetSize(size);
    strValidationOk.SetPosition(sprValidationBox.GetPosition().x+sprValidationBox.GetSize().x/2-strValidationOk.GetRect().GetWidth()/2, sprValidationBox.GetPosition().y+3*sprValidationBox.GetSize().y/4-strValidationOk.GetRect().GetHeight()/2);

    strMyMoneyTitle.SetText("Votre argent : ");
    strMyMoneyTitle.SetSize(size);
    strMyMoneyTitle.SetPosition(sprCompetencesBox.GetPosition().x+10, sprMyMoneyBox.GetPosition().y+sprMyMoneyBox.GetSize().y/2-strMyMoneyTitle.GetRect().GetHeight()/2);

    strMyMoney.SetSize(size);
    strMyMoney.SetPosition(strMyMoneyTitle.GetPosition().x+strMyMoneyTitle.GetRect().GetWidth(), strMyMoneyTitle.GetPosition().y);

    strMerchantMoneyTitle.SetText("Argent du marchand : ");
    strMerchantMoneyTitle.SetSize(size);
    strMerchantMoneyTitle.SetPosition(10, sprMerchantMoneyBox.GetPosition().y+sprMerchantMoneyBox.GetSize().y/2-strMerchantMoneyTitle.GetRect().GetHeight()/2);

    strMerchantMoney.SetSize(size);
    strMerchantMoney.SetPosition(strMerchantMoneyTitle.GetPosition().x+strMerchantMoneyTitle.GetRect().GetWidth(), strMerchantMoneyTitle.GetPosition().y);

    strBuy.SetText("Acheter");
    strBuy.SetSize(size);
    strBuy.SetPosition(sprBuySellBox.GetPosition().x+sprBuySellBox.GetSize().x/2-strBuy.GetRect().GetWidth()/2, sprBuySellBox.GetPosition().y+10);

    strSell.SetText("Vendre");
    strSell.SetSize(size);
    strBuy.SetPosition(sprBuySellBox.GetPosition().x+sprBuySellBox.GetSize().x/2-strSell.GetRect().GetWidth()/2, sprBuySellBox.GetPosition().y+15+size);

    strQuit.SetText("Quitter");
    strQuit.SetSize(size);
    strBuy.SetPosition(sprBuySellBox.GetPosition().x+sprBuySellBox.GetSize().x/2-strQuit.GetRect().GetWidth()/2, sprBuySellBox.GetPosition().y+10+(5+size)*2);

    strCompetencesTitle.SetText("Compétences");
    strCompetencesTitle.SetSize(size);
    strCompetencesTitle.SetPosition(sprCompetencesBox.GetPosition().x+sprCompetencesBox.GetSize().x/2-strCompetencesTitle.GetRect().GetWidth()/2, sprCompetencesBox.GetPosition().y+10);

    strCompetenceName.SetSize(size);
    strCompetenceName.SetX(sprCompetencesBox.GetPosition().x+20);

    strItemName.SetSize(size);
    strItemName.SetX(sprItemsBox.GetPosition().x+10);

    strItemPrice.SetSize(size);
    strItemPrice.SetX(sprItemsBox.GetPosition().x+sprItemsBox.GetSize().x-50);
}

////////////////////////////////////////////////////////////
/// Load the competences modified by items
////////////////////////////////////////////////////////////
void Marchand::InitComps()
{
    comp c;

    c.type = ATTAQUE;
    c.name = "Attaque";
    competences.push_back(c);

    c.type = DEFENSE;
    c.name = "Défense";
    competences.push_back(c);

    c.type = VITESSE;
    c.name = "Vitesse";
    competences.push_back(c);

    c.type = ENDURANCE;
    c.name = "Endurance";
    competences.push_back(c);

    c.type = INTELLIGENCE;
    c.name = "Intelligence";
    competences.push_back(c);
}

////////////////////////////////////////////////////////////
/// Load the different merchant items
////////////////////////////////////////////////////////////
void Marchand::InitMerchantItems()
{
    for (int i = 0 ; i < M_TAILLE ; ++i)
    {
        merchant_obj o;
        o.show = true;
        o.type = i;
        switch (i)
        {
            case M_POTION_VIE:
                o.name = "Potion de Vie";
                o.description = "Cette Potion de Vie régénère vos HP.";
                // TODO : Number and price have to be loaded from a file
                break;
            case M_POTION_MANA:
                o.name = "Potion de Mana";
                o.description = "Cette Potion de Mana restaure vos points de magie.";
                // TODO : Number and price have to be loaded from a file
                break;
            case M_SAUVEGARDES:
                o.name = "Sauvegardeur";
                o.description = "Cet objet vous permet de sauvegarder votre partie à tout moment du jeu.";
                // TODO : Number and price have to be loaded from a file
                break;
            default:
                cout << "Bug : Vous essayez de charger un objet non conforme !" << endl;
        }

        merchant_objects.push_back(o);
        // This vector should have the same size as the hero's one
    }
}
