#include "Floor.hpp"

#include <math.h>

using namespace sf;
using namespace std;

bool Floor::CouvrePosition(Vector2f pos)
{
    bool res=false;
    Vector2f mypos=GetPosition();

    if (GetRotation()== 0)
    {
        Vector2f myposend=Vector2f(mypos.x+m_size,mypos.y+TAILLEBOC);

        res= (pos.x >= mypos.x) && (pos.x <=myposend.x)&& (pos.y>=mypos.y) && (pos.y<=myposend.y);
    }
    else
        cout<<"Angle de rotation non traiter dans Floor::CouvrePosition(float)"<<endl;

    //float angle =GetRotation() * M_PI / 180;

    return res;
}
