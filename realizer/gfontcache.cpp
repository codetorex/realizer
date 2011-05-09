#include "stdafx.h"
#include "gfontcache.h"

TMemberInfo GFontFileMemberInfo()
{
	TMemberInfo memberInfo;
	GFontFile ent;

	memberInfo.SetObjectName(TMemberInfo::GetOffset(&ent,&ent.FileName));
	//memberInfo.AddMember("FileName"     ,TMemberInfo::GetOffset(&ent,&ent.FileName)  ,MT_STRING);
	memberInfo.AddMember("SizeSmallest" ,TMemberInfo::GetOffset(&ent,&ent.sizeMin)   ,MT_INT);
	memberInfo.AddMember("SizeLargest"  ,TMemberInfo::GetOffset(&ent,&ent.sizeMax)   ,MT_INT);
	memberInfo.AddMember("WidthSmallest",TMemberInfo::GetOffset(&ent,&ent.weightMin) ,MT_INT);
	memberInfo.AddMember("WidthLargest" ,TMemberInfo::GetOffset(&ent,&ent.weightMax) ,MT_INT);
	memberInfo.AddMember("IsItalic"     ,TMemberInfo::GetOffset(&ent,&ent.italic)    ,MT_BOOL);
	memberInfo.AddMember("OutlineWidth" ,TMemberInfo::GetOffset(&ent,&ent.outline)   ,MT_INT);
	memberInfo.AddMember("CanOutline"   ,TMemberInfo::GetOffset(&ent,&ent.canoutline),MT_BOOL);

	return memberInfo;
}

TMemberInfo GFontFile::MemberInfo = GFontFileMemberInfo();

TMemberInfo GFontEntryMemberInfo()
{
	TMemberInfo memberInfo;
	GFontEntry ent;
	
	memberInfo.SetObjectName(TMemberInfo::GetOffset(&ent,&ent.FontName));
	//memberInfo.AddMember("FontName",TMemberInfo::GetOffset(&ent,&ent.FontName),MT_STRING);
	memberInfo.AddMember("Files",TMemberInfo::GetOffset(&ent,&ent.Loaded),MT_ARRAY,MT_OBJECT,&GFontFile::MemberInfo);
	
	return memberInfo;
}

TMemberInfo GFontEntry::MemberInfo = GFontEntryMemberInfo();