#ifndef VVERTEXBUFFER_H
#define VVERTEXBUFFER_H

#include "realizertypes.h"
#include "tcomposition.h"

// TODO: make these guys enum


/**
 * Why we dont make this template?
 * and define some structs to hold the data
 * and use structs as template parameter?
 * so we can vertices.Add(new VertexPositionColor(new Vector3(x, y, 0),Color.Green));
 */

class VVertexBuffer
{
public:
	/// True when buffer is locked ( able to be written )
	bool	Locked;

	/// Buffer object pointer
	rvbf	BufferObject;

	/// Byte capacity of buffer
	ui32	Capacity;

	/// Byte count of buffer used
	ui32	Length;

	/// Pointer to data
	void*	Data;

	/// Length of a single item
	ui32	ItemLength;

	/// Not necessary but useful
	TBufferFormat* BufferFormat;

	VVertexBuffer()
	{
		Data = 0;
		Length = 0;
		Capacity = 0;
		BufferObject = 0;
		Locked = false;
	}

	~VVertexBuffer()
	{
		if (BufferObject)
		{
			if (Locked)
			{
				UnlockBuffer();
			}
			DeleteBuffer();
			Data = 0;
		}
	}

	inline ui32 GetItemCount()
	{
		return (Length / ItemLength);
	}

	/**
	 * Allocates memory on GPU (Vertex Buffer Object)
	 * @param newCapacity
	 */
	void CreateBuffer(ui32 newCapacity);

	/**
	 * Deallocates memory on GPU
	 */
	void DeleteBuffer();

	/**
	 * Makes GPU memory writable
	 */
	void LockBuffer(int offset,int length);
	
	inline void LockBuffer()
	{
		LockBuffer(0,Capacity);
	}

	/**
	 * Commits writes and unlocks buffer makes it usable for rendering.
	 */
	void UnlockBuffer();
};


/**
 * This class is for defined vertex data
 */
template <class T>
class VVertexBufferDefined: public VVertexBuffer
{
public:
	VVertexBufferDefined()
	{
		BufferFormat = 0;
		ItemLength = sizeof(T);
	}

	/**
	 * Returns byte length for defined object count
	 */
	inline ui32 GetByteLength( ui32 objCount)
	{
		return objCount * sizeof(T);
	}

	/**
	 * Calculates bytes necessary for definition and creates buffer accordingly
	 */
	inline void CreateDefinedBuffer(ui32 objCapacity)
	{
		ui32 byteCap = GetByteLength(objCapacity);
		CreateBuffer(byteCap);
	}

	inline void Add(const T& value)
	{
		void* dst = &Data[Length];
		int typeLength = sizeof(T);
		MemoryDriver::Copy(dst,&value,typeLength);
		Length += typeLength;
	}
};

#endif