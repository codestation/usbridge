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

#ifndef LIST_H_
#define LIST_H_
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
class List {
private:
	typedef int (*COMPARE_FUNC) (void *, void *);
	typedef void (*DELETE_FUNC) (void *);
	typedef void *(*COPY_FUNC) (void *);
	COMPARE_FUNC comp;
	DELETE_FUNC del;
	COPY_FUNC cpy;
	struct node {
		void *obj;
		node *next;
		int counter;
	} *head, **tail, *iter;
	int counter;
#ifndef _WIN32
	pthread_mutex_t lock;
#else
	void *lock;
	inline int pthread_mutex_init(void *,int *) { return 0; }
	inline int pthread_mutex_lock(void *) { return 0; }
	inline int pthread_mutex_unlock(void *) { return 0; }
	inline int pthread_mutex_destroy(void *) { return 0; }
#endif
	node *create(void *obj);
public:
	List(COMPARE_FUNC, DELETE_FUNC = 0, COPY_FUNC = 0);
	void add(void *item);
	void *get(void *);
	void *getByIndex(int i);
	bool exist(void *);
	bool remove(void *);
	void *pop();
	List *copy(List *l);
	int count();
	bool empty();
	void clear();
	void rewind();
	void *next();
	bool hasNext();
	virtual ~List();
};

#endif /*LIST_H_*/
