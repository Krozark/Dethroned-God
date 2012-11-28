#ifndef DG_MARCHAND_HPP
#define DG_MARCHAND_HPP

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Taille_fenetre.hpp"
#include "Global_taille_fenetre.hpp"
#include "Hero.hpp"

class Marchand
{
    public:

        ////////////////////////////////////////////////////////////
        /// Default constructor
        ///
        /// \param app : Window in which the string will be displayed
        /// \param touches_Sys : Define the navigation keys
        /// \param h : Hero who bargains
        ///
        ////////////////////////////////////////////////////////////
        Marchand(sf::RenderWindow *app, _touches_sys* touches_Sys, Hero *h);

        ////////////////////////////////////////////////////////////
        /// Events gestion
        ///
        /// \param event : Index of caught event
        ///
        ////////////////////////////////////////////////////////////
        void getEvents(sf::Event event);

        ////////////////////////////////////////////////////////////
        /// Draw on the screen the different boxes
        ///
        ////////////////////////////////////////////////////////////
        void Draw();

        ////////////////////////////////////////////////////////////
        // Merchant states
        ////////////////////////////////////////////////////////////
        enum buy_sell_enum
        {
            STATE_QUESTION = 0,
            STATE_BUY,
            STATE_SELL
        } state_buy_sell;

        enum action_enum
        {
            STATE_CHOOSE = 0,
            STATE_VALIDATION,
            STATE_WARNING
        } state_action;

    private:

        ////////////////////////////////////////////////////////////
        /// Update the items possesed by the hero
        ///
        ////////////////////////////////////////////////////////////
        void UpdateItems();

        ////////////////////////////////////////////////////////////
        /// Load the different merchant items
        ///
        ////////////////////////////////////////////////////////////
        inline void InitMerchantItems();

        ////////////////////////////////////////////////////////////
        /// Load the images used by merchants
        ///
        ////////////////////////////////////////////////////////////
        inline void InitImages();

        ////////////////////////////////////////////////////////////
        /// Load the strings used by merchants
        ///
        ////////////////////////////////////////////////////////////
        inline void InitStrings();

        ////////////////////////////////////////////////////////////
        /// Load the competences modified by items
        ///
        ////////////////////////////////////////////////////////////
        inline void InitComps();

        ////////////////////////////////////////////////////////////
        /// Inline functions for the events
        ///
        ////////////////////////////////////////////////////////////
        inline void keyRight();
        inline void keyLeft();
        inline void keyUp();
        inline void keyDown();
        inline void keyEnter();
        inline void keyReturn();

        ////////////////////////////////////////////////////////////
        /// Show a warning window with the "Ok" button
        ///
        /// \param s : string to display
        ///
        ////////////////////////////////////////////////////////////
        inline void Warning(std::string s);

        ////////////////////////////////////////////////////////////
        /// Show a validation window with the "Yes" and "No" buttons
        ///
        /// \param s : string to display
        ///
        ////////////////////////////////////////////////////////////
        inline void Validation(std::string s);

        ////////////////////////////////////////////////////////////
        /// Called when we leave the merchant
        ///
        ////////////////////////////////////////////////////////////
        void Quit();

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        enum type_merchant_items
        {
            M_POTION_VIE = 0,
            M_POTION_MANA,
            M_SAUVEGARDES,
            M_TAILLE
        };

        struct merchant_obj
        {
            std::string name;
            std::string description;
            int type;
            int price;
            int number;
            bool show;
        };

        std::vector<struct merchant_obj>    merchant_objects,
                                            hero_objects,
                                            *current_objects,
                                            *other_objects;

        struct comp
        {
            std::string name;
            STATS type;
        };
        std::vector<struct comp> competences;

        _touches_sys*       touchesSys;
        Hero*               heros;
        sf::RenderWindow*   App;

        sf::Image       imgMerchantHeadBox,
                        imgDescriptionBox,
                        imgItemsBox,
                        imgItemsBoxCursor,
                        imgMoneyBox,
                        imgCompetencesBox,
                        imgValidationBox,
                        imgValidationBoxButton,
                        imgValidationBoxCursor,
                        imgBuySellBox,
                        imgBuySellBoxCursor;

        sf::Sprite      sprMerchantHeadBox,
                        sprDescriptionBox,
                        sprItemsBox,
                        sprItemsBoxCursor,
                        sprMyMoneyBox,
                        sprMerchantMoneyBox,
                        sprCompetencesBox,
                        sprValidationBox,
                        sprValidationBoxButton,
                        sprValidationBoxCursor,
                        sprBuySellBox,
                        sprBuySellBoxCursor;

        sf::String      strDescription,
                        strValidation,
                        strValidationYes,
                        strValidationNo,
                        strValidationOk,
                        strMyMoneyTitle,
                        strMyMoney,
                        strMerchantMoneyTitle,
                        strMerchantMoney,
                        strItemsTitle,
                        strItemName,
                        strItemPrice,
                        strCompetencesTitle,
                        strCompetenceName,
                        strBuy,
                        strSell,
                        strQuit;

        int     money,
                buy_sell_cursor,
                items_cursor,
                items_number,
                items_tobuysell_number;

        bool    choice_cursor,
                must_update;
};

#endif // DG_MARCHAND_HPP
