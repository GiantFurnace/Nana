/*
 * nana native API header
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

#ifndef _BACKER_NANA_H_
#define _BACKER_NANA_H_

#include <cstdio>
#include <string>
#include <time.h>


/*
* what nana means?
* find the answer if you really want to know about the "nana".
* nana is actually a lightweight logging module,she has her personal emotion, such as happy, compain and so on.
* emotons equivalent to the logging level:debug, info ,error.
*/

namespace logging
{
    class Nana
    {
    public:
	typedef FILE * Life;
	/*
        * @note:NanaEmotion defines the logging level
	*/
	enum NanaEmotion
	{
	    COMPLAIN = 0,
	    PEACE = 1,
	    HAPPY = 2,
	};	
	
	/*
	 * @note:LifeLength means the size of logging file
        */
	enum LifeLength
	{
	    KB = 1024,
	    MB = ((KB)*(KB)),
	};
	public:
	/*
	* @note:born interface returns the singleton of nana
	*/
	static Nana *born( const std::string & cradle, int emotion, int breakTime = 0, int lifeLength= 50*MB )
	{
	    /*
	    * @note:it's not necessary to use the double-check and mutex lock,
	    * construct nana in global.
	    */
	    if ( ! nana_ )
	    {
		nana_ = new Nana( cradle, emotion, breakTime, lifeLength );
	    }
	    return nana_;
	}

	/*
	 * @note:say interface means output the content to the logging file
	 */
	void say( int emotion, const char *toWho, int location, const char *about, ...);
	bool is( int emotion ) { return emotion <= emotion_; }

	/*
        * @note:nana dies,when she don't logging anything any more
    	*/
	void die();
                
	/*
         * @note:output as normal
         */
        void asNormal() { normal_ = true; }
    private:
	/*
         * @note:shutup interface means cache the content in memory,
         * you need to configure the break time when construct nana
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
