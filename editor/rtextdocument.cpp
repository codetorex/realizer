#include "stdafx.h"
#include "rtextdocument.h"
#include "tfile.h"
#include "tstreamreader.h"

void RTextDocument::LoadDocument()
{
	// delete prior datas
	Lines.DeletePointers();

	TStream* fs = TFile::OpenRead(FilePath);
	TStreamReader* sr = new TStreamReader(fs);

	while(!sr->EndOfStream)
	{
		TString curLine = sr->ReadLine();
		if (sr->EndOfStream && curLine.Length == 0)
		{
			break;
		}

		RTextLine* rline = new RTextLine();
		rline->Line = curLine;
		Lines.Add(rline);
	}

	sr->Close(); // already deletes fs dont worry for it
	// TODO: make Tstream stuff as smart ptrs

	UpdateName();
}
