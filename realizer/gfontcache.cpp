#include "stdafx.h"
#include "gfontcache.h"
#include "cengine.h"
#include "trmlwriter.h"
#include "tstream.h"
#include "gfont.h"

TMemberInfo GFontFileMemberInfo()
{
	TMemberInfo memberInfo;
	GFontFile ent;

	TMemberInfoBuilder mib(&memberInfo,&ent);

	mib.SetObjectName(&ent.FileName);

	//memberInfo.AddMember("FileName"     ,TMemberInfo::GetOffset(&ent,&ent.FileName)  ,MT_STRING);
	mib.AddMember("SizeSmallest" ,&ent.SizeMin   ,MT_INT);
	mib.AddMember("SizeLargest"  ,&ent.SizeMax   ,MT_INT);
	mib.AddMember("WidthSmallest",&ent.WeightMin ,MT_INT);
	mib.AddMember("WidthLargest" ,&ent.WeightMax ,MT_INT);
	mib.AddMember("IsItalic"     ,&ent.Italic    ,MT_BOOL);
	mib.AddMember("OutlineWidth" ,&ent.Outline   ,MT_INT);
	mib.AddMember("CanOutline"   ,&ent.CanOutline,MT_BOOL);

	return memberInfo;
}

TMemberInfo GFontFile::MemberInfo = GFontFileMemberInfo();

TMemberInfo GFontEntryMemberInfo()
{
	TMemberInfo memberInfo;
	GFontEntry ent;

	TMemberInfoBuilder mib(&memberInfo,&ent);
	
	mib.SetObjectName(&ent.FontName);
	//memberInfo.AddMember("FontName",TMemberInfo::GetOffset(&ent,&ent.FontName),MT_STRING);
	mib.AddMember("Files",&ent.Files,MT_ARRAY,MT_OBJECT,&GFontFile::MemberInfo);
	
	return memberInfo;
}

TMemberInfo GFontEntry::MemberInfo = GFontEntryMemberInfo();

const TString FontPath = "fonts/";

void GFontCache::CreateCache()
{
	IDirectory* FontDirectory = Engine.FileSystem.GetDirectory(FontPath);
	IFileEnumerator* FontFiles = FontDirectory->EnumerateFiles();

	while(FontFiles->MoveNext())
	{
		TString fileextension = FontFiles->Current->GetExtension();

		if (fileextension == ".fnt")
		{
			TString fname = FontPath;
			fname += FontFiles->Current->GetName();

			GFont* LoadedFont = Engine.GUI.Fonts.LoadStaticFont(fname); // .FNT is a static font
			if (LoadedFont)
			{
				GFontEntry* fentry = GetFontEntry(LoadedFont->Name, true);
				GFontFile* curfile = fentry->AddFontFile(fname,LoadedFont->Size,LoadedFont->Size,LoadedFont->Weight,LoadedFont->Weight,LoadedFont->Italic,LoadedFont->OutlineWidth,false);
			}
			else
			{
				throw Exception("Cant load font file for cache");
			}
			
		}
	}
	
}

void GFontCache::SaveCache( TStream* cacheStream )
{
	TRMLWriter* rmlWriter = new TRMLWriter(cacheStream);
	rmlWriter->WriteElementStart("fontcache");

	/*THashMapEnumerator<GFontEntry*> sEntries(&Entries);

	while(sEntries.MoveNext())
	{
		rmlWriter->Serialize(&GFontEntry::MemberInfo,sEntries.Current->Value);
	}*/
	for (ui32 i=0;i<Entries.Count;i++)
	{
		rmlWriter->SerializeObject( &GFontEntry::MemberInfo, Entries.Item[i] );
	}


	rmlWriter->WriteElementEnd();
}

GFontEntry* GFontCache::CreateFontEntry( const TString& fontname )
{
	GFontEntry* entry = new GFontEntry();
	entry->FontName = fontname;
	entry->CompareName = fontname.ToLower();
	//Entries.Add(fontname,entry);
	Entries.Add(entry);
	return entry;
}

GFontEntry* GFontCache::GetFontEntry( const TString& fontname,bool createIfNotExist /*= false*/ )
{
	/*GFontEntry* entry = Entries.GetValueOrNull(fontname);
	if (entry) return entry;*/

	for (ui32 i=0;i<Entries.Count;i++)
	{
		GFontEntry* curEntry = Entries.Item[i];
		if (curEntry->FontName == fontname)
		{
			return curEntry;
		}
	}

	//not found
	if (createIfNotExist)
	{
		return CreateFontEntry(fontname);
	}

	return 0;
}

GFontEntry* GFontCache::FindFontEntry( const TString& fontname )
{
	/*GFontEntry* entry = Entries.GetValueOrNull(fontname);
	if (entry) return entry;*/
	GFontEntry* entry = GetFontEntry(fontname);
	if (entry) return entry;

	// TODO: if this function becomes sluggish you may need to implement THybridMap which is merge of TArray and HashMap it finds fast, and can look sequentially fast
	TString lowercase = fontname.ToLower();

	for (ui32 i=0;i<Entries.Count;i++)
	{
		GFontEntry* curEntry = Entries.Item[i];
		if (curEntry->CompareName.IndexOf(lowercase) != -1)
		{
			return curEntry;
		}
	}

	/*THashMapEnumerator<GFontEntry*> allentries(&Entries);
	while(allentries.MoveNext())
	{
		if (allentries.Current->Value->CompareName.IndexOf(lowercase) != -1)
		{
			return allentries.Current->Value;
		}
	}*/

	// still not found?
	return 0;
}

GFontFile* GFontEntry::AddFontFile( const TString& path, int sizeMin, int sizeMax, int weightMin,int weightMax, bool italic, int outline, bool canOutline )
{
	GFontFile* fileEntry = new GFontFile();
	fileEntry->FileName = path;
	fileEntry->SizeMin = sizeMin;
	fileEntry->SizeMax = sizeMax;
	fileEntry->WeightMin = weightMin;
	fileEntry->WeightMax = weightMax;
	fileEntry->Italic = italic;
	fileEntry->Outline = outline;
	fileEntry->CanOutline = canOutline;

	Files.Add(fileEntry);
	return fileEntry;
}