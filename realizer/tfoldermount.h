#ifndef TFOLDERMOUNT_H
#define TFOLDERMOUNT_H

#include "tmount.h"


class TFolderMount: public TMount
{
private:
	inline void GetRelativePath(TString& tmpStr, const TString& path)
	{
		tmpStr += FullPath;
		//tmpStr += TPath::DirectorySeprator;
		tmpStr += path;
	}

public:
	TString FullPath; // should always end with directory seprator

	TFolderMount(const TString& path,dword permission)
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

	TStream* Open(const TString& path, FileMode mode)
	{
		TString tmpStr(1024); // TODO: write a better string allocation and dealloction algrotihms to make these temporary stuff better.
		GetRelativePath(tmpStr,path);
		TStream* result = File::Open(tmpStr,mode);
		return result;
	}

	TStream* Create(const TString& path)
	{
		return Open(path,fm_Write);
	}

	bool Delete(const TString& path)
	{

		return false;
	}

	bool Move(const TString& srcpath,const TString& dstpath)
	{
		return false;
	}

	bool Exists(const TString& path)
	{
		TString tmpStr(1024);
		GetRelativePath(tmpStr,path);
		return TFileInfo::Exists(tmpStr);
	}

	bool CreateDir(const TString& path)
	{

		return false;
	}

	bool DeleteDir(const TString& path)
	{
		return false;
	}

	bool MoveDir(const TString& srcDir,const TString& dstDir)
	{
		return false;
	}

	bool ExistsDir(const TString& path)
	{
		TString tmpStr(1024);
		GetRelativePath(tmpStr,path);
		return TDirectoryInfo::Exists(tmpStr);
	}

	IFile* GetFile(const TString& path)
	{
		TFileInfo* file = new TFileInfo(path);
		return file;
	}

	IDirectory*	GetDirectory(const TString& path)
	{
		TString tmpStr(1024);
		GetRelativePath(tmpStr,path);
		TDirectoryInfo* DirectoryInterface = new TDirectoryInfo(tmpStr);
		return DirectoryInterface;
	}
};

#endif