/*
@author:chenzhengqiang
@date:2018-09-29
@email:642346572@qq.com
*/


#include "nana.h"
using logging::Nana;

/*
 * @note:break time means cache logging data in memory
 */
static const int BREAK_TIME = 5; // 5 seconds

/*
 * @note:life length indicates the size of log file, when exceed the size of log file,
   nana will generates a new
 */
static const int LIFE_LENGTH = 1024*1024*10;

int main( int argc, char ** argv )
{
    (void) argc;
    (void) argv;
    Nana * nana = Nana::born( "nana.log", Nana::HAPPY, BREAK_TIME, LIFE_LENGTH );
    nana->say( Nana::HAPPY, __func__, __LINE__, "A girl named nana born in this world with %s emotion", "HAPPY");
    nana->die();
    return 0;
}
