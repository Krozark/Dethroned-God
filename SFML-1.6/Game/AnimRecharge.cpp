#include "AnimRecharge.hpp"

using namespace std;
using namespace sf;

AnimRecharge::AnimRecharge(Image &img,int tailleImgX,int temps)
{
    preloadAnim(img,tailleImgX);
    setTime(temps);
}

void AnimRecharge::setTime(int temps)
{
    SetFrameTime(temps/(float)Size());
}

void AnimRecharge::run(float time)
{
    if (IsPaused())
        Play();
    anim(time);
}

void AnimRecharge::preloadAnim(Image &img,int x)
{
    spr.SetImage(img);
    loadAnim(img,x);
}

void AnimRecharge::loadAnim(Image &image,int x)
{
    int perso_image_taille_x=(image.GetWidth())/x;
    int perso_image_taille_y=(image.GetHeight());

    for (int i=1;i<=x;++i)
        animGame.PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1), 0, perso_image_taille_x*i, perso_image_taille_y)));

    SetAnim(&animGame);
    SetLoop(false);
    Stop();
}
