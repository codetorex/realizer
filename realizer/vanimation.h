#ifndef VANIMATOR_H
#define VANIMATOR_H

#include "vnode.h"
#include "mmathdriver.h"

enum AnimationDataType
{
	ADT_NOWB, // no write back
	ADT_FLOAT,
	ADT_INT,
	ADT_BYTE,
};

enum AnimationAlgorithm
{
	AA_LINEAR,
	AA_COSINE,
	AA_CUBIC,
	AA_HERMITE,
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
	int Frame;
	float TimeRef; // frame in seconds
	float Value;
};

/**
 * Simple interpolation managing class.
 */
class VAnimation
{
private:
	float FramesPerSecond;

public:
	AnimationDataType WriteBackDataType;
	AnimationAlgorithm Algorithm;
	AnimationStatus Status;
	TArray< VAnimationKeyFrame > KeyFrames;
	bool Loop;

	float CurrentTime;
	float CurrentValue;
	int CurrentFrameIndex;
	int LastFrameIndex;

	void* WriteBackPtr;

	VAnimation()
	{

	}

	VAnimation(AnimationAlgorithm _algorithm, AnimationDataType _wbDataType = ADT_NOWB, void* _wbPtr = 0,bool _loop = false)
	{
		Set(_algorithm,_wbDataType,_wbPtr,_loop);
	}

	void Set(AnimationAlgorithm _algorithm, AnimationDataType _wbDataType = ADT_NOWB, void* _wbPtr = 0,bool _loop = false)
	{
		WriteBackDataType = _wbDataType;
		WriteBackPtr = _wbPtr;

		Algorithm = _algorithm;
		Loop = _loop;
		FramesPerSecond = 30.0f;
		CurrentTime= 0.0f;
		CurrentFrameIndex = 0;
		Status = AS_NOTSTARTED;
	}

	void AddKeyFrame(int frame, float value)
	{
		VAnimationKeyFrame newKeyFrame;
		newKeyFrame.Frame = frame;
		newKeyFrame.Value = value;
		KeyFrames.Add(newKeyFrame);
	}

	inline void WriteBackFloat()
	{
		*(float*)WriteBackPtr = CurrentValue;
	}

	inline void WriteBackInt()
	{
		*(int*)WriteBackPtr = (int)CurrentValue;
	}

	inline void WriteBackByte()
	{
		*(byte*)WriteBackPtr = (byte)MathDriver::Clamp(0,255,CurrentValue);
	}

	inline void WriteBack()
	{
		switch(WriteBackDataType)
		{
		case ADT_NOWB:
			return;

		case ADT_FLOAT:
			WriteBackFloat();
			return;

		case ADT_INT:
			WriteBackInt();
			return;

		case ADT_BYTE:
			WriteBackByte();
			return;
		}
	}

	void Rewind()
	{
		CurrentTime = 0.0f;
		CurrentFrameIndex = 0;
		Status = AS_NOTSTARTED;
	}

	/**
	 * Cumbersome unoptimized implementation for test purposes only.
	 */
	void AdvanceTime(float time)
	{
		if (Status == AS_ENDED) return;

		CurrentTime += time;
		int currentKey = -1;
		for (int i=CurrentFrameIndex;i<KeyFrames.Count;i++)
		{
			VAnimationKeyFrame& curKeyFrame = KeyFrames.Item[i];
			if (CurrentTime >= curKeyFrame.TimeRef)
			{
				currentKey = i;
				continue;
			}
			break;
		}

		if (currentKey == -1) // animation for this object not started yet
		{
			Status = AS_NOTSTARTED;
			return;
		}

		// this is point -1
		
		if (currentKey >= KeyFrames.Count-1) // animation for this object is ended
		{
			if (Status != AS_ENDED)
			{
				CurrentValue = KeyFrames.Item[KeyFrames.Count-1].Value;
				CurrentTime = KeyFrames.Item[KeyFrames.Count-1].TimeRef;
				WriteBack();
			}
			Status = AS_ENDED;
			return;
		}

		Status = AS_RUNNING;

		CurrentFrameIndex = currentKey; // can be in point -1

		VAnimationKeyFrame& curKeyFrame = KeyFrames.Item[currentKey];
		VAnimationKeyFrame& nextKeyFrame = KeyFrames.Item[currentKey+1];

		float curValue = curKeyFrame.Value;
		float nextValue = nextKeyFrame.Value;
		float tween = (CurrentTime - curKeyFrame.TimeRef) / (nextKeyFrame.TimeRef - curKeyFrame.TimeRef);

		switch (Algorithm)
		{
		case AA_LINEAR:
			CurrentValue = MathInterpolate::LinearInterpolate(curValue,nextValue,tween);
			break;

		case AA_COSINE:
			CurrentValue = MathInterpolate::CosineInterpolate(CurrentValue,nextValue,tween);
			break;

		case AA_CUBIC:
		case AA_HERMITE:
			throw 0;
		}
		
		WriteBack();
	}

	inline float get_FramesPerSecond()
	{
		return FramesPerSecond;
	}

	void set_FramesPerSecond(float value)
	{
		FramesPerSecond = value;
		UpdateTimeReferences();
	}

	void UpdateTimeReferences()
	{
		float frameRelation = 1 / FramesPerSecond;
		for (int i=0;i<KeyFrames.Count;i++)
		{
			VAnimationKeyFrame& curKeyFrame = KeyFrames.Item[i];
			curKeyFrame.TimeRef = frameRelation * (float)curKeyFrame.Frame;
		}
	}
};

class VAnimationManager
{
public:

};

#endif