#include "stdafx.h"
#include "gwindow.h"
#include "vgui.h"

// TODO: remove this
#include "gschemedskin.h"

class GWindowTitlebar: public GObject
{
public:
	int windowX,windowY;
	int gx,gy;
	bool moving;

	GWindowTitlebar()
	{
		moving = false;
	}

	void MouseDown(int x,int y, int button)
	{
		GObject* p = (GObject*)Parent;

		windowX = p->X;
		windowY = p->Y;

		gx = Master->X;
		gy = Master->Y;
		moving = true;
	}

	void MouseUp(int x,int y,int button)
	{
		moving = false;
	}

	void Update()
	{
		this->GObject::Update();

		if (moving)
		{
			int dx,dy;
			dx = Master->X - gx;
			dy = Master->Y - gy;
			
			GObject* p = (GObject*)Parent;
			p->SetLeft(windowX + dx);
			p->SetTop(windowY + dy);
		}
	}
};

GWindow::GWindow()
{
	TitleBar = new GWindowTitlebar();
	AddChild(TitleBar);
	ClassID = GWINDOW_CLASSID;
}

void GWindow::Render()
{
	Skin->RenderWindow(this);
	this->GObject::Render(); // render children
}

void GWindow::Update()
{
	this->GObject::Update();
	// do stuff here, like effects?
}

void GWindow::Initialize()
{
	// initialize and position close button here.
	// call PerformLayout here.
	TitleBar->Master = Master;
	TitleBar->Skin = Skin;
	Layout();
}

void GWindow::Layout()
{
	// this->GObject::Layout(); TODO: use this for layouting the objects like docked, padded, 
	Skin->LayoutWindow(this);
}

void GWindow::CenterToScreen()
{
	int newX = (((GObject*)Parent)->Width - Width) / 2;
	int newY = (((GObject*)Parent)->Height - Height) / 2;

	SetLeft(newX);
	SetTop(newY);
}