#ifndef VANIMATIONSIMPLE_H
#define VANIMATIONSIMPLE_H

#include "vanimation.h"
#include "tdiagram.h"

/**
 * VAnimationSimpleSet
 * VAnimationSimpleLinear: single algorithm single value
 * VAnimationSimpleCosine
 * VAnimationSimpleBezier
 * VAnimationSimpleCatmullRom
 * VAnimationMultiSet
 * VAnimationMultiLinear
 * VAnimationMultiCosine
 * VAnimationMultiBezier
 * VAnimationMultiCatmullRom: algorithm same but value is multiple
 * VAnimationComplex: frames are same but algorithms are different
 * VAnimationCombination: uses multiple VAnimationSimple's together
 */

/*class VAnimationSimpleSet: public VAnimation
{
public:

};*/

/**
 * There is only one value to be interpolated.
 * Uses diagram interface
 */
class VAnimationSimpleLinear: public VAnimation
{
public:
	TDiagramOutput<float> Output;

	VAnimationSimpleLinear()
	{
		BytePerFrame = VAnimationKeyFrame::SizeWithoutValue + sizeof(float);
	}

	void AdvanceAnimation()
	{
		VAnimationKeyFrame* curKeyFrame = GetCurrentKeyFrame();
		VAnimationKeyFrame* nextKeyFrame = GetNextKeyFrame();

		float mu = (CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);

		float y1 = *(float*)&curKeyFrame->Value[0];
		float y2 = nextKeyFrame->Value[0];
		float result = MathInterpolate::LinearInterpolate(y1,y2,mu);

		Output.Set(result);
	}
};

class VAnimationMultiLinear: public VAnimation
{
private:
	ui32 ValueCount;

public:
	TArray< TDiagramOutput<float>* > Outputs;

	VAnimationMultiLinear()
	{
		BytePerFrame = 0;
	}

	VAnimationMultiLinear(ui32 valueCount)
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
		BytePerFrame = VAnimationKeyFrame::SizeWithoutValue + (sizeof(float) * ValueCount);
		InitializeBuffer();

		for (int i=0;i<p_ValueCount;i++)
		{
			Outputs.Add( new TDiagramOutput<float>() );
		}
	}

	void AdvanceAnimation()
	{
		VAnimationKeyFrame* curKeyFrame = GetCurrentKeyFrame();
		VAnimationKeyFrame* nextKeyFrame = GetNextKeyFrame();

		float mu = (CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);
		
		for (ui32 i=0;i<ValueCount;i++)
		{
			float y1 = curKeyFrame->GetFloatValue(i);
			float y2 = nextKeyFrame->GetFloatValue(i);

			Outputs[i]->Set( MathInterpolate::LinearInterpolate(y1,y2,mu) );
		}
	}

	inline float GetValue(int outputPort)
	{
		return Outputs.Item[outputPort]->Value;
	}
};


#endif