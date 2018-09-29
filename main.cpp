/*
@author:chenzhengqiang
@date:2018-09-29
@email:642346572@qq.com
*/


#include "nana.h"
using logging::Nana;

static const int BREAK_TIME = 5;
static const int LIFE_LENGTH = 1024*1024*10;
int main( int argc, char ** argv )
{
    Nana * nana = Nana::born( "nana.log", Nana::HAPPY, BREAK_TIME, LIFE_LENGTH );
    nana->say( Nana::HAPPY, __func__, "A girl named nana born in this world with %d year(s) old", 1);
    nana->die();
    return 0;
}
