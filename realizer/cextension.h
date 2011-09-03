#ifndef CEXTENSION_H
#define CEXTENSION_H

class CExtension
{
public:
	virtual TString*	GetExtensionName() = 0;
	virtual int		GetExtensionType() = 0;
	virtual void*	GetExtension();
};

class CExtensionManager: public TArray< CExtension* >
{
public:

};

#endif