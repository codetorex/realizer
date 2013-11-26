#ifndef CCOMMAND_H
#define CCOMMAND_H


#include "tconsole.h"
#include "tapplication.h"
#include "tdictionary.h"

#include "tlogconsole.h"

class CCommand
{
public:
	String Command;
	String CommandLineParameters;




};

class CVariable
{
public:
	String Variable;
	void* Target;

};

/**
 * Event driver command shell for controlling engine.
 */
class CCommandManager
{
public:
	TConsole* Output;
	
	TDictionary<CCommand> Commands;

	TDictionary<CVariable> Variables;

	CCommand* CurrentCommand; // current running command


	//TQueue<ch32> InputBuffer;

	/**
	 * A command input.
	 */
	void Input(ui32 chr);

	void RunCmd(const String& cmd);

	void Start(TConsole* _output)
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