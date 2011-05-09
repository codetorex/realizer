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
		dataPtr = Data;
		Data = 0;
		Free();
	}

	CapacityItem = cap;
	Capacity = BufferFormat->BytesPerItem * CapacityItem;
	Engine.Renderer.CreateVertexBuffer(this,Capacity);
	

	if (dataPtr != 0)
	{
		MemoryDriver::Copy(Data,dataPtr, oldUsed * BufferFormat->BytesPerItem);
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
	BufferObject = 0;
}



void VVertexBuffer::LockBuffer(int offset,int length)
{
	if (BufferObject == 0)
		throw Exception("Not created");

	Engine.Renderer.LockVertexBuffer(this,offset,length);
	Index = 0;
	Used = 0;
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