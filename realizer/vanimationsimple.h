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
 * VAnimationCombination: uses multiple VAnimationSimple's together ( DECORATOR PATTERN )
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

	float GetValueAtTime(float time, int& keyFrame,int port)
	{

	}
};



#endif