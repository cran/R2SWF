/*
    Ming, an SWF output library

    Copyright (C) 2005-2009  Sandro Santilli <strk@kbt.io>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef GC_H
#define GC_H 1

#include "ming_config.h"

/* typedef void *mem_node; */
typedef struct mem_node_t mem_node;

typedef void (*dtorfunctype)(void*);

mem_node *ming_gc_add_node(void *ptr, dtorfunctype dtor);
void ming_gc_remove_node(mem_node *node);

#endif
