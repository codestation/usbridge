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

#include "List.h"

List::List(COMPARE_FUNC cfunc, DELETE_FUNC dfunc, COPY_FUNC cpfunc) {
	head = 0;
	tail = &head;
	iter = 0;
	counter = 0;
	comp = cfunc;
	del = dfunc;
	cpy = cpfunc;
	pthread_mutex_init(&lock, NULL);
}

List::node *List::create(void *obj) {
	node *ret = new node;
	ret->obj = obj;
	ret->next = 0;
	ret->counter = 1;
	return ret;
}

void List::add(void *obj) {
	node *curr = create(obj);
	pthread_mutex_lock(&lock);
	*tail = curr;
	tail = &((*tail)->next);
	iter = head;
	counter++;
	pthread_mutex_unlock(&lock);
}

void *List::get(void *obj) {
	node *curr = head;
	while(curr) {
		if(!comp(curr->obj, obj))
			return curr->obj;
		curr = curr->next;
	}
	return 0;
}
/*
void *List::pop() {
	if(head) {
		node *curr = head;
		node *prev = head;
		while(curr) {
			if(curr == tail) {
				prev->next = curr->next;
				if(head == curr)
					head = curr->next;
				if(tail == curr)
					tail = prev->next;
				curr->next = 0;
				iter = head;
				counter--;
				return curr;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return 0;
}
*/
void *List::getByIndex(int n) {
	node *curr = head;
	while(n-- && curr)
		curr = curr->next;
	return curr ? curr->obj : 0;
}

bool List::exist(void *obj) {
	return get(obj) != 0;
}

bool List::remove(void *obj) {
	pthread_mutex_lock(&lock);
	if(head) {
		node *curr = head;
		node *prev = head;
		while(curr) {
			if(!comp(curr->obj, obj)) {
				prev->next = curr->next;
				if(head == curr)
					head = curr->next;
				if(*tail == curr)
					*tail = prev->next;
				if(del)
					del(curr->obj);
				delete curr;
				iter = head;
				counter--;
				pthread_mutex_unlock(&lock);
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	pthread_mutex_unlock(&lock);
	return false;
}

List *List::copy(List *l) {
	if(!cpy)
		return 0;
	List *ret = new List(comp, del, cpy);
	node *curr = head;
	while(curr) {
		node *tmp = new node;
		tmp->obj = cpy(curr->obj);
		tmp->next = 0;
		ret->add(tmp);
		curr = curr->next;
	}
	return ret;
}

int List::count() {
	return counter;
}


void List::clear() {
	while(head) {
		node *next = head->next;
		if(del)
			del(head->obj);
		delete head;
		head = next;
	}
	iter = head;
	tail = &head;
	counter = 0;
}

void List::rewind() {
	iter = head;
}

bool List::empty() {
	return counter == 0;
}

void *List::next() {
	node *tmp = iter;
	if(iter)
		iter = iter->next;
	return tmp->obj;
}

bool List::hasNext() {
	return iter != 0;
}


List::~List() {
	clear();
	pthread_mutex_destroy(&lock);
}
