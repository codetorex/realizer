#ifndef CCOMMAND_H
#define CCOMMAND_H


#include "tconsoledriver.h"
#include "tapplication.h"
#include "tdictionary.h"

#include "tlogconsole.h"

class CCommand
{
public:
	TString Command;
	TString CommandLineParameters;




};

class CVariable
{
public:
	TString Variable;
	void* Target;

};

/**
 * Event driver command shell for controlling engine.
 */
class CCommandManager
{
public:
	TConsoleDriver* Output;
	
	TDictionary<CCommand> Commands;

	TDictionary<CVariable> Variables;

	CCommand* CurrentCommand; // current running command


	//TQueue<ch32> InputBuffer;

	/**
	 * A command input.
	 */
	void Input(ch32 chr);

	void RunCmd(const TString& cmd);

	void Start(TConsoleDriver* _output)
	{
		Output = _output;

		Output->SetForeColor(CC_GREEN);
		Output->WriteLine(Application.IdentifyText);

		//Output->KeyPress +=	GetHandler(this,&CShell::Input);

		//WritePrompt();
	}

	void ConnectAsLogOutput()
	{
		TLogConsole* lg = new TLogConsole(Output);
		Log.RegisterOutput(lg);
	}

	void WritePrompt()
	{
		Output->Write("realizer$ ");
	}
};

#endif