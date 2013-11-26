#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include "tarray.h"
#include "tqueue.h"
#include "cresource.h"

class String;

class VTexture;

class CResourceCommand
{
public:
	enum ResourceCommands
	{
		LOAD,
		UNLOAD,
		GARBAGE_COLLECT,
	};

	enum ResourceCommandStatus
	{
		PENDING,
		PROCESSING,
		DONE,
	};

	ui32 ResourceType;
	ui16 Command;
	ui16 Status;
	
	// Parameters
	union
	{
		struct TextureParameters
		{
			String* Path;
		};
	};
};


/**
 * Loads resources in its thread so rendering wont stops?
 * Good for tracking resources.
 */
class CResourceManager: Array< Resource* >
{
public:
	ui32 TotalLoaded;
	ui32 TotalFreed;

	ui32 MemoryUsed;
	ui32 MemoryFreed;

	ui32 BytesPerSecond; // for measuring up loading speed

	/// Queue holds the pending loading operations
	TQueue< CResourceCommand* > ResourceQueue;

	void LoadTexture( const String& path, VTexture*& targetPtr, Resource* parent = 0 );

	inline void QueueLoad(CResourceCommand* cmd)
	{
		ResourceQueue.Enqueue(cmd);
	}


};

/**
 * When loading resources:
 * 1. Check if resource is already loaded.
 * 2. If loaded then AcquireResource and increase reference count.
 * 3. Attach resource to a parent resource.
 * 
 * When unloading them:
 * 1. Decrease reference count.
 * 2. If its 0 then unload all childs first.
 * 3. Then unload self.
 */

#endif