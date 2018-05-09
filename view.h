#ifndef VIEW_H
#define VIEW_H

#pragma once

#include "Config.h"

class View
{
	public:
		View() {}
		virtual void draw() = 0;
		virtual void draw(WORD) = 0;
		virtual ~View() {}  // This will prevent memory leaks since descendant class destructors are actually calling this base class's destructor.

	// These base class properties are common to all
	// descendant classes except for bounding box class.
	protected:
		SHORT base;
		SHORT width;
		SHORT height;
		SHORT baseX;
		SHORT baseY;
		COORD cursor;
		ItemTupleVector mItemTupleVector;
		ItemTupleVectorConstIter mItemTupleVectorConstIter;
		ItemTupleVectorIter mItemTupleVectorIter;

};

#endif