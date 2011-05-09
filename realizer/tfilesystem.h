
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

	TMount* MountSystemFolder(const str8& folderPath,dword permission);
	TMount* MountPackage(const str8& packagePath,dword permission);

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
	TStream*		Open(const str8& path,FileMode mode);

	/**
	* Loads file to memory and returns buffer object.
	*/
	TBuffer*		Load(const str8& path);

	/**
	* This stream is fully memory cached.
	*/
	TMemoryStream*	LoadOpen(const str8& path);

	/**
	* Gets file entry record.
	*/
	IFile*			GetFile(const str8& path);

	/**
	* Gets directory entry record.
	*/
	IDirectory*		GetDirectory(const str8& path);

	/**
	* Finds file in mounts, and returns mount which contains the file.
	*/
	TMount*			FindFileInMounts(const str8& path);
};




#endif