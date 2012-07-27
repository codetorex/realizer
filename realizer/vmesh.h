#ifndef VMESH_H
#define VMESH_H

#include "tbinary.h"
#include "vrenderertypes.h"
#include "vvertexbuffer.h"

class VIndexBuffer;

/**
 * Holds array of vertices
 */
class VMesh
{
public:
	/// Set appropriate bits for attributes this mesh has like position, vertex color, normal
	TFlag32			StandartAttributes;

	/// Index buffered mesh
	VIndexBuffer*	Index;

	/// Main vertex buffer, could be just a position or fused data
	VVertexBuffer*	Vertices;

	int				MeshType;
	ui32			PrimitiveCount;

	VMesh()
	{
		Index = 0;
	}

	inline void SetMeshType(int meshType)
	{
		this->MeshType = meshType;
		UpdatePrimitiveCount();
	}

	inline int GetMeshType()
	{
		return MeshType;
	}

	/**
	 * Call this after you changed buffers
	 */
	void UpdatePrimitiveCount()
	{
		if (Index)
		{
			throw 0;
			//PrimitiveCount = Index->Count;
		}
		else
		{
			PrimitiveCount = GetPrimitiveCount(MeshType,Vertices->GetItemCount());
		}
	}

	/**
	 * Calculates primitive count for given vertex count and mesh type.
	 */
	static inline int GetPrimitiveCount(int meshType, ui32 vertexCount) // todo: move this to correct stuff
	{
		switch (meshType)
		{
		case RL_TRIANGLELIST: return vertexCount/3;
		case RL_TRIANGLESTRIP: return vertexCount-2;
		case RL_POINTLIST: return vertexCount;
		case RL_LINELIST: return vertexCount/2;
		case RL_LINESTRIP: return vertexCount-1;
		case RL_TRIANGLEFAN: return vertexCount-2;
		}
		return -1;
	}
};

#endif