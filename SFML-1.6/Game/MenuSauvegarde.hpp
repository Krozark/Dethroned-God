#ifndef DG_SAVE_HPP
#define DG_SAVE_HPP

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define NB_SAVE_SLOTS       3

#include "Taille_fenetre.hpp"
#include "Sons.hpp"
#include "SaveStruct.hpp"
#include "Global_taille_fenetre.hpp"

class MenuSauvegarde
{
    public:

        ////////////////////////////////////////////////////////////
        /// Default constructor
        ///
        /// \param touches_Sys : Define the navigation keys
        /// \param so : Define the sounds used in the menu
        /// \param load : Load menu if true, save menu if false
        ///
        ////////////////////////////////////////////////////////////
        MenuSauvegarde(_touches_sys* touches_Sys, Sons* so, bool load = false);

        ////////////////////////////////////////////////////////////
        /// Events gestion
        ///
        /// \param event : Index of caught event
        ///
        ////////////////////////////////////////////////////////////
        void getEvents(sf::Event event);

        ////////////////////////////////////////////////////////////
        /// Draw on the screen the menu title and the save slots
        ///
        /// \param target : Application into which draw the menu
        ///
        ////////////////////////////////////////////////////////////
        void Draw(sf::RenderTarget& target);

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// Save the game data with the given index
        ///
        /// \param save : Structure which contains all the elements we have to save in a file
        /// \param touches : Structure which contains the keyboard configuration and the shortcuts
        /// \param settings : Structure which contains the main settings of the game
        ///
        ///////////////////////////////////////////////////////////////////////////////////////////////
        void SaveGame(_sauvegarde* save,_touches_heros* touches,_options* settings);

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// Load the game if there is one at the current slot
        ///
        /// \param save : Structure in which we have to put the saved game
        ///
        ///////////////////////////////////////////////////////////////////////////////////////////////
        void LoadGame(_sauvegarde* save, _touches_heros* touches);

        //Sate of the save menu
        enum    save_enum { STATE_NONE = 0, STATE_QUESTION, STATE_GAMESAVED } save_state;

        ///////////////////////////////////////////////////////////////
        /// Getters and setters
        ///////////////////////////////////////////////////////////////
        const bool getLoadGame() { return load_game; };
        const bool getSaveGame() { return save_game; };
        const bool getExit() { return exit; };
        void setLoadGame(bool val) { load_game = val; };
        void setSaveGame(bool val) { save_game = val; };
        void setExit(bool val) { exit = val; };

    private:

        ////////////////////////////////////////////////////////////
        /// Load the images used in the save menu
        ///
        ////////////////////////////////////////////////////////////
        void InitImages();

        ////////////////////////////////////////////////////////////
        /// Define the strings used in the save menu
        ///
        ////////////////////////////////////////////////////////////
        void InitStrings();

        ////////////////////////////////////////////////////////////
        /// Draw on the screen a message box
        ///
        /// \param target : Application into which draw the menu
        /// \param question : Yes/No buttons if true, Ok button if false
        ///
        ////////////////////////////////////////////////////////////
        void DrawMessageBox(sf::RenderTarget& target, bool question = true);

        ////////////////////////////////////////////////////////////
        /// Inline fonctions for the events
        ////////////////////////////////////////////////////////////
        inline void keyRightLeft();
        inline void keyUp();
        inline void keyDown();
        inline void keyReturn();

        ////////////////////////////////////////////////////////////
        /// Initialiez the slots's text
        ////////////////////////////////////////////////////////////
        inline void majSlotsText();
        inline void majOneSlot(int i,bool init=true);
        void majTimeGame(_sauvegarde save, int i);

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        sf::Image   imgSaveTitle,
                    imgSaveSlot,
                    imgSaveSlotCursor,
                    imgSaveConfirm,
                    imgSaveButton,
                    imgSaveButtonCursor;

        sf::Sprite  sprSaveTitle,
                    sprSaveSlot,
                    sprSaveSlotCursor,
                    sprSaveConfirm,
                    sprSaveButton,
                    sprSaveButtonCursor;

        sf::String  strSaveTitle,
                    strSaveSlotMythologie[NB_SAVE_SLOTS], //emplacement libre ou nom du niveau
                    strSaveSlotNomLevel[NB_SAVE_SLOTS],
                    strSaveSlotTypeLevel[NB_SAVE_SLOTS],
                    strSaveSlotTime[NB_SAVE_SLOTS],
                    strSaveConfirm,
                    strSaveYes,
                    strSaveNo,
                    strSaveOk;

        std::string texteEmplacementLibre;

        int     save_slot;

        bool    confirm_choice,
                is_load,
                load_game,
                save_game,
                exit;

        Sons *sons;
        _touches_sys* touchesSys;
};

#endif // DG_SAVE_HPP
