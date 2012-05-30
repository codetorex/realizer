#ifndef GDROPDOWN_H
#define GDROPDOWN_H

#include "gobject.h"



class GDropDown: public GObject
{
protected:
	GObject* LastFocus;

public:
	GDropDown* ParentMenu;
	GDropDown* SubMenu;

	void SetSubMenu(GDropDown* pSubMenu);
	void HideSubMenu();


	GDropDown* GetRootMenu();

	bool IsRelative(GDropDown* menu);

	bool IsRelativeObject(GObject* obj);

	GLayout* Layouter; // use a vertical layouter

	GDropDown();

	void Render();
	void Update();
	void Layout();

	/**
	 * Popups at given position
	 */
	void Show(int x,int y);
	
	/**
	 * Popups at current cursor position
	 */
	void Show();

	void Hide();
};


#endif