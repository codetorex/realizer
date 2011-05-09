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

	dword PermissionFlags;
	MountType MyType;

	IDirectory* MountRoot;

	virtual TStream*	Open(const str8& path, FileMode mode) = 0;
	virtual TStream*	Create(const str8& path) = 0;
	virtual bool		Delete(const str8& path) = 0;
	virtual bool		Move(const str8& srcpath,const str8& dstpath) = 0;
	virtual bool		Exists(const str8& path) = 0;

	virtual bool		CreateDir(const str8& path) = 0;
	virtual bool		DeleteDir(const str8& path) = 0;
	virtual bool		MoveDir(const str8& srcDir,const str8& dstDir) = 0;

	virtual IFile*		GetFile(const str8& path) = 0;
	virtual IDirectory*	GetDirectory(const str8& path) = 0;

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