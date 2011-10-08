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



class VAnimation;

class VAnimationAlgorithm
{
public:

	enum KnownImplementations
	{
		AA_SET,
		AA_LINEAR,
		AA_COSINE,
	};

	static VAnimationAlgorithm* GetAlgorithm( KnownImplementations which );

	virtual void AdvanceAnimation( VAnimation& animation) = 0;
};

class VAnimationAlgorithmSet: public VAnimationAlgorithm
{
public:
	static VAnimationAlgorithmSet Instance;
	void AdvanceAnimation( VAnimation& animation );
};

class VAnimationAlgorithmLinear: public VAnimationAlgorithm
{
public:
	static VAnimationAlgorithmLinear Instance;
	void AdvanceAnimation( VAnimation& animation );
};

class VAnimationAlgorithmCosine: public VAnimationAlgorithm
{
public:
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
	float	Value[1];
};

/**
 * Simple interpolation managing class.
 */
class VAnimation
{
private:
	TByteArray Buffer;
	VAnimationKeyFrame* KeyFrameWritePtr;
	ui32 CurValueIndex;
	ui32 BytePerFrame;

	inline void UpdateCurrentFramePtr()
	{
		CurrentValueFrame = (VAnimationKeyFrame*)(Buffer.Data + (Buffer.Capacity - BytePerFrame));
	}

	inline void CheckCapacity()
	{
		if ( (FrameCount+1) >= (Buffer.Capacity / BytePerFrame))
		{
			Buffer.Grow(Buffer.Capacity * 2);
			UpdateCurrentFramePtr();
		}
	}

	friend class VAnimationKeyFrameEnumerator;

public:

	ui32					ValueCount;
	ui32					FrameCount;
	float					FramesPerSecond;
	float					CurrentTime;
	bool					Loop;
	AnimationStatus			Status;
	VAnimationAlgorithm*	Algorithm;
	VAnimationKeyFrame*		CurrentValueFrame;
	ui32					CurrentFrameIndex;

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
		ValueCount = 0;
		FrameCount = 0;
	}

	VAnimation(ui32 _ValueCount, VAnimationAlgorithm* _algorithm, ui32 _initialBuffer): Algorithm(_algorithm)
	{
		SetupBuffer(_ValueCount, _initialBuffer);
	}

	void SetupBuffer(ui32 _ValueCount, ui32 _initialBuffer = 8);


	/**
	 * Start creating a key frame.
	 */
	void KeyFrameBegin(ui32 frame);

	/**
	 * Set a value of key frame and prepare to write next value.
	 */
	inline void KeyFrameAddValue(float value)
	{
		KeyFrameWritePtr->Value[CurValueIndex++] = value;
	}

	/**
	 * Finalize creating key frame.
	 */
	inline void KeyFrameEnd()
	{
		FrameCount++;
	}

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

	/**
	 * Frames should be sorted before animation can run.
	 * If frames are created as sorted no need to call this function.
	 */
	void SortKeyFrames()
	{
		throw NotImplementedException();
	}

	void AddKeyFrame(ui32 frame, float v0);
	void AddKeyFrame(ui32 frame, float v0,float v1);
	void AddKeyFrame(ui32 frame, float v0,float v1,float v2);
	void AddKeyFrame(ui32 frame, float v0,float v1,float v2,float v3);

	inline void Rewind()
	{
		CurrentTime = 0.0f;
		CurrentFrameIndex = 0;
		Status = AS_NOTSTARTED;
	}

	/**
	 * Cumbersome unoptimized implementation for test purposes only.
	 */
	void AdvanceTime(float time);


	virtual void ValuesChanged() = 0;
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
		SetupBuffer(sz,_bufferSize);

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
		SetupBuffer(sz,_bufferSize);

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