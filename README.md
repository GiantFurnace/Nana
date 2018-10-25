# Logging Module Nana
---
## 1. Abstract

What nana means? find the answer if you really want to know about the "nana".<br/>
Nana is actually a lightweight logging module,she has her personal emotion, like the folowing:HAPPY,PEACE,SHOUT,ANGRY.<br/>
Nana's Emotions equivalent to the logging level:DEBUG, INFO ,WARN, ERROR.<br/>  

## 2. Understand the emotion of Nana

Make the logging personate.<br/> 
Emotion of nana is the logging level like the common logging module

### Logging Level Map
HAPPY->DEBUG<br/>
PEACE->INFO<br/>
SHOUT->WARN<br/>
ANGRY->ERROR<br/>

## 3. Sample Code
-----------

``` 
#include "nana.h"
using logging::Nana;

/*
 * @note:break time means cache logging data in memory
 */
static const int BREAK_TIME = 5; // 5 seconds

/*
 * @note:life length indicates the size of log file, when exceed the size of log file,
 * nana will generates a new
 */
static const int LIFE_LENGTH = 1024*1024*10;

/*
* Usefull macros for logging
* #define _DEBUG (Nana::HAPPY)
* #define _INFO  (Nana::PEACE)
* #define _WARN  (Nana::SHOUT)
* #define _ERROR (Nana::ANGRY)
* #define _LOG_OUTPUT(LEVEL,FORMAT,MESSAGE,...);\
* nana->say(LEVEL, __func__, __FILE__, __LINE__, FORMAT, MESSAGE, ##__VA_ARGS__);
*/

int main( int argc, char ** argv )
{
    (void) argc;
    (void) argv;
    Nana * nana = Nana::born( "nana.log", Nana::HAPPY, BREAK_TIME, LIFE_LENGTH );
    
    /*
     * @note:you can call nana->asNormal() to output DEBUG,INFO,WARN,ERROR 
     * instead of HAPPY,PEACE,SHOUT,ANGRY;
     */
    
    //_LOG_OUTPUT(_DEBUG, "A girl named nana born in this world with %s emotion", "HAPPY");
    nana->say( Nana::HAPPY, __func__, __FILE__, __LINE__, 
               "A girl named nana born in this world with %s emotion", "HAPPY");
    nana->die();
    return 0;
}
  
```

## 4. Contact

|Author          | Email            | Wechat      |
| ---------------|:----------------:| -----------:|
| chenzhengqiang | 642346572@qq.com | 18819373904 |

**Notice:  Any comments and suggestions are welcomed**

## 5. License
[Apache License 2.0](./LICENSE)
