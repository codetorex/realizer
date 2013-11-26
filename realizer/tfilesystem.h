
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

	Array<TMount*> Mounts;

	TMount* MountSystemFolder(const String& folderPath,ui32 permission);
	TMount* MountPackage(const String& packagePath,ui32 permission);

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
	TStream*		Open(const String& path,FileMode mode);

	/**
	* Loads file to memory and returns buffer object.
	*/
	TBuffer*		Load(const String& path);

	/**
	* This stream is fully memory cached.
	*/
	TMemoryStream*	LoadOpen(const String& path);

	/**
	* Gets file entry record.
	*/
	IFile*			GetFile(const String& path);

	/**
	* Gets directory entry record.
	*/
	IDirectory*		GetDirectory(const String& path);

	/**
	* Finds file in mounts, and returns mount which contains the file.
	*/
	TMount*			FindFileInMounts(const String& path);

	/**
	 * Finds directory in mounts, and returns mount which contains the directory.
	 */
	TMount*			FindDirectoryInMounts(const String& path);
};




#endif