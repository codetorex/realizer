#include "stdafx.h"
#include "rproject.h"
#include "txmlwriter.h"
#include "reditor.h"

void RProjectNode::SerializeNodes(TXMLWriter& xw)
{
	TArrayEnumerator<GTreeNode*> nd(Nodes);
	while(nd.MoveNext())
	{
		RProjectNode* pn = (RProjectNode*)nd.Current;
		pn->Serialize(xw);
	}
}

void RProject::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("project");
	xw.WriteAttributeString("name",ProjectName);

	SerializeNodes(xw);

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

void RProject::SaveAs( const String& projectPath )
{
	TFileStream* fs = File::OpenWrite(projectPath);
	TXMLWriter xw(fs);

	xw.WriteStartElement("realizer");

	Serialize(xw);

	xw.WriteEndElement();


	xw.Close();
}

RProject::RProject()
{
	Text = "Project";
	Type = RProjectNode::RS_PROJECT;
	Image = Resources.PVProject;
}

