/*
 * Adapter.h
 *
 *  Created on: 09/11/2010
 *      Author: code
 */

#ifndef ADAPTER_H_
#define ADAPTER_H_

#include <stddef.h>

class Adapter {
	typedef void (*callback_in) (char *data, int size);
	//must be called from derived class
	callback_in cb_in;
protected:
	Adapter *adp_in;
public:
	//must be implemented by derived class
	virtual int callback_out(const char *data, int size) = 0;
	virtual bool open(bool wait) = 0;
	virtual void close() = 0;
	bool out(Adapter *in, bool wait = false);

	//virtual ~Adapter();
};

#endif /* ADAPTER_H_ */
