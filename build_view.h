#pragma once
#include "view.h"
#include "Config.h"

class BuildView :
	public View
{
	public:
		BuildView(void);
		virtual void draw();

	private:
		void draw_chars( int, char );
};