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

	virtual TStream*	Open(const String& path, FileMode mode) = 0;
	virtual TStream*	Create(const String& path) = 0;
	virtual bool		Delete(const String& path) = 0;
	virtual bool		Move(const String& srcpath,const String& dstpath) = 0;
	virtual bool		Exists(const String& path) = 0;

	virtual bool		CreateDir(const String& path) = 0;
	virtual bool		DeleteDir(const String& path) = 0;
	virtual bool		MoveDir(const String& srcDir,const String& dstDir) = 0;
	virtual bool		ExistsDir(const String& path) = 0;

	virtual IFile*		GetFile(const String& path) = 0;
	virtual IDirectory*	GetDirectory(const String& path) = 0;

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