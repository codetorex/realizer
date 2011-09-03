// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FONTLOADER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FONTLOADER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FONTLOADER_EXPORTS
#define FONTLOADER_API __declspec(dllexport)
#else
#define FONTLOADER_API __declspec(dllimport)
#endif

// This class is exported from the fontloader.dll
class FONTLOADER_API Cfontloader {
public:
	Cfontloader(void);
	// TODO: add your methods here.
};

extern FONTLOADER_API int nfontloader;

FONTLOADER_API int fnfontloader(void);
