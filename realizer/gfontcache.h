#ifndef GFONTCACHE_H
#define GFONTCACHE_H

#include <tserialization.h>
#include <tarray.h>
#include <thashmap.h>


enum FontQuality
{
	FQ_DEFAULT,
	FQ_ANTIALIASED,
};

enum FontWeight
{
	RW_THIN = 100,
	RW_ULTRALIGHT = 200,
	RW_LIGHT = 300,
	RW_NORMAL = 400,
	RW_MEDIUM = 500,
	RW_SEMIBOLD = 600,
	RW_BOLD = 700,
	RW_ULTRABOLD = 800,
	RW_HEAVY = 900,
};

class GFont;


class GFontFile
{
public:
	TString FileName;

	int SizeMin;
	int SizeMax;

	int WeightMin;
	int WeightMax;

	bool Italic;

	int Outline;
	bool CanOutline;

	inline bool Match(int _size, int _weight, bool _italic,int _outline)
	{
		if ( _size < SizeMin && _size > SizeMax)
		{
			return false;
		}

		if (_weight < WeightMin && _weight > WeightMax)
		{
			return false;
		}

		if (_italic != Italic)
		{
			return false;
		}

		if ( Outline != _outline )
		{
			if (!CanOutline)
			{
				return false;
			}
		}
		return true;
	}

	static TType MemberInfo;
};

class GFontEntry
{
public:
	TString FontName;
	TString CompareName;

	TArray< GFontFile* > Files;

	/// TODO: Rename all MemberInfo to TypeInfo
	static TType MemberInfo;

	GFont* GetFont(int fontSize = 12, FontWeight fontWeight = RW_NORMAL , int outlineWidth = 0, bool italic = false);

	GFontFile* AddFontFile(const TString& path, int sizeMin, int sizeMax, int weightMin,int weightMax, bool italic, int outline, bool canOutline);
};

class TStream;

class GFontCache
{
public:
	TArray< GFontEntry* > Entries;

	/**
	* Loads cache from file.
	*/
	void LoadCache(TStream* cacheStream);

	/**
	* Saves cache to file.
	*/
	void SaveCache(TStream* cacheStream);

	/**
	* Loops through fonts folder and generates entries for font files.
	*/
	void CreateCache();

	/**
	 * Gets font entry by name from entries array. If not found creates if told like so or returns null.
	 */
	GFontEntry* GetFontEntry(const TString& fontname,bool createIfNotExist = false);

	GFontEntry* FindFontEntry( const TString& fontname );

	/**
	 * Creates a font entry and adds it to entries array.
	 */
	GFontEntry* CreateFontEntry(const TString& fontname);
};

#endif

