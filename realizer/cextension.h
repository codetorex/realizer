#ifndef CEXTENSION_H
#define CEXTENSION_H

class CExtension
{
public:
	/**
	 * Return the extension name.
	 */
	virtual String*	GetExtensionName() = 0;
	
	/**
	 * Type of extension.
	 */
	virtual int			GetExtensionType() = 0;

	virtual void		Initialize() = 0;
	virtual void		Finalize() = 0;
};

class CExtensionManager: public Array< CExtension* >
{
public:
	Array< CExtension* > Extensions;


};

#endif