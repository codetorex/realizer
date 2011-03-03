#ifndef VVERTEXBUFFER_H
#define VVERTEXBUFFER_H

#include "vvertexbufferformat.h"


// TODO: make these guys enum



class VVertexBuffer: public TFlexibleBuffer
{
public:
	bool	Locked;
	rvbf	BufferObject;
	int		MeshType;
	dword	PrimitiveCount;

	VVertexBuffer(VVertexBufferFormat* _format, int _capacity,int _meshType)
	{
		Capacity = _capacity;
		BufferFormat = _format;
		MeshType = _meshType;
		PrimitiveCount = 0;

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
	void DeleteVertexBuffer();

	/**
	* Makes GPU memory writable.
	*/
	void LockBuffer();

	void Render();

	/**
	* Commits writes and unlocks buffer makes it usable for rendering.
	*/
	void UnlockBuffer();

	inline int GetPrimCount() // todo: move this to correct stuff
	{
		switch (MeshType)
		{
		case RL_TRIANGLELIST: return Used/3;
		case RL_TRIANGLESTRIP: return Used-2;
		case RL_POINTLIST: return Used;
		case RL_LINELIST: return Used/2;
		case RL_LINESTRIP: return Used-1;
		case RL_TRIANGLEFAN: return Used-2;
		}
		return -1;
	}

};


#endif