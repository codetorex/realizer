#ifndef TMOUNT_H
#define TMOUNT_H


#include "realizertypes.h"
#include "tarray.h"
#include "tstream.h"
#include "tdirectoryinfo.h"
#include "tfile.h"

class TMount
{
public:
	enum Permissions
	{
		Readable = 1,
		Writeable = 2,
	};

	enum MountType
	{
		FilesystemMount,
		PackageMount,
	};

	ui32 PermissionFlags;
	MountType MyType;

	IDirectory* MountRoot;

	virtual TStream*	Open(const TString& path, FileMode mode) = 0;
	virtual TStream*	Create(const TString& path) = 0;
	virtual bool		Delete(const TString& path) = 0;
	virtual bool		Move(const TString& srcpath,const TString& dstpath) = 0;
	virtual bool		Exists(const TString& path) = 0;

	virtual bool		CreateDir(const TString& path) = 0;
	virtual bool		DeleteDir(const TString& path) = 0;
	virtual bool		MoveDir(const TString& srcDir,const TString& dstDir) = 0;
	virtual bool		ExistsDir(const TString& path) = 0;

	virtual IFile*		GetFile(const TString& path) = 0;
	virtual IDirectory*	GetDirectory(const TString& path) = 0;

	bool HasPermission(FileMode mode);

	static bool IsWritableRequired(FileMode mode);
	static bool IsReadableRequired(FileMode mode);
	static void RequiredPermissions(FileMode mode, bool& OUT pWrite,bool& OUT pRead);

	inline bool CanRead()
	{
		return (PermissionFlags | Readable) > 0;
	}

	inline bool CanWrite()
	{
		return (PermissionFlags | Writeable) > 0;
	}
};

#endif