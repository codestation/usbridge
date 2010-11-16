/*
 * Adapter.cpp
 *
 *  Created on: 09/11/2010
 *      Author: code
 */

#include "Adapter.h"

bool Adapter::out(Adapter *in, bool wait) {
	if(open(wait)) {
		adp_in = in;
		return true;
	} else
		return false;
}

