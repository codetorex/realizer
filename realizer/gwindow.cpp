#include "stdafx.h"
#include "gwindow.h"


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
}