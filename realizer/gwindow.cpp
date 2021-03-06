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

	void OnMouseDown(int x,int y, int button)
	{
		GObject* p = (GObject*)Parent;

		windowX = p->X;
		windowY = p->Y;

		gx = Master->X;
		gy = Master->Y;
		moving = true;
	}

	void OnMouseUp(int x,int y,int button)
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
			p->X = windowX + dx;
			p->Y = windowY + dy;
		}
	}
};

GWindow::GWindow()
{
	TitleBar = new GWindowTitlebar();
	AddChild(TitleBar);
	ClassID = GWINDOW_CLASSID;
	Layouter = &GLayout::Instance; // default layouter
	SetRectangle(0,0,250,250);
}

void GWindow::Render()
{
	Skin->RenderWindow(this);
	this->GObject::Render(); // render children

	/*Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(ScreenRegion.X + ObjectRegion.X,ScreenRegion.Y + ObjectRegion.Y,ObjectRegion.Width,ObjectRegion.Height,TColors::Red);*/
}

void GWindow::Update()
{
	this->GObject::Update();
	// do stuff here, like effects, animations?
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

	//this->GObject::Layout();
	// this->GObject::Layout(); TODO: use this for layouting the objects like docked, padded, 
	Skin->LayoutWindow(this);
	Layouter->Layout(this,false);
	//LayoutChilds(); // let them resize
	//TRectangle usage = Layouter->Layout(this,false);
	//ObjectRegion.SetRectangle(ObjectRegion.X + usage.X, ObjectRegion.Y + usage.Y,usage.Width,usage.Height);
}

void GWindow::CenterToScreen()
{
	int newX = (((GObject*)Parent)->Width - Width) / 2;
	int newY = (((GObject*)Parent)->Height - Height) / 2;

	X = newX;
	Y = newY;
}