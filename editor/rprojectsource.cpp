#include "stdafx.h"
#include "rprojectsource.h"
#include "txmlwriter.h"
#include "reditor.h"

RProjectSourceTypeManager RProjectSourceTypeManager::Instance;

void RProjectSource::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("source");
	xw.WriteAttributeString("path", FilePath);
	xw.WriteEndElement();
}

RProjectSource::RProjectSource( const TString& path )
{
	FilePath = path;
	UpdateType();
}

void RProjectSource::UpdateType()
{
	TString ext = TPath::GetExtension(FilePath);
	ext.ToLowerInplace();
	SourceType = RProjectSourceTypeManager::Instance.GetType(ext);
	Image = SourceType->Image;
}

void RProjectSource::UpdateText()
{
	Text = TPath::GetFileName(FilePath);
}

void RProjectSourceTypeManager::InitializeDefaultTypes()
{
	Unknown.Image = Resources.PVDocument;
	Unknown.Name = "Unknown";
	Unknown.Index = -1;

	AddType(new RProjectSourceTypeImage());
}

void RProjectSourceTypeManager::AddType( RProjectSourceType* typ )
{
	typ->Index = Types.Count;
	Types.Add(typ);
}

RProjectSourceType* RProjectSourceTypeManager::GetType( const TString& ext )
{
	RProjectSourceType* result = 0;

	TArrayEnumerator< RProjectSourceType* > ae(Types);
	while(ae.MoveNext())
	{
		TArrayEnumerator< TString* > exts(ae.Current->Extensions);
		while(exts.MoveNext())
		{
			if (*exts.Current == ext)
			{
				result = ae.Current;
				break;
			}
		}

		if (result)
		{
			break;
		}
	}

	if (!result)
	{
		result = &Unknown;
	}

	return result;
}

RProjectSourceTypeImage::RProjectSourceTypeImage()
{
	Name = "Image";
	Image = Resources.PVImage;
	AddExtension(".bmp");
	AddExtension(".png");
	AddExtension(".jpg");
}

void RProjectSourceTypeImage::Open( RProjectSource* src )
{
	throw NotImplementedException();
}