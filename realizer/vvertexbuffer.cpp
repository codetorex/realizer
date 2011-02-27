#include "stdafx.h"
#include "vvertexbuffer.h"


void VVertexBuffer::CreateVertexBuffer( int cap )
{
	// if there is data in buffer, create buffer and copy data inside it.
	// engine->CrateVertexBuffer(this)
	if (BufferObject != 0)
		throw Exception("Already created");

	byte* dataPtr = 0;
	int oldUsed = Used;

	if (Used > 0)
	{
		dataPtr = Buffer;
		Buffer = 0;
		Free();
	}

	Engine.Renderer.CreateVertexBuffer(this,cap * BufferFormat->BytesPerItem);
	Capacity = cap;

	if (dataPtr != 0)
	{
		MemoryDriver::Copy(Buffer,dataPtr, oldUsed * BufferFormat->BytesPerItem);
		delete [] dataPtr;
	}
}