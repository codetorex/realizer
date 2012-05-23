#ifndef RTEXTVIEW_H
#define RTEXTVIEW_H

#include "rdocumentview.h"
#include "rdocument.h"
#include "gscrollbar.h"

class TStream;
class RTextDocument;

class RTextViewStyle
{
public:
	GFont* Font;

	void LoadStyle(TStream* styleStream);

	void LoadVisualStudioStyle(TStream* visualStudio);
};

// THERE SHOULD BE GLOBAL STYLE
// AND PER LANGUAGE OVERRIDES

/**
 * Main text editor/viewer implementation.
 */
class RTextView: public RDocumentView
{
private:
	/// Pixel height of single line
	int LineHeight;

	/// Count of possible line count in screen
	int LineCount;

	/// Character length of line number width
	int LineNumberWidth;

	/// Character pixel length for line number
	int LineNumberPixelWidth;

	bool ShowLineNumbers;

	TColor32 LineNumberBackColor;
	TColor32 LineNumberForeColor;

	GScrollBar VertBar;

public:
	RTextView();

	RTextViewStyle* Style;

	inline RTextDocument* GetTextDocument()
	{
		return (RTextDocument*)Document;
	}

	void MouseWheel(int x,int y, int delta);

	void KeyDown(ui32 keyID);

	void Render();
	
	void DocumentChanged();

	void Layout();
};


#endif