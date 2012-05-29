#include "stdafx.h"
#include "rproject.h"


void RProject::UpdateText()
{
	TStringBuilderStack<256> sb;
	sb.Append("Project '");
	sb.Append(ProjectName);
	sb.AppendChar('\'');

	Text = sb.ToString();
}

