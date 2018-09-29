# Logging Module Nana
---
## 1. Abstract--
make every everything personate.<br/> 
what nana means?<br/>
find the answer if you really want to know about the "nana".<br/>
Nana is actually a lightweight logging module,she has her personal emotion, as the folowing:HAPPY,PEACE, COMPLAIN.<br/>
Nana's Emotons equivalent to the logging level:DEBUG, INFO ,ERROR.<br/>  

## 2. Sample Code
-----------

``` 
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
  
```

## 3. Contact

|Author          | Email            | Wechat      |
| ---------------|:----------------:| -----------:|
| chenzhengqiang | 642346572@qq.com | 18819373904 |

**Notice:  Any comments and suggestions are welcomed**

## 4. License
[Apache License 2.0](./LICENSE)
