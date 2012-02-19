#ifndef VVERTEXBUFFER_H
#define VVERTEXBUFFER_H

#include "vvertexbufferformat.h"
#include "realizertypes.h"

// TODO: make these guys enum



class VVertexBuffer: public TCompositeBuffer
{
public:
	bool	Locked;
	rvbf	BufferObject;
	int		MeshType;
	ui32	PrimitiveCount;

	VVertexBuffer()
	{

	}

	~VVertexBuffer()
	{
		if (BufferObject)
		{
			if (Locked)
			{
				UnlockBuffer();
			}
			DeleteVertexBuffer();
			Data = 0;
		}
	}

	VVertexBuffer(VVertexBufferFormat* _format, int _capacity,int _meshType,bool makeitReady = false)
	{
		InitializeBuffer(_format,_capacity,_meshType,makeitReady);
	}

	void InitializeBuffer(VVertexBufferFormat* _format, int _capacity,int _meshType,bool makeitReady = false)
	{
		CapacityItem = _capacity;
		BufferFormat = _format;
		Capacity = BufferFormat->BytesPerItem * CapacityItem;
		MeshType = _meshType;
		PrimitiveCount = 0;

		BufferObject = 0;
		Locked = false;

		if (makeitReady)
		{
			CreateVertexBuffer(CapacityItem);
			LockBuffer();
		}
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
	void LockBuffer(int offset,int length);
	
	inline void LockBuffer()
	{
		LockBuffer(0,CapacityItem);
	}
	

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