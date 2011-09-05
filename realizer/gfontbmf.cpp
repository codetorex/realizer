#include "stdafx.h"
#include "gfontloader.h"
#include "vtexture.h"

class BMFLoader : GFontLoader
{
public:
	TStream* bmf;
	TString texturePath;

	BMFLoader(GFont* target,const TString& _texturePath,TStream* src):GFontLoader(target)
	{
		bmf = src;
		texturePath = _texturePath;
	}

	void Load()
	{
		ch8 magic[4];
		bmf->Read(magic,1,4);

		if (MemoryDriver::Compare(magic,"BMF\x03",4) != 0)
		{
			throw Exception("Unrecognized format");
		}

		byte blockType;
		int blockSize;
		while( bmf->Read(&blockType,1,1) )
		{
			bmf->Read(&blockSize,1,4);

			switch ( blockType )
			{
			case 1: // info
				ReadInfoBlock(blockSize);
				break;

			case 2: // common
				ReadCommonBlock(blockSize);
				break;

			case 3: // pages
				ReadPagesBlock(blockSize);
				break;

			case 4: // chars
				ReadCharsBlock(blockSize);
				break;

			case 5: // kerning pairs
				ReadKerningPairsBlock(blockSize);
				break;

			default:
				throw Exception("Unrecognized format");
			}
		}

		SetUndefinedCharacters();
		SetSpaceWidth();
	}

	void ReadInfoBlock(int size)
	{
		// pragma pack is supported by GCC too
#pragma pack(push)
#pragma pack(1)
		struct infoBlock
		{
			WORD fontSize;
			BYTE reserved:4;
			BYTE bold    :1;
			BYTE italic  :1;
			BYTE unicode :1;
			BYTE smooth  :1;
			BYTE charSet;
			WORD stretchH;
			BYTE aa;
			BYTE paddingUp;
			BYTE paddingRight;
			BYTE paddingDown;
			BYTE paddingLeft;
			BYTE spacingHoriz;
			BYTE spacingVert;
			BYTE outline;
			char fontName[1];
		};
#pragma pack(pop)

		byte buffer[1024];
		byte* src = buffer;
		if (size > 1024)
		{
			src = new byte[size];
		}

		bmf->Read(src,1,size);
		infoBlock* blk = (infoBlock*)src;

		font->Size = blk->fontSize > 32768 ? (65536 - blk->fontSize) : blk->fontSize;
		font->OutlineWidth = blk->outline;
		font->Name += blk->fontName;

		if (blk->aa)
		{
			font->Quality = FQ_ANTIALIASED;
		}

		if (blk->bold)
		{
			font->Weight = RW_BOLD;
		}

		if (blk->italic)
		{
			font->Italic = true;
		}

		if (size > 1024)
		{
			delete [] src;
		}
	}

	void ReadCommonBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct commonBlock
		{
			WORD lineHeight;
			WORD base;
			WORD scaleW;
			WORD scaleH;
			WORD pages;
			BYTE packed  :1;
			BYTE reserved:7;
			BYTE alphaChnl;
			BYTE redChnl;
			BYTE greenChnl;
			BYTE blueChnl;
		}; 
#pragma pack(pop)

		byte buffer[1024];
		bmf->Read(buffer,1,size);
		commonBlock* blk = (commonBlock*)buffer;
		font->BaseLine = blk->base;
	}

	void ReadPagesBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct pagesBlock
		{
			char pageNames[1];
		};
#pragma pack(pop)

		byte buffer[1024];
		bmf->Read(buffer,1,size);
		pagesBlock* blk = (pagesBlock*)buffer;

		char* pagename = blk->pageNames;

		// actually, pagename is relative to fnt file, but we using stream, so how do we know where is this shit? (old note)
		// yeah that is going to be problem so I implemented texturePath to know where to look hmm (05.09.2011)
		TString relativePath = texturePath;
		relativePath += pagename;

		font->FontTexture = Engine.Textures.LoadTexture( relativePath );
	}

	void ReadCharsBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct charsBlock
		{
			struct charInfo
			{
				DWORD id;
				WORD  x;
				WORD  y;
				WORD  width;
				WORD  height;
				short xoffset;
				short yoffset;
				short xadvance;
				BYTE  page;
				BYTE  chnl;
			} chars[1];
		};
#pragma pack(pop)
		byte* buffer = new byte [size];
		bmf->Read(buffer,1,size);

		charsBlock* blk = (charsBlock*)buffer;
		int charCount = size / sizeof(charsBlock::charInfo);

		for (int i =0;i<charCount;i++)
		{
			charsBlock::charInfo* ci = &blk->chars[i];
			GCharacter* curChar;
			if (ci->id != -1)
			{
				curChar = CreateCharacter(ci->id);
			}
			else
			{
				curChar = &font->DefaultCharacter;
			}
			SetChar(curChar,ci->x,ci->y,ci->width,ci->height,ci->xoffset,ci->yoffset,ci->xadvance);
		}

		delete [] buffer;
	}

	void ReadKerningPairsBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct kerningPairsBlock
		{
			struct kerningPair
			{
				DWORD first;
				DWORD second;
				short amount;
			} kerningPairs[1];
		};
#pragma pack(pop)
		byte* buffer = new byte [size];
		bmf->Read(buffer,1,size);

		kerningPairsBlock* blk = (kerningPairsBlock*)buffer;
		int pairCount = size / sizeof(kerningPairsBlock::kerningPair);
		for (int i=0;i<pairCount;i++)
		{
			AddKerningPair(blk->kerningPairs[i].first,blk->kerningPairs[i].second,blk->kerningPairs[i].amount);
		}

		delete [] buffer;
	}
};

void GFont::LoadBMF( TStream* bmfstream, const TString& texturePath, bool closestream )
{
	BMFLoader loader(this,texturePath,bmfstream);
	loader.Load();

	if (closestream)
	{
		bmfstream->Close();
		bmfstream = 0;
	}
}