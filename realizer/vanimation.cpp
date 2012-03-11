#include "stdafx.h"
#include "vanimation.h"

const ui32 VAnimationKeyFrame::SizeWithoutValue = sizeof(float) + sizeof(ui32);

/*VAnimationAlgorithmLinear VAnimationAlgorithmLinear::Instance;
VAnimationAlgorithmCosine VAnimationAlgorithmCosine::Instance;
VAnimationAlgorithmSet VAnimationAlgorithmSet::Instance;

void VAnimationAlgorithmLinear::AdvanceAnimation( VAnimation& animation )
{
	VAnimationKeyFrame* curKeyFrame = animation.GetCurrentKeyFrame();
	VAnimationKeyFrame* nextKeyFrame = animation.GetNextKeyFrame();
	VAnimationKeyFrame* resultKeyFrame = animation.CurrentValueFrame;

	float mu = (animation.CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);


	for (ui32 i=0;i<animation.ValueCount;i++)
	{
		float y1 = curKeyFrame->Value[i];
		float y2 = nextKeyFrame->Value[i];

		resultKeyFrame->Value[i] = MathInterpolate::LinearInterpolate(y1,y2,mu);
	}
}


void VAnimationAlgorithmCosine::AdvanceAnimation( VAnimation& animation )
{
	VAnimationKeyFrame* curKeyFrame = animation.GetCurrentKeyFrame();
	VAnimationKeyFrame* nextKeyFrame = animation.GetNextKeyFrame();
	VAnimationKeyFrame* resultKeyFrame = animation.CurrentValueFrame;

	float mu = (animation.CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);


	for (ui32 i=0;i<animation.ValueCount;i++)
	{
		float y1 = curKeyFrame->Value[i];
		float y2 = nextKeyFrame->Value[i];

		resultKeyFrame->Value[i] = MathInterpolate::CosineInterpolate(y1,y2,mu);
	}
}

void VAnimationAlgorithmSet::AdvanceAnimation( VAnimation& animation )
{
	VAnimationKeyFrame* curKeyFrame = animation.GetCurrentKeyFrame();
	VAnimationKeyFrame* resultKeyFrame = animation.CurrentValueFrame;

	for (ui32 i=0;i<animation.ValueCount;i++)
	{
		resultKeyFrame->Value[i] = curKeyFrame->Value[i];
	}
}

VAnimationAlgorithm* VAnimationAlgorithm::GetAlgorithm( KnownImplementations which )
{
	switch( which )
	{
	case AA_SET:
		return &VAnimationAlgorithmSet::Instance;

	case AA_LINEAR:
		return &VAnimationAlgorithmLinear::Instance;

	case AA_COSINE:
		return &VAnimationAlgorithmCosine::Instance;
	}
	return 0;
}*/

void VAnimation::UpdateTimeReferences( )
{
	float frameRelation = 1 / FramesPerSecond;

	VAnimationKeyFrameEnumerator sframes( this );
	while(sframes.MoveNext())
	{
		sframes.Current->TimeRef = frameRelation * (float)(sframes.Current->Frame);
	}
}

/*void VAnimation::GetValueAtTime( float time , int& keyFrame )
{

	int currentKey = GetKeyFrameIndex(time);

	
	int LastFrameZB = FrameCount - 1; // last frame id for zerobased index
	if (currentKey >= LastFrameZB) // animation for this object is ended
	{
		CurrentFrameIndex = FrameCount - 1;
		AdvanceAnimation();
		return;
	}


}*/

void VAnimation::UpdateAnimation()
{
	if (Status == AS_ENDED)
	{
		if (!Loop)
		{
			return;
		}
		else
		{
			Rewind();
		}
	}

	int currentKey = GetKeyFrameIndex(CurrentTime, CurrentFrameIndex);

	if (currentKey == -1) // animation for this object not started yet
	{
		Status = AS_NOTSTARTED;
		return;
	}

	// this is point -1

	int LastFrameZB = FrameCount - 1; // last frame id for zerobased index
	if (currentKey >= LastFrameZB) // animation for this object is ended
	{
		if (Status != AS_ENDED)
		{
			CurrentFrameIndex = FrameCount - 1;
			AdvanceAnimation();
		}
		set_Status(AS_ENDED);
		return;
	}

	set_Status(AS_RUNNING);

	CurrentFrameIndex = currentKey; // can be in point -1

	AdvanceAnimation();
}


void VAnimation::InitializeBuffer( ui32 _frameCapacity /*= 8*/ )
{
	//BytePerFrame = sizeof(VAnimationKeyFrame) + ((ValueCount) * sizeof(float));
	Buffer.InitializeByteArray(BytePerFrame * _frameCapacity);
}

int VAnimation::GetKeyFrameIndex( float time, int startIndex /*= 0*/ )
{
	int currentKey = -1;
	VAnimationKeyFrameEnumerator sframes( this , startIndex);
	while(sframes.MoveNext())
	{
		if (time >= sframes.Current->TimeRef)
		{
			currentKey = sframes.FrameIndex;
			continue;
		}
		break;
	}
	return currentKey;
}
