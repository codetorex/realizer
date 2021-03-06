#include "stdafx.h"
#include "tfilesystem.h"
#include "tfoldermount.h"
#include "tmemorystream.h"

IDirectory* TFileSystem::GetDirectory( const String& path )
{
	if (LastUsedMount != 0)
	{
		if ( LastUsedMount->ExistsDir(path) )
		{
			return LastUsedMount->GetDirectory(path);
		}
	}
	else
	{
		TMount* filemount = FindDirectoryInMounts(path);
		if (filemount != NULL)
		{
			LastUsedMount = filemount;
			return filemount->GetDirectory(path);
		}
	}

	return NULL;
}

TStream* TFileSystem::Open( const String& path,FileMode mode )
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

TMount* TFileSystem::FindDirectoryInMounts( const String& path )
{
	int i = Mounts.Count;
	while(i--)
	{
		TMount* curMount = Mounts[i];
		if (curMount->ExistsDir(path))
		{
			return curMount;
		}
	}
	return NULL;
}

TMount* TFileSystem::FindFileInMounts( const String& path )
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

TMount* TFileSystem::MountSystemFolder( const String& folderPath,ui32 permission )
{
	TFolderMount* FMount = new TFolderMount(folderPath,permission);
	Mount(FMount);
	return FMount;
}

TBuffer* TFileSystem::Load( const String& path )
{
	TStream* file = Open(path,fm_Read);
	int length = file->Length();
	TBuffer* result = new TBuffer(length);
	file->Read(result->Data,1,length);
	file->Close();
	return result;
}

TMemoryStream* TFileSystem::LoadOpen( const String& path )
{
	TBuffer* file = Load(path);
	TMemoryStream* result = new TMemoryStream(file);
	file->Data = 0;
	delete file;
	return result;
}

