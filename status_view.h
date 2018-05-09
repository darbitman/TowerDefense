#ifndef STATUS_VIEW_H
#define STATUS_VIEW_H
#pragma once

class GameState;

#include "view.h"
#include "bounding_box_helper.h"
#include "InputDeviceHandler.h"

const double kStatusViewWidth = 73;
const double kStatusViewHeight = 15;
const double kStatusViewHeightBase = 84;
const double kStatusViewWidthBase = 4;

class StatusView :
	public View
{
	public:
		StatusView(GameState *);
		~StatusView();

		virtual void draw();
		virtual void draw(WORD);

		void clearView();
		void addMessage(string);

	private:
		void init(GameState *);

		// Disable default copy constructor and assignment operator.
		StatusView(const StatusView &);
		StatusView & operator=(const StatusView &);

		// Allow processMouseInput function access to mItemTupleVector.
		friend WORD InputDeviceHandler::processMouseInput(WORD);

	private:
		WORD							wWhite, wRed,
										wWhiteOnBlack,
										wBlackOnGray;

		int								top, bottom, nViewableMessages;
		static const std::size_t		kMaxItems = 100;
		GameState						*gs;
		BoundingBoxHelper				*bb;

		deque<std::string>				messages;
		deque <std::string>::iterator	it;
};


#endif