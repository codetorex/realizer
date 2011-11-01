#ifndef VANIMATIONBUILDER_H
#define VANIMATIONBUILDER_H

#include "vanimation.h"
#include "vanimationsimple.h"
#include "vanimationmulti.h"

class VAnimationBuilder
{
private:
	ui32 BufferIndex;
	ui32 CurrentOutputConnection;
	

	VAnimationKeyFrame* GetKeyFramePointerFromCurrentPosition()
	{
		return (VAnimationKeyFrame*)&Animation->Buffer.Data[BufferIndex];
	}

	void Initialize()
	{
		Animation = 0;
		BufferIndex = 0;
		CurrentOutputConnection = 0;
		AnimationMultiLinear = 0;
	}

public:
	VAnimation* Animation;

	union
	{
		VAnimationMultiLinear* AnimationMultiLinear;
		VAnimationMultiCosine* AnimationMultiCosine;
	};
	

	enum VAnimationType
	{
		VT_SIMPLESET,
		VT_SIMPLELINEAR,
		VT_SIMPLECOSINE,
		VT_SIMPLEBEZIER,
		VT_SIMPLECATMULL,
		VT_MULTISET,
		VT_MULTILINEAR,
		VT_MULTICOSINE,
		VT_MULTIBEZIER,
		VT_MULTICATMULL,
		VT_COMPLEX,
	};

	VAnimationType CurrentAnimationType;

	/**
	 * Uses existing animation for building on it.
	 */
	VAnimationBuilder(VAnimationMultiLinear& animationVar, int valueCount, float fps = 30.0f)
	{
		Initialize();
		CurrentAnimationType = VT_MULTILINEAR;
		AnimationMultiLinear = &animationVar;
		Animation = &animationVar;
		animationVar.set_ValueCount( valueCount );
		animationVar.set_FramesPerSecond( fps );
	}

	/**
	 * Uses existing animation for building on it.
	 */
	VAnimationBuilder(VAnimationMultiCosine& animationVar, int valueCount, float fps = 30.0f)
	{
		Initialize();
		CurrentAnimationType = VT_MULTICOSINE;
		AnimationMultiCosine = &animationVar;
		Animation = &animationVar;
		animationVar.set_ValueCount( valueCount );
		animationVar.set_FramesPerSecond( fps );
	}

	/**
	 * Creates new animation.
	 */
	VAnimationBuilder(VAnimationType AnimType, int ValueCount, float fps = 30.0f)
	{
		Initialize();

		CurrentAnimationType = AnimType;
		switch(AnimType)
		{
		case VT_MULTILINEAR:
			if (!AnimationMultiLinear)
				AnimationMultiLinear = new VAnimationMultiLinear();

			AnimationMultiLinear->set_ValueCount(ValueCount);
			AnimationMultiLinear->set_FramesPerSecond(fps);
			Animation = AnimationMultiLinear;
			break;

		case VT_MULTICOSINE:
			if (!AnimationMultiCosine)
				AnimationMultiCosine = new VAnimationMultiCosine();

			AnimationMultiCosine->set_ValueCount(ValueCount);
			AnimationMultiCosine->set_FramesPerSecond(fps);
			Animation = AnimationMultiCosine;
			break;

		default:
			throw 0;
			return;
		}

	}

	inline void KeyFrameStart(int frame)
	{
		VAnimationKeyFrame* kframe = GetKeyFramePointerFromCurrentPosition();
		kframe->Frame = frame;
		kframe->TimeRef = (float)frame / Animation->FramesPerSecond;
		BufferIndex += VAnimationKeyFrame::SizeWithoutValue;
	}

	inline void KeyFrameSet(float value)
	{
		*(float*)&Animation->Buffer.Data[BufferIndex] = value;
		BufferIndex += sizeof(float);
	}

	inline void KeyFrameEnd()
	{
		Animation->FrameCount++;
	}

	/**
	 * Easy functions to add key frames.
	 */
	void AddKeyFrame(int frame, float v0)
	{
		KeyFrameStart(frame);
		KeyFrameSet(v0);
		KeyFrameEnd();
	}

	void AddKeyFrame(int frame, float v0,float v1)
	{
		KeyFrameStart(frame);
		KeyFrameSet(v0);
		KeyFrameSet(v1);
		KeyFrameEnd();
	}

	void AddKeyFrame(int frame, float v0,float v1,float v2)
	{
		KeyFrameStart(frame);
		KeyFrameSet(v0);
		KeyFrameSet(v1);
		KeyFrameSet(v2);
		KeyFrameEnd();
	}

	void AddKeyFrame(int frame, float v0,float v1,float v2,float v3)
	{
		KeyFrameStart(frame);
		KeyFrameSet(v0);
		KeyFrameSet(v1);
		KeyFrameSet(v2);
		KeyFrameSet(v3);
		KeyFrameEnd();
	}

	inline void UpdateTimeReferences()
	{
		Animation->UpdateTimeReferences();
	}

	/**
	 * You can connect outputs serially to memory pointers
	 */
	void ConnectMemory(int outputPort, float* outputMemPtr)
	{
		switch(CurrentAnimationType)
		{
		case VT_MULTILINEAR:
		case VT_MULTICOSINE:
			((VAnimationMulti*)Animation)->Outputs.Item[outputPort]->ConnectMemory(outputMemPtr);
			break;
		}
	}

	inline void ConnectMemory(float* outputMemPtr)
	{
		ConnectMemory(CurrentOutputConnection++,outputMemPtr);
	}

	void ConnectMemoryConverting(int outputPort,void* targetPtr, TDiagramOutputType targetType)
	{
		switch(CurrentAnimationType)
		{
		case VT_MULTILINEAR:
		case VT_MULTICOSINE:
			((VAnimationMulti*)Animation)->Outputs.Item[outputPort]->ConnectMemoryConverting(targetPtr,targetType);
			return;
		}
	}

	inline void ConnectMemoryConverting(void* targetPtr, TDiagramOutputType targetType)
	{
		ConnectMemoryConverting(CurrentOutputConnection++,targetPtr,targetType);
	}
};

#endif