#ifndef GFONTCACHE_H
#define GFONTCACHE_H

#include <tserializable.h>
#include <tarray.h>

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
	str8 FileName;

	int sizeMin;
	int sizeMax;

	int weightMin;
	int weightMax;

	bool italic;

	int outline;
	bool canoutline;

	static TMemberInfo MemberInfo;
};

class GFontEntry
{
public:
	str8 FontName;

	TArray< GFontFile* > Files;
	TArray< GFont* > Loaded;

	static TMemberInfo MemberInfo;

	GFont* GetFont(int fontSize = 12, FontWeight fontWeight = RW_NORMAL , int outlineWidth = 0, bool italic = false);
};

class GFontCache
{
public:
	TArray< GFontEntry* > Entries;

	/**
	* Loads cache from file.
	*/
	void LoadCache(const str8& cachefile);

	/**
	* Saves cache to file.
	*/
	void SaveCache(const str8& cachefile);

	/**
	* Loops through fonts folder and generates entries for font files.
	*/
	void CreateCache();

	GFontEntry* GetFontEntry(const str8& fontname);
};

#endif

