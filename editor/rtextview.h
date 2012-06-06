#ifndef RTEXTVIEW_H
#define RTEXTVIEW_H

#include "rdocumentview.h"
#include "rdocument.h"
#include "gscrollbar.h"

class TStream;
class RTextDocument;
class RTextViewStyle;
class TXMLNode;

class RTextViewStyleColor
{
public:
	TColor32 ForeColor;
	TColor32 BackColor;
};


/**
 * Style loader interface
 */
class RTextViewStyleLoader
{
public:
	virtual void LoadStyle(RTextViewStyle* style,TStream* srcStream) = 0;
};

/**
 * Visual studio 2010 style loader.
 * Styles from: http://studiostyl.es/
 */
class RTextViewStyleVS2010Loader: public RTextViewStyleLoader
{
public:
	static RTextViewStyleVS2010Loader Instance;

	TXMLNode* colorsNode;
	void GetColor(const TString& item, TColor32& fore, TColor32& back);

	void GetColor(const TString& item, RTextViewStyleColor& clr)
	{
		GetColor(item,clr.ForeColor,clr.BackColor);
	}

	void LoadStyle(RTextViewStyle* style,TStream* srcStream);
};


class RTextViewStyle
{
public:
	GFont* Font;

	RTextViewStyleColor Text;
	RTextViewStyleColor SelectedText;

	RTextViewStyleColor LineNumber;
	

	inline void LoadStyle(RTextViewStyleLoader* loader, TStream* styleStream)
	{
		loader->LoadStyle(this,styleStream);
	}
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

	GScrollBar VertBar;

public:

	static RTextViewStyle* DefaultStyle;

	RTextView();

	RTextViewStyle* Style;

	inline RTextDocument* GetTextDocument()
	{
		return (RTextDocument*)Document;
	}

	void OnMouseWheel(int x,int y, int delta);

	void OnKeyDown(ui32 keyID);

	void Render();
	
	void DocumentChanged();

	void Layout();
};


#endif