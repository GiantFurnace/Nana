/*
 * nana implementation file
 *
 * Copyright (c) 2018-2028 chenzhengqiang 642346572@qq.com 
 * All rights reserved since 2018-10-04
 *
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 *
 *   1.  Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MER-
 * CHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPE-
 * CIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTH-
 * ERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License ("GPL") version 2 or any later version,
 * in which case the provisions of the GPL are applicable instead of
 * the above. If you wish to allow the use of your version of this file
 * only under the terms of the GPL and not to allow others to use your
 * version of this file under the BSD license, indicate your decision
 * by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL. If you do not delete the
 * provisions above, a recipient may use your version of this file under
 * either the BSD or the GPL.
 */

#include "nana.h"
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <sys/stat.h>
#include <stdarg.h>
#include <errno.h>
extern int errno;

using logging::Nana;

namespace logging
{
    static pthread_mutex_t  GuardianAngel = PTHREAD_MUTEX_INITIALIZER;
    static const char * Emotions[]={"__COMPLAIN__", "__PEACE__", "__HAPPY__"};
    static const char * Normals[]={"__ERROR__", "__INFO__", "__DEBUG__"};
    Nana* Nana :: nana_ = 0;

    Nana::Nana( const std::string & cradle, int emotion, int breakTime, int life )
    :cradle_( cradle ), emotion_( emotion ), breakTime_( breakTime ), lifeLength_( life ),normal_(false)

    {
        life_ = fopen( cradle_.c_str(), "w");
        if ( life_ == 0 )
	{		 
	    std::cerr<<"failed to open log file<"<<cradle_<<"> error info:"<<(strerror(errno))<<std::endl;
	    exit(EXIT_FAILURE);
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

    void Nana::say( int emotion,  const char * toWho, const char * where, int location, const char *about, ... )
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
	    strftime ( timeBuffer, sizeof(timeBuffer), "%F %T ", loctime );
	    int said;
            if ( ! normal_ )
            {
                said = snprintf( speak, sizeof(speak),"%s%-9s [%s@%s:%d]: ", timeBuffer, Emotions[emotion], toWho, where, location);
            }
            else
            {
                said = snprintf( speak, sizeof(speak),"%s%-9s [%s@%s:%d]: ", timeBuffer, Normals[emotion], toWho, where, location );
            }
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
