
#ifndef TFILESYSTEM_H
#define TFILESYSTEM_H

#include "tmount.h"

class TBuffer;
class TMemoryStream;

class TFileSystem
{
private:
	TMount* LastUsedMount;

public:
	inline TFileSystem()
	{
		LastUsedMount = 0;
	}

	TArray<TMount*> Mounts;

	TMount* MountSystemFolder(const TString& folderPath,dword permission);
	TMount* MountPackage(const TString& packagePath,dword permission);

	TMount*	DefaultSaveMount;

	
	inline void Mount(TMount* mount)
	{
		Mounts.Add(mount);
		if (mount->CanWrite())
		{
			DefaultSaveMount = mount;
		}
	}

	inline void Unmount(TMount* mount)
	{
		Mounts.Remove(mount);
	}

	/**
	* Opens file as a stream.
	*/
	TStream*		Open(const TString& path,FileMode mode);

	/**
	* Loads file to memory and returns buffer object.
	*/
	TBuffer*		Load(const TString& path);

	/**
	* This stream is fully memory cached.
	*/
	TMemoryStream*	LoadOpen(const TString& path);

	/**
	* Gets file entry record.
	*/
	IFile*			GetFile(const TString& path);

	/**
	* Gets directory entry record.
	*/
	IDirectory*		GetDirectory(const TString& path);

	/**
	* Finds file in mounts, and returns mount which contains the file.
	*/
	TMount*			FindFileInMounts(const TString& path);
};




#endif