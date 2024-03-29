/*
    Ming, an SWF output library
    Copyright (C) 2001  Opaque Industries - http://www.opaque.net/

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

/* blocklist.h
 *
 * $Id$
 *
 * Notice: This header file contains declarations of functions and types that
 * are just used internally. All library functions and types that are supposed
 * to be publicly accessable are defined in ./src/ming.h.
 */

#ifndef SWF_BLOCKLIST_H_INCLUDED
#define SWF_BLOCKLIST_H_INCLUDED

#include "ming.h"


typedef struct SWFBlockList_s *SWFBlockList;

SWFBlockList newSWFBlockList(void);
void destroySWFBlockList(SWFBlockList list);

void SWFBlockList_addBlock(SWFBlockList blocklist, SWFBlock block);

SWFBlock SWFBlockList_getLastBlock(SWFBlockList blocklist);

void SWFBlockList_addToSprite(SWFBlockList list, SWFSprite sprite);

int SWFBlockList_completeBlocks(SWFBlockList list, int version);

int SWFBlockList_writeBlocksToMethod(SWFBlockList list,
				     SWFByteOutputMethod method, void *data);

void SWFBlockList_resolveCharacterDependencies(SWFBlockList list,
					       SWFCharacter character);

#endif /* SWF_BLOCKLIST_H_INCLUDED */
