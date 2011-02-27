#include "stdafx.h"
#include "tfilesystem.h"
#include "tfoldermount.h"

TStream* TFileSystem::Open( const str8& path,FileMode mode )
{
	if (TMount::IsWritableRequired(mode))
	{
		if (DefaultSaveMount == NULL) return NULL;
		return DefaultSaveMount->Open(path,mode);
	}

	if (LastUsedMount != 0)
	{
		if (LastUsedMount->CanRead())
		{
			if (LastUsedMount->Exists(path))
			{
				return LastUsedMount->Open(path,mode);
			}
		}
	}
	else
	{
		TMount* fileMount = FindFileInMounts(path);
		if (fileMount != NULL)
		{
			if (fileMount->CanRead())
			{
				LastUsedMount = fileMount;
				return fileMount->Open(path,mode);
			}
		}
	}
	return NULL;
}

TMount* TFileSystem::FindFileInMounts( const str8& path )
{
	int i = Mounts.Count;
	while(i--)
	{
		TMount* curMount = Mounts[i];
		if (curMount->Exists(path))
		{
			return curMount;
		}
	}
	return NULL;
}

TMount* TFileSystem::MountSystemFolder( const str8& folderPath,dword permission )
{
	TFolderMount* FMount = new TFolderMount(folderPath,permission);
	Mount(FMount);
	return FMount;
}