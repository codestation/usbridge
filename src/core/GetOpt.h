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

#ifndef GETOPT_H_
#define GETOPT_H_

#include <unistd.h>

class GetOpt {
private:
	int c;
	char **v;
	const char *o;
public:
	GetOpt(int argc, char **argv, const char *optstring) : c(argc), v(argv), o(optstring) {}
	int operator()() { return getopt (c, v, o); }
	int opt() { return optopt; }
	const char *arg() { return optarg; }
};

#endif /* GETOPT_H_ */
