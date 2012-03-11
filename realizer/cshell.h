#ifndef CSHELL_H
#define CSHELL_H


#include "tconsoledriver.h"
#include "tapplication.h"
#include "tdictionary.h"

class CShellCommand
{
public:
	TString Command;
	TString CommandLine;



};

class CShellVariable
{
public:
	TString Variable;
	void* Target;

};


class 
{
public:


};



/**
 * Event driver command shell for controlling engine.
 */
class CCommandManager
{
public:
	TConsoleDriver* Output;
	
	TDictionary<CShellCommand> Commands;

	TDictionary<CShellVariable> Variables;

	CShellCommand* CurrentCommand; // current running command


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

		Output->KeyPress +=	GetHandler(this,&CShell::Input);

		WritePrompt();
	}

	void WritePrompt()
	{
		Output->Write("realizer$ ");
	}
};

#endif