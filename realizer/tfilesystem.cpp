#include "stdafx.h"
#include "tfilesystem.h"
#include "tfoldermount.h"
#include "tmemorystream.h"

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

TBuffer* TFileSystem::Load( const str8& path )
{
	TStream* file = Open(path,fm_Read);
	int length = file->Length();
	TBuffer* result = new TBuffer(length);
	file->Read(result->Data,1,length);
	file->Close();
	return result;
}

TMemoryStream* TFileSystem::LoadOpen( const str8& path )
{
	TBuffer* file = Load(path);
	TMemoryStream* result = new TMemoryStream(file);
	file->Data = 0;
	delete file;
	return result;
}

