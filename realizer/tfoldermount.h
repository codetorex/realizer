#ifndef TFOLDERMOUNT_H
#define TFOLDERMOUNT_H

#include "tmount.h"


class TFolderMount: public TMount
{
private:
	inline void GetRelativePath(String& tmpStr, const String& path)
	{
		tmpStr += FullPath;
		//tmpStr += TPath::DirectorySeprator;
		tmpStr += path;
	}

public:
	String FullPath; // should always end with directory seprator

	TFolderMount(const String& path,ui32 permission)
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

	TStream* Open(const String& path, FileMode mode)
	{
		String tmpStr(1024); // TODO: write a better string allocation and dealloction algrotihms to make these temporary stuff better.
		GetRelativePath(tmpStr,path);
		TStream* result = File::Open(tmpStr,mode);
		return result;
	}

	TStream* Create(const String& path)
	{
		return Open(path,fm_Write);
	}

	bool Delete(const String& path)
	{

		return false;
	}

	bool Move(const String& srcpath,const String& dstpath)
	{
		return false;
	}

	bool Exists(const String& path)
	{
		String tmpStr(1024);
		GetRelativePath(tmpStr,path);
		return TFileInfo::Exists(tmpStr);
	}

	bool CreateDir(const String& path)
	{

		return false;
	}

	bool DeleteDir(const String& path)
	{
		return false;
	}

	bool MoveDir(const String& srcDir,const String& dstDir)
	{
		return false;
	}

	bool ExistsDir(const String& path)
	{
		String tmpStr(1024);
		GetRelativePath(tmpStr,path);
		return TDirectoryInfo::Exists(tmpStr);
	}

	IFile* GetFile(const String& path)
	{
		TFileInfo* file = new TFileInfo(path);
		return file;
	}

	IDirectory*	GetDirectory(const String& path)
	{
		String tmpStr(1024);
		GetRelativePath(tmpStr,path);
		TDirectoryInfo* DirectoryInterface = new TDirectoryInfo(tmpStr);
		return DirectoryInterface;
	}
};

#endif