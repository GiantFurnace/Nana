/*
@filename:nana.cpp
@author:chenzhengqiang
@start date:2018-09-29 09:55:05
*/


#include "nana.h"
#include <cstring>
#include <pthread.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdexcept>

using std::runtime_error;
using logging::Nana;



namespace logging
{
	
    static pthread_mutex_t  GuardianAngel = PTHREAD_MUTEX_INITIALIZER;
    static const char * Emotions[]={"__COMPLAIN__", "__PEACE__", "__HAPPY__"};
    Nana* Nana :: nana_ = 0;

    Nana::Nana( const std::string & cradle, int emotion, int breakTime, int lifeLength )
    :cradle_( cradle ), emotion_( emotion ), breakTime_( breakTime ), lifeLength_( lifeLength )

    {
	    life_ = fopen( cradle_.c_str(), "w");
	    if ( life_ == 0 )
	    {
	        throw runtime_error("Failed to open logging file");
	    }

	    setbuf( life_, NULL );
	    memset( said_, 0, sizeof( said_ ) );
            shutupTime_ = time( NULL );
	    now_ = said_;
	    end_ = said_+sizeof(said_);
    }
	


    void Nana::die()
    {
        
        if ( life_ != 0 )
        {
               
               pthread_mutex_lock( &GuardianAngel );
               fprintf( life_, said_ );
	       fclose( life_ );
               pthread_mutex_unlock( &GuardianAngel );
        } 

        if ( nana_ != 0 )
        { 
           pthread_mutex_lock( &GuardianAngel );
           if ( nana_ != 0 )
           { 
               delete nana_;
               nana_ = 0;
           }
           pthread_mutex_unlock( &GuardianAngel );
        }
    }
    


    unsigned long Nana::lifeLength()
    {
	    if ( cradle_.empty() )
	    return 0;
		
	    struct stat lifeStat; 
	    if (stat(cradle_.c_str(), &lifeStat) < 0)
	    {    
               return 0;    
	    }
	    return lifeStat.st_size;
    }



    bool Nana::reborn( void )
    {
	    //the time format:"YYYY-MM-DD HH:MM:SS" 
	    char curTime[24];
	    time_t now;
	    struct tm *localTime;
	    now = time( NULL );
	    localTime = localtime(&now);
	    strftime( curTime, sizeof(curTime), "%Y%m%d%H%M%S", localTime );
	    std::string oldCradle=cradle_+"."+curTime;
	    rename(cradle_.c_str(), oldCradle.c_str());
	    fclose(life_);

	    life_ = fopen(cradle_.c_str(), "w");
	    if (life_ == 0)
	    {
            return false;
	    }

	    setbuf(life_, NULL);
	    return true;
    }



    void Nana::say( int emotion,  const char * toWho, const char *about, ... )
    {
	    if ( emotion <= emotion_ )
	    {
	        va_list valist;
	        va_start(valist, about);
	        char timeBuffer[60]={0};
	        time_t now;
	        struct tm *loctime;
                
	        pthread_mutex_lock( &GuardianAngel );
	        if ( lifeLength() > lifeLength_ )
	        {
		        if ( reborn() == false )
		        {
			        pthread_mutex_unlock(&GuardianAngel);
			        return;
		       }
	        }
    			
	        now = time ( NULL );
	        loctime = localtime(&now);
	        char speak[1024]={0};
	        strftime ( timeBuffer, sizeof(timeBuffer), "%F %T : ", loctime );
	        int said = snprintf( speak, sizeof(speak),"%s %-15s :%-8s: ", timeBuffer, toWho, Emotions[emotion] );
	        said += vsnprintf( speak+said, sizeof(speak)-said, about, valist );
	        char *prev = now_;
    			
	        if ( (now_+said+1) < (end_) )
	        {
		        memcpy( now_, speak, said );
		        now_ += said;
		        *now_ = '\n';
		        now_ += 1;
	        }

	        //nana needs a break time , just shutup nana's speaking
	        shutup();

	        if ( (prev+said+1) >= (end_) )
	        {
		        memcpy(now_, speak, said);
		        now_ += said;
		        *now_ = '\n';
		        now_ += 1;
	        }
    			
	        pthread_mutex_unlock( &GuardianAngel );
	        va_end(valist);
	    }
    }



    void Nana::shutup()
    {
	    if( breakTime_ <= 0 )
	    {
	        if ( now_ > said_ )
	        {
		        fprintf( life_, said_ );
		        now_ = said_;
		        memset( said_, 0, 4*KB*9 );
	        }
	    }
	    else
	    {
	         time_t currentTime = time(NULL);
	         if ( ((now_ - said_ ) >= (8*KB)) || ( currentTime-shutupTime_ ) >=(time_t) breakTime_ )
	         {
		          fprintf( life_, said_ );
		          now_ = said_;
		          memset(said_, 0, 4*KB*9);
	         }
	         shutupTime_ = currentTime;
	    }
    }
};
