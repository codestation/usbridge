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

#include "Thread.h"

Thread::Thread() {
}

int Thread::run() {
	return 0;
}

#ifdef _WIN32
unsigned __stdcall Thread::_run(void *arg) {
	return ((Thread *)arg)->run();
}
#else
void *Thread::_run(void *arg) {
	return (void *)((Thread *)arg)->run();
}
#endif

void Thread::start() {
#ifdef _WIN32
	th = _beginthreadex(NULL, 4096, &_run, this, 0, NULL);
#else
	pthread_create(&th, NULL, &_run, this);
#endif
}

int Thread::startAndDetach() {
	start();
	return detach();
}

int Thread::startAndWait() {
	start();
	return wait();
}

int Thread::detach() {
#ifdef _WIN32
	return 0;
#else
	return pthread_detach(th);
#endif
}

int Thread::wait() {
#ifdef _WIN32
	return WaitForSingleObject((void *)th, INFINITE);
#else
	return pthread_join(th, NULL);
#endif
}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
}
