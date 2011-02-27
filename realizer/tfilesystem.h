
#ifndef TFILESYSTEM_H
#define TFILESYSTEM_H

#include "tmount.h"

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

	TStream*		Open(const str8& path,FileMode mode);
	IFile*			GetFile(const str8& path);
	IDirectory*		GetDirectory(const str8& path);
	TMount*			FindFileInMounts(const str8& path);
};




#endif