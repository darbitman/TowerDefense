#include "ViewManager.h"

ViewManager::ViewManager()
{
  views.resize(0);
}

ViewManager::~ViewManager()
{

}

void ViewManager::push_back(View* v)
{
	if(typeid(*v) == typeid(StatusView))
	{
		ViewConstIter cit = findView(v);
	
		// If ViewManager is in the vector.
		if(cit != views.end())
		{
			delete v;
			return;
		}
	}

	views.push_back(v);
}

void ViewManager::pop_back()
{
	if(!views.empty())
	{
		View* v = views.back();
		views.pop_back();
		delete v;
	}
}

// Removes all view objects from the vector.
void ViewManager::clear()
{
	for(int i = views.size() - 1; i >= 0; --i)
	{
		// We make an exception here not to remove StatusView
		// object from the vector.
		if(typeid(*views[i]) == typeid(StatusView))
			continue;

		View * v = views[i];
		views.pop_back();
		delete v;
	}
}

// Remove a single view object from the vector.
void ViewManager::erase ( View * v )
{
	// Set initial position of iterator.
	ViewIter it ( views.begin ( ) );

	// Attempt to find View object from the vector.
	// Returned type is a constant iterator.
	ViewConstIter cit = findView ( v );

	// Safely convert a constant iterator to a r/w iterator.
	advance ( it, distance <ViewConstIter> ( it, cit ) );

	// If compared view types are equal, remove from vector.
	// Don't forget to delete parameter View v object.
	if ( it != views.end ( ) )
	{
		View * view = *it;
		views.erase ( it );

		delete view;
		delete v;
	}

}

// Draw a single view.
void ViewManager::drawSingleView ( View * v ) const
{
	// Attempt to find View object from the vector.
	// Returned type is a constant iterator.
	ViewConstIter cit = findView ( v );
	
	if ( cit != views.end ( ) )
		( *cit )->draw ( ) ;

}

// Find the View object in the vector.
// Returned type is constant iterator.
ViewConstIter ViewManager::findView ( View * v ) const
{
	// Declare explicitly so that a compiler control path
	// return value warning message is avoided.
	ViewConstIter cit;

	if ( v != NULL )
	{
		cit = std::find_if ( views.begin ( ), 
						views.end ( ),
						std::bind2nd <ViewTypePred> ( ViewTypePred ( ), v ) ); 
	}

	return cit;

}

bool ViewManager::match(View * v) const
{
	for(int i = 0; i < views.size(); ++i)
	{
		// We make an exception here not to remove StatusView
		// object from the vector.
		if(typeid(*views[i]) == typeid(StatusView))
			return true;
	}

	return false;
}

void ViewManager::drawViews()
{
  for (uint32 i = 0; i < views.size(); i++)
    views[i]->draw();
};