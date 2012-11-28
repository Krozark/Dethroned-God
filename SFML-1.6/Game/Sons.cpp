#include "Sons.hpp"

using namespace std;
using namespace sf;

Sons::Sons(_options* settings)
{
    this->settings = settings;
    initSons();
}

Sons::Sons()
{
    settings = NULL;
    initSons();
}


void Sons::initSons()
{
    tab_buffers.resize(2*TAILLE_SON);
    tab_sons.resize(2*TAILLE_SON);
    tab_buffers.clear();
    tab_sons.clear();

    //potions
    loadSon(string("./sons/ramasse.ogg"));
    loadSon(string("./sons/utilPopo.ogg"));
    loadSon(string("./sons/bouger.ogg"));
    loadSon(string("./sons/valid.ogg"));
    loadSon(string("./sons/GameOver.ogg"));
    tab_sons[tab_sons.size()-1].SetVolume(20);
    loadSon(string("./sons/utilParchemin.ogg"));
    loadSon(string("./sons/lvlup.ogg"));
    loadSon(string("./sons/upcarac.ogg"));
    loadSon(string("./sons/downcarac.ogg"));
    loadSon(string("./sons/tape/air1.ogg"));
    loadSon(string("./sons/tape/air2.ogg"));
    loadSon(string("./sons/tape/sword1.ogg"));
    loadSon(string("./sons/tape/sword2.ogg"));
    loadSon(string("./sons/tape/sword3.ogg"));
    loadSon(string("./sons/tape/sword4.ogg"));
    loadSon(string("./sons/bossTue.ogg"));
    loadSon(string("./sons/miss.ogg"));
    loadSon(string("./sons/epicFail.ogg"));
    loadSon(string("./sons/Save.ogg"));
    loadSon(string("./sons/teleport.ogg"));
    loadSon(string("./sons/teleportArrived.ogg"));
    loadSon(string("./sons/or.ogg"));
    loadSon(string("./sons/trempoline.ogg"));
    loadSon(string("./sons/accelerateur.ogg"));
    loadSon(string("./sons/trap.ogg"));
    loadSon(string("./sons/error.ogg"));
    loadSon(string("./sons/respawn.ogg"));
    loadSon(string("./sons/milathea.ogg"));
}

inline void Sons::loadSon(string path)
{
    buffer.LoadFromFile(path);
    tab_buffers.push_back(buffer);
    son.SetBuffer(tab_buffers[tab_buffers.size()-1]);
    son.SetVolume((settings==NULL)?50:settings->volumeSons);
    son.SetRelativeToListener(true);
    tab_sons.push_back(son);
}


void Sons::playSon(int son)
{
    tab_sons[son].Play();
}


void Sons::updateVolumeSons(_options *settigns)
{
    for (unsigned int i=0; i<tab_sons.size(); ++i)
        tab_sons[i].SetVolume(settings->volumeSons);
}


