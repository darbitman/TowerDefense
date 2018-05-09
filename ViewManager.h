#ifndef ViewManager_h__
#define ViewManager_h__

// Forward declare view classes so new functions can reference them.
class BuildView;
class CreatureInfoView;
//class GameOverView;
//class InfoPaneView;
class MainMenuView;
class MapView;
class PlayerInfoView;
class TowerInfoView;

#include "Config.h"
#include "view.h"
#include "InputDeviceHandler.h"
#include "Logger\Logger.h"

typedef std::vector <View *> ViewVector;
typedef std::vector <View *>::const_iterator ViewConstIter;
typedef std::vector <View *>::iterator ViewIter;

class ViewManager
{
public:
  ViewManager();
  ~ViewManager();

  void push_back(View* v);
  void pop_back();

  // Removes all views from the vector.
  void clear();

  // removes an arbitrary view from the vector.
  void erase ( View * );

  // attempts to find a match of view.
  bool match(View *) const;
  
  uint32 size() { return views.size(); }
  void drawViews();

  // draws a single view to the std output.
  void drawSingleView ( View * ) const;

  bool isEmpty() const;


  // InputDeviceHandler class needs access to this class's view vector
  // for processing mouse events such as mouse clicks and mouse hover,
  // so make these processMouseInput member functions a friend of this class.
  friend WORD InputDeviceHandler::getItemClicked();
  friend WORD InputDeviceHandler::getItemHovered();
  friend WORD InputDeviceHandler::processMouseInput(WORD);

private:

	ViewConstIter findView ( View * ) const;

	// Comparator predicate for determining if two view subclasses are of the same type.
	// Inlined for efficiency.
	struct ViewTypePred : public std::binary_function <View *, View *, bool>
	{
		bool operator ( ) ( const View * lhs, const View * rhs ) const
		{
			return ( typeid ( *lhs ) == typeid ( *rhs ) );
		}
	};

private:
  vector<View *> views;

};

#endif // ViewManager_h__
