/*
@filename:nana.h
@author:chenzhengqiang
@date:2018-09-29
@email:642346572@qq.com
*/

#ifndef _BACKER_NANA_H_
#define _BACKER_NANA_H_

#include <cstdio>
#include <string>
#include <time.h>


/*
what nana means?
find the answer if you really want to know about the "nana".
nana is actually a lightweight logging module,she has her personal emotion, such as happy, compain and so on.
emotons equivalent to the logging level:debug, info ,error.
*/

namespace logging
{
	class Nana
	{
	public:
		typedef FILE * Life;
		/*
		    NanaEmotion defines the logging level
		*/
		enum NanaEmotion
		{
			COMPLAIN = 0,
			PEACE = 1,
			HAPPY = 2,
		};
		
		
		/*
	             LifeLength means the size of logging file
		*/
		enum LifeLength
		{
			KB = 1024,
			MB = ((KB)*(KB)),
		};

	public:
		/*
		    born interface returns the singleton of nana
		*/
		static Nana *born( const std::string & cradle, int emotion, int breakTime = 0, int lifeLength= 50*MB )
		{
			/*
			    it's not necessary to use the double-check and mutex lock,
			    construct nana in global.
			*/
			if ( ! nana_ )
			{
				nana_ = new Nana( cradle, emotion, breakTime, lifeLength );
			}
			return nana_;
		}

		/*
		    say interface means output the content to the logging file
		*/
		void say( int emotion, const char *toWho, int location, const char *about, ...);
		bool is( int emotion ) { return emotion <= emotion_; }

		/*
		    nana dies,when she don't logging anything any more
    	        */
		void die();
                /*
                  output as normal
                */
                void asNormal() { normal_ = true; }
	private:
		/*
		    shutup interface means cache the content in memory,
		    you need to configure the break time when construct nana
		*/
		void shutup();

	
	private:

		Nana( const std::string & cradle, int emotion, int breakTime, int lifeLength );
		Nana( const Nana & ) {}
		Nana & operator=( const Nana & ) { return *this; }
		~Nana() {;}
		bool reborn();
		unsigned long lifeLength();

	private:
		static Nana * nana_;
		std::string cradle_;
		int emotion_;
		Life life_;
		int breakTime_;
		unsigned long lifeLength_;
		time_t shutupTime_;
		char said_[4 * KB * 9];
		char *now_;
		char *end_;
                bool normal_;
	};
}
#endif
