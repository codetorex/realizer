#include "stdafx.h"
#include "tmount.h"

bool TMount::IsWritableRequired( FileMode mode )
{
	switch(mode)
	{
	case fm_Append:
	case fm_Write:
		return true;
		break;

	case fm_ReadWrite:
	case fm_AppendRead:
	case fm_WriteRead:
		return true;
		break;
	}
	return false;
}

bool TMount::IsReadableRequired(FileMode mode)
{
	switch(mode)
	{
	case fm_Read:
		return true;
		break;

	case fm_ReadWrite:
	case fm_AppendRead:
	case fm_WriteRead:
		return true;
		break;
	}
	return false;
}

void TMount::RequiredPermissions( FileMode mode, bool& OUT pWrite,bool& OUT pRead )
{
	switch(mode)
	{
	case fm_Read:
		pRead = true;
		break;

	case fm_Append:
	case fm_Write:
		pWrite = true;
		break;

	case fm_ReadWrite:
	case fm_AppendRead:
	case fm_WriteRead:
		pRead = true;
		pWrite = true;
		break;
	}
}



bool TMount::HasPermission( FileMode mode )
{
	bool reqReadable = false;
	bool reqWritable = false;
	bool result = true;

	RequiredPermissions(mode,reqWritable,reqReadable);

	if (reqReadable)
	{
		if ( (PermissionFlags & Readable) == 0)
		{
			result = false;
		}
	}

	if (reqWritable)
	{
		if ( (PermissionFlags & Writeable) == 0)
		{
			return false;
		}
	}
	return result;
}

