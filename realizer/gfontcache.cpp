#include "stdafx.h"
#include "gfontcache.h"
#include "cengine.h"
#include "trmlwriter.h"
#include "tstream.h"
#include "gfont.h"
#include "ttypebuilder.h"
#include "ttypemanager.h"

TType GFontFileMemberInfo()
{
	TType memberInfo;
	GFontFile ent;

	TTypeBuilder mib(&memberInfo,&ent);


	mib.SetName("FontFile");

	//mib.SetObjectName(&ent.FileName);

	//memberInfo.AddMember("FileName"     ,TMemberInfo::GetOffset(&ent,&ent.FileName)  ,MT_STRING);
	mib.AddField("FileName"     ,&ent.FileName  , Types.ST_String);
	mib.AddField("SizeSmallest" ,&ent.SizeMin   ,Types.ST_Int32);
	mib.AddField("SizeLargest"  ,&ent.SizeMax   ,Types.ST_Int32);
	mib.AddField("WidthSmallest",&ent.WeightMin ,Types.ST_Int32);
	mib.AddField("WidthLargest" ,&ent.WeightMax ,Types.ST_Int32);
	mib.AddField("IsItalic"     ,&ent.Italic    ,Types.ST_Bool);
	mib.AddField("OutlineWidth" ,&ent.Outline   ,Types.ST_Int32);
	mib.AddField("CanOutline"   ,&ent.CanOutline,Types.ST_Bool);

	return memberInfo;
}

TType GFontFile::MemberInfo = GFontFileMemberInfo();

TType GFontEntryMemberInfo()
{
	TType memberInfo;
	GFontEntry ent;

	TTypeBuilder mib(&memberInfo,&ent);
	
	mib.SetName("FontEntry");

	mib.AddField("FontName", &ent.FontName, Types.ST_String);

	//mib.SetObjectName(&ent.FontName);
	//memberInfo.AddMember("FontName",TMemberInfo::GetOffset(&ent,&ent.FontName),MT_STRING);
	mib.AddTemplateField("Files",&ent.Files, Types.ST_Array, &GFontFile::MemberInfo);
	
	return memberInfo;
}

TType GFontEntry::MemberInfo = GFontEntryMemberInfo();

const String FontPath = "fonts/";

void GFontCache::CreateCache()
{
	IDirectory* FontDirectory = Engine.FileSystem.GetDirectory(FontPath);
	IFileEnumerator* FontFiles = FontDirectory->EnumerateFiles();

	while(FontFiles->MoveNext())
	{
		String fileextension = FontFiles->Current->GetExtension();

		if (fileextension == ".fnt")
		{
			String fname = FontPath;
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
		rmlWriter->SerializeObject( &GFontEntry::MemberInfo, Entries.Items[i] );
	}


	rmlWriter->WriteElementEnd();
}

GFontEntry* GFontCache::CreateFontEntry( const String& fontname )
{
	GFontEntry* entry = new GFontEntry();
	entry->FontName = fontname;
	entry->CompareName = fontname.ToLower();
	//Entries.Add(fontname,entry);
	Entries.Add(entry);
	return entry;
}

GFontEntry* GFontCache::GetFontEntry( const String& fontname,bool createIfNotExist /*= false*/ )
{
	/*GFontEntry* entry = Entries.GetValueOrNull(fontname);
	if (entry) return entry;*/

	for (ui32 i=0;i<Entries.Count;i++)
	{
		GFontEntry* curEntry = Entries.Items[i];
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

GFontEntry* GFontCache::FindFontEntry( const String& fontname )
{
	/*GFontEntry* entry = Entries.GetValueOrNull(fontname);
	if (entry) return entry;*/
	GFontEntry* entry = GetFontEntry(fontname);
	if (entry) return entry;

	// TODO: if this function becomes sluggish you may need to implement THybridMap which is merge of TArray and HashMap it finds fast, and can look sequentially fast
	String lowercase = fontname.ToLower();

	for (ui32 i=0;i<Entries.Count;i++)
	{
		GFontEntry* curEntry = Entries.Items[i];
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

GFontFile* GFontEntry::AddFontFile( const String& path, int sizeMin, int sizeMax, int weightMin,int weightMax, bool italic, int outline, bool canOutline )
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