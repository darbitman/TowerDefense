#ifndef HELPVIEW_H
#define HELPVIEW_H

class GameState;

#include "view.h"
#include "bounding_box_helper.h"
#include "Logger\Logger.h"
#include "InputDeviceHandler.h"

class HelpView : public View
{
	public:
		HelpView();
		HelpView(GameState * gs);
		~HelpView();

		virtual void draw();
		virtual void draw(WORD);

	private:
		void init(GameState * gs);

		// Disable default copy constructor and assignment operator.
		HelpView(const HelpView &);
		HelpView & operator=(const HelpView &);

	private:
		GameState			* gs;
		BoundingBoxHelper	* bb;

		WORD				wRed, wWhite,		// Holds possible colors for the current string.	
							wWhiteOnBlack,
							wBlackOnGray;

		std::string			str;
};

#endif
