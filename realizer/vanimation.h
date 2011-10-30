#ifndef VANIMATOR_H
#define VANIMATOR_H

#include "vnode.h"
#include "mmathdriver.h"
#include "tbuffer.h"
#include "tenumerator.h"

enum AnimationDataType
{
	ADT_NOWB, // no write back
	ADT_FLOAT,
	ADT_INT,
	ADT_BYTE,
};

enum VAnimationAlgorithms
{
	AA_SET,
	AA_LINEAR,
	AA_COSINE,
};

class VAnimation;

class VAnimationAlgorithm
{
public:
	int BytesPerKeyFrame;

	static VAnimationAlgorithm* GetAlgorithm( VAnimationAlgorithms which );

	virtual void AdvanceAnimation( VAnimation& animation) = 0;
};

class VAnimationAlgorithmSet: public VAnimationAlgorithm
{
public:
	static VAnimationAlgorithmSet Instance;

	VAnimationAlgorithmSet()
	{
		BytesPerKeyFrame = sizeof(float);
	}

	void AdvanceAnimation( VAnimation& animation );
};

class VAnimationAlgorithmLinear: public VAnimationAlgorithm
{
public:
	VAnimationAlgorithmLinear()
	{
		BytesPerKeyFrame = sizeof(float);
	}

	static VAnimationAlgorithmLinear Instance;
	void AdvanceAnimation( VAnimation& animation );
};

class VAnimationAlgorithmCosine: public VAnimationAlgorithm
{
public:
	VAnimationAlgorithmCosine()
	{
		BytesPerKeyFrame = sizeof(float);
	}

	static VAnimationAlgorithmCosine Instance;
	void AdvanceAnimation( VAnimation& animation );
};

enum AnimationStatus
{
	AS_NOTSTARTED,
	AS_RUNNING,
	AS_ENDED,
};

class VAnimationKeyFrame
{
public:
	float	TimeRef;
	ui32	Frame;
	byte	Value[1]; // unlimited number of complexities will be hidden behind this value

	inline float GetFloatValue(int index)
	{
		return ((float*)&Value[0])[index];
	}

	static const ui32 SizeWithoutValue;
};

/**
 * Simple interpolation managing class.
 */
class VAnimation
{
public:
	friend class VAnimationKeyFrameEnumerator;

	ui32					FrameCount;
	float					FramesPerSecond;
	float					CurrentTime;
	bool					Loop;
	AnimationStatus			Status;
	ui32					CurrentFrameIndex;
	ui32					BytePerFrame;
	TByteArray				Buffer;

	inline void CheckCapacity()
	{
		if ( (FrameCount/*+1*/) >= (Buffer.Capacity / BytePerFrame)) // that +1 artifact is from animation 2.0, since last frame was result frame at the time
		{
			Buffer.Grow(Buffer.Capacity * 2);
		}
	}

	inline float get_FramesPerSecond()
	{
		return FramesPerSecond;
	}

	inline void set_FramesPerSecond(float value)
	{
		FramesPerSecond = value;
		UpdateTimeReferences();
	}

	VAnimation()
	{
		FrameCount = 0;
		FramesPerSecond = 30.0f;
		CurrentTime= 0.0f;
		CurrentFrameIndex = 0;
		Status = AS_NOTSTARTED;
		Loop = false;
	}

	void InitializeBuffer( ui32 _frameCapacity = 8 );


	/**
	 * Get frame pointer by id.
	 */
	inline VAnimationKeyFrame* GetFrame(ui32 id)
	{
		return (VAnimationKeyFrame*)(Buffer.Data + (id * BytePerFrame));
	}

	/**
	 * Get last frame data.
	 */
	inline VAnimationKeyFrame* GetLastKeyFrame()
	{
		return (VAnimationKeyFrame*)(Buffer.Data + (Buffer.Capacity - BytePerFrame));
	}

	/**
	 * Updates time references.
	 * This is necessary when you changed frame number of an item or when added or removed frames.
	 */
	void UpdateTimeReferences();



	inline VAnimationKeyFrame* GetCurrentKeyFrame()
	{
		return GetFrame(CurrentFrameIndex);
	}

	inline VAnimationKeyFrame* GetNextKeyFrame()
	{
		return GetFrame(CurrentFrameIndex+1);
	}

	inline void Rewind()
	{
		CurrentTime = 0.0f;
		CurrentFrameIndex = 0;
		Status = AS_NOTSTARTED;
	}

	void AdvanceTime(float time);

	/**
	 * Interpolate key frames
	 */
	virtual void AdvanceAnimation() = 0;
};



class VAnimationKeyFrameEnumerator: public TEnumerator< VAnimationKeyFrame* >
{
private:
	VAnimation* curAnim;
	inline void UpdateCurrentPtr(ui32 idx)
	{
		Current = (VAnimationKeyFrame*)(curAnim->Buffer.Data + (curAnim->BytePerFrame * idx));
	}

	inline void UpdateCurrentPtr()
	{
		UpdateCurrentPtr( FrameIndex );
	}

public:

	int FrameIndex;
	int EndIndex;

	/**
	 * Constructor for enumerator.
	 * @param startIndex zero based index for starting frame id.
	 * @param endIndex zero based index for ending enumeration, if -1 then goes until last frame.
	 */
	VAnimationKeyFrameEnumerator( VAnimation* anim, int startIndex = 0, int endIndex = -1 )
	{
		curAnim = anim;
		StartFrom(startIndex);
		if (endIndex == -1)
		{
			EndIndex = curAnim->FrameCount-1;
		}
		else
		{
			EndIndex = endIndex;
		}
	}

	inline void Reset()
	{
		StartFrom(0);
	}

	inline void StartFrom(int index)
	{
		index--;
		FrameIndex = index;
		UpdateCurrentPtr();
	}

	inline bool MoveNext()
	{
		if (FrameIndex != EndIndex)
		{
			Current = (VAnimationKeyFrame*)((byte*)Current + curAnim->BytePerFrame);
			FrameIndex++;
			return true;
		}
		return false;
	}
};

template <int sz>
class VAnimationIntWriteBack: public VAnimation
{
public:
	int* WriteBackPointers[sz];

	VAnimationIntWriteBack()
	{
		//Setup();
	}

	inline void Setup(VAnimationAlgorithm* pAlgorithm, int _bufferSize = 8, ...)
	{
		Algorithm = pAlgorithm;
		InitializeBuffer(sz,_bufferSize);

		va_list ap;
		va_start(ap,_bufferSize);
		for (int i=0;i<sz;i++)
		{
			WriteBackPointers[i] = va_arg(ap, int* );
		}
		va_end(ap);
	}

	void ValuesChanged()
	{
		for (int i =0;i<sz;i++)
		{
			*WriteBackPointers[i] = (int)CurrentValueFrame->Value[i];
		}
	}
};

template <int sz>
class VAnimationByteWriteBack: public VAnimation
{
public:
	int* WriteBackPointers[sz];

	VAnimationByteWriteBack()
	{
		//Setup();
	}

	inline void Setup(VAnimationAlgorithm* pAlgorithm, int _bufferSize = 8, ...)
	{
		Algorithm = pAlgorithm;
		InitializeBuffer(sz,_bufferSize);

		va_list ap;
		va_start(ap,_bufferSize);
		for (int i=0;i<sz;i++)
		{
			WriteBackPointers[i] = va_arg(ap, byte* );
		}
		va_end(ap);
	}

	void ValuesChanged()
	{
		for (int i =0;i<sz;i++)
		{
			*WriteBackPointers[i] = (byte)MathDriver::Clamp(0.0f,255.0f,CurrentValueFrame->Value[i]);
		}
	}
};

typedef VAnimationByteWriteBack<4> VAnimationColor;

/**
 * Composite manager for VAnimation.
 */
class VAnimationManager
{
public:

};

#endif