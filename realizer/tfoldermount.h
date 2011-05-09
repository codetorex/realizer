#ifndef TFOLDERMOUNT_H
#define TFOLDERMOUNT_H

#include "tmount.h"


class TFolderMount: public TMount
{
private:
	inline void GetRelativePath(str8& tmpStr, const str8& path)
	{
		tmpStr += FullPath;
		//tmpStr += TPath::DirectorySeprator;
		tmpStr += path;
	}

public:
	str8 FullPath; // should always end with directory seprator

	TFolderMount(const str8& path,dword permission)
	{
		TDirectoryInfo* td = new TDirectoryInfo(path);
		MountRoot = td;
		FullPath = td->FullPath;
		PermissionFlags = permission;
		MyType = FilesystemMount;
	}

	~TFolderMount()
	{
		delete MountRoot;
	}

	TStream* Open(const str8& path, FileMode mode)
	{
		str8 tmpStr(1024); // TODO: write a better string allocation and dealloction algrotihms to make these temporary stuff better.
		GetRelativePath(tmpStr,path);
		TStream* result = File::Open(tmpStr,mode);
		return result;
	}

	TStream* Create(const str8& path)
	{
		return Open(path,fm_Write);
	}

	bool Delete(const str8& path)
	{

		return false;
	}

	bool Move(const str8& srcpath,const str8& dstpath)
	{
		return false;
	}

	bool Exists(const str8& path)
	{
		str8 tmpStr(1024);
		GetRelativePath(tmpStr,path);
		return TFileInfo::Exists(tmpStr);
	}

	bool CreateDir(const str8& path)
	{

		return false;
	}

	bool DeleteDir(const str8& path)
	{
		return false;
	}

	bool MoveDir(const str8& srcDir,const str8& dstDir)
	{
		return false;
	}

	IFile* GetFile(const str8& path)
	{
		TFileInfo* file = new TFileInfo(path);
		return file;
	}

	IDirectory*	GetDirectory(const str8& path)
	{
		throw "Not Impelemented";
	}
};

#endif