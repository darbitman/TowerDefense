#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

class GameState;

#include "view.h"
#include "bounding_box_helper.h"
#include "Logger\Logger.h"
#include "InputDeviceHandler.h"

class AboutView : public View
{
	public:
		AboutView();
		AboutView(GameState * gs);
		~AboutView();

		virtual void draw();
		virtual void draw(WORD);

	private:
		void init(GameState * gs);

		// Disable default copy constructor and assignment operator.
		AboutView(const AboutView &);
		AboutView & operator=(const AboutView &);

	private:
		GameState			* gs;
		BoundingBoxHelper	* bb;

		WORD				wRed, wWhite,		// Holds possible colors for the current string.	
							wWhiteOnBlack,
							wBlackOnGray;

		std::string			str;

};

#endif