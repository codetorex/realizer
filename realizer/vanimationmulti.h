#ifndef VANIMATIONMULTI_H
#define VANIMATIONMULTI_H

#include "vanimation.h"
#include "tdiagram.h"

class VAnimationMulti: public VAnimation
{
private:
	virtual int CalculateBytesPerFrame(int valueCount) = 0;

public:
	ui32 ValueCount;
	Array< TDiagramOutput<float>* > Outputs;

	typedef delegate1<void,VAnimationMulti*> AnimationEvent;
	event< AnimationEvent > Updated;

	VAnimationMulti()
	{
		BytePerFrame = 0;
	}

	VAnimationMulti(ui32 valueCount)
	{
		BytePerFrame = 0;
		set_ValueCount(valueCount);
	}

	inline ui32 get_ValueCount()
	{
		return ValueCount;
	}

	inline void set_ValueCount(ui32 p_ValueCount )
	{
		ValueCount = p_ValueCount;
		BytePerFrame = CalculateBytesPerFrame(p_ValueCount); //
		InitializeBuffer();

		for (ui32 i=0;i<p_ValueCount;i++)
		{
			Outputs.Add( new TDiagramOutput<float>() );
		}
	}

	virtual void AdvanceAnimation() = 0;

	inline float GetValue(int outputPort)
	{
		return Outputs.Items[outputPort]->Value;
	}
};

class VAnimationMultiLinear: public VAnimationMulti
{
private:
	int CalculateBytesPerFrame(int valueCount)
	{
		return VAnimationKeyFrame::SizeWithoutValue + (sizeof(float) * ValueCount);
	}

public:

	void AdvanceAnimation()
	{
		VAnimationKeyFrame* curKeyFrame = GetCurrentKeyFrame();
		VAnimationKeyFrame* nextKeyFrame = GetNextKeyFrame();

		float mu = (CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);

		for (ui32 i=0;i<ValueCount;i++)
		{
			float y1 = curKeyFrame->GetFloatValue(i);
			float y2 = nextKeyFrame->GetFloatValue(i);

			Outputs[i]->Set( Math.Interpolate.Linear(y1,y2,mu) );
		}
	}
};

class VAnimationMultiCosine: public VAnimationMulti
{
private:
	int CalculateBytesPerFrame(int valueCount)
	{
		return VAnimationKeyFrame::SizeWithoutValue + (sizeof(float) * ValueCount);
	}

public:
	void AdvanceAnimation()
	{
		VAnimationKeyFrame* curKeyFrame = GetCurrentKeyFrame();
		VAnimationKeyFrame* nextKeyFrame = GetNextKeyFrame();

		float mu = (CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);

		for (ui32 i=0;i<ValueCount;i++)
		{
			float y1 = curKeyFrame->GetFloatValue(i);
			float y2 = nextKeyFrame->GetFloatValue(i);

			Outputs[i]->Set( Math.Interpolate.Cosine(y1,y2,mu) );
		}
	}
};


#endif