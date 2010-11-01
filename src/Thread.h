/*
 *  Project Bahamut: full ad-hoc tunneling software to be used by the
 *  Playstation Portable (PSP) to emulate online features.
 *
 *  Copyright (C) 2008-2010  Codestation
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef THREAD_H_
#define THREAD_H_

#ifdef _WIN32
#define WINVER 0x0501 //Windows XP
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

class Thread {
private:
#ifdef _WIN32
	unsigned int th;
#else
	pthread_t th;
#endif

#ifdef _WIN32
	static unsigned __stdcall _run(void *);
#else
	static void *_run(void *);
#endif
protected:
	virtual int run();
public:
	Thread();
	void start();
	int detach();
	int startAndDetach();
	int wait();
	int startAndWait();
	virtual ~Thread();
};

#endif /* THREAD_H_ */
