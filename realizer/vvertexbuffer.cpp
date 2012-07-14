#include "stdafx.h"
#include "cengine.h"
#include "vvertexbuffer.h"

void VVertexBuffer::DeleteBuffer()
{
	if (BufferObject == 0)
		throw Exception("Not created");

	if (Locked)
	{
		UnlockBuffer();
	}

	Engine.Renderer.DeleteVertexBuffer(this);
	BufferObject = 0;
	Capacity = 0;
	Length = 0;
	Data = 0;
}

void VVertexBuffer::LockBuffer(int offset,int length)
{
	if (BufferObject == 0)
		throw Exception("Not created");

	Engine.Renderer.LockVertexBuffer(this,offset,length);
	Length = 0;
	Locked = true;
}

void VVertexBuffer::UnlockBuffer()
{
	if (BufferObject == 0)
		throw Exception("Not created");

	Engine.Renderer.UnlockVertexBuffer(this);
	Locked = false;
}

void VVertexBuffer::CreateBuffer( ui32 newCapacity )
{
	Capacity = newCapacity;


	BufferObject = 0;
	Locked = false;

	// if there is data in buffer, create buffer and copy data inside it.
	// engine->CrateVertexBuffer(this)
	if (BufferObject != 0)
		throw Exception("Already created");

	Engine.Renderer.CreateVertexBuffer(this,Capacity);
}