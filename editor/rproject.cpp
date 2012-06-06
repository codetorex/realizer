#include "stdafx.h"
#include "rproject.h"
#include "txmlwriter.h"

void RProjectFolder::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("folder");
	xw.WriteAttributeString("name", Text);

	TArrayEnumerator<GTreeNode*> nd(Nodes);
	while(nd.MoveNext())
	{
		RProjectNode* pn = (RProjectNode*)nd.Current;
		pn->Serialize(xw);
	}

	xw.WriteEndElement();
}

void RProjectSource::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("source");
	xw.WriteAttributeString("path", FilePath);
	xw.WriteEndElement();
}

void RProject::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("project");
	xw.WriteAttributeString("name",ProjectName);
	xw.WriteEndElement();
}


void RProject::UpdateText()
{
	TStringBuilderStack<256> sb;
	sb.Append("Project '");
	sb.Append(ProjectName);
	sb.AppendChar('\'');

	Text = sb.ToString();
}

void RProject::SaveAs( const TString& projectPath )
{
	TFileStream* fs = File::OpenWrite(projectPath);
	TXMLWriter xw(fs);

	xw.WriteStartElement("realizer");

	Serialize(xw);

	xw.WriteEndElement();


	xw.Close();
}

