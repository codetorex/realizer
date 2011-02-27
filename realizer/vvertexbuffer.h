#ifndef VVERTEXBUFFER_H
#define VVERTEXBUFFER_H


#include "cengine.h"
#include "vvertexbufferformat.h"

class VVertexBuffer: public TFlexibleBuffer
{
public:
	bool Locked;
	rvbf BufferObject;

	VVertexBuffer(VVertexBufferFormat* _format, int _capacity)
	{
		Capacity = _capacity;
		BufferFormat = _format;

		BufferObject = 0;
		Locked = false;
	}

	/**
	* Allocates memory on GPU.
	*/
	void CreateVertexBuffer(int cap);

	/**
	* Deallocates memory on GPU.
	*/
	void DeleteVertexBuffer()
	{
		if (BufferObject == 0)
			throw Exception("Not created");

		if (Locked)
		{
			UnlockBuffer();
		}

		Engine.Renderer.DeleteVertexBuffer(this);
	}

	/**
	* Makes GPU memory writable.
	*/
	inline void LockBuffer()
	{
		if (BufferObject == 0)
			throw Exception("Not created");

		Engine.Renderer.LockVertexBuffer(this,0,Capacity);
		Locked = true;
	}

	/**
	* Commits writes and unlocks buffer makes it usable for rendering.
	*/
	inline void UnlockBuffer()
	{
		if (BufferObject == 0)
			throw Exception("Not created");

		Engine.Renderer.UnlockVertexBuffer(this);
		Locked = false;
	}

};


#endif