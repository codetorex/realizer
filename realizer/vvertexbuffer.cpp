#include "stdafx.h"
#include "cengine.h"
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

void VVertexBuffer::DeleteVertexBuffer()
{
	if (BufferObject == 0)
		throw Exception("Not created");

	if (Locked)
	{
		UnlockBuffer();
	}

	Engine.Renderer.DeleteVertexBuffer(this);
}

void VVertexBuffer::LockBuffer()
{
	if (BufferObject == 0)
		throw Exception("Not created");

	Engine.Renderer.LockVertexBuffer(this,0,Capacity);
	Indicator = Buffer;
	Locked = true;
}

void VVertexBuffer::UnlockBuffer()
{
	if (BufferObject == 0)
		throw Exception("Not created");

	Engine.Renderer.UnlockVertexBuffer(this);
	Locked = false;

	PrimitiveCount = GetPrimCount();
}

void VVertexBuffer::Render()
{
	Engine.Renderer.RenderVertexBuffer(this);
}