#include "stdafx.h"
#include "vanimation.h"

VAnimationAlgorithmLinear VAnimationAlgorithmLinear::Instance;
VAnimationAlgorithmCosine VAnimationAlgorithmCosine::Instance;
VAnimationAlgorithmSet VAnimationAlgorithmSet::Instance;

void VAnimationAlgorithmLinear::AdvanceAnimation( VAnimation& animation )
{
	VAnimationKeyFrame* curKeyFrame = animation.GetCurrentKeyFrame();
	VAnimationKeyFrame* nextKeyFrame = animation.GetNextKeyFrame();
	VAnimationKeyFrame* resultKeyFrame = animation.CurrentFrame;

	float mu = (animation.CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);


	for (int i=0;i<animation.ValueCount;i++)
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
	VAnimationKeyFrame* resultKeyFrame = animation.CurrentFrame;

	float mu = (animation.CurrentTime - curKeyFrame->TimeRef) / (nextKeyFrame->TimeRef - curKeyFrame->TimeRef);


	for (int i=0;i<animation.ValueCount;i++)
	{
		float y1 = curKeyFrame->Value[i];
		float y2 = nextKeyFrame->Value[i];

		resultKeyFrame->Value[i] = MathInterpolate::CosineInterpolate(y1,y2,mu);
	}
}

void VAnimationAlgorithmSet::AdvanceAnimation( VAnimation& animation )
{
	VAnimationKeyFrame* curKeyFrame = animation.GetCurrentKeyFrame();
	VAnimationKeyFrame* resultKeyFrame = animation.CurrentFrame;

	for (int i=0;i<animation.ValueCount;i++)
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
}

void VAnimation::UpdateTimeReferences( )
{
	float frameRelation = 1 / FramesPerSecond;

	VAnimationKeyFrameEnumerator sframes( this );
	while(sframes.MoveNext())
	{
		sframes.Current->TimeRef = frameRelation * (float)(sframes.Current->Frame);
	}
}

void VAnimation::AdvanceTime( float time )
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

	CurrentTime += time;
	int currentKey = -1;

	VAnimationKeyFrameEnumerator sframes( this, CurrentFrameIndex );
	while(sframes.MoveNext())
	{
		if (CurrentTime >= sframes.Current->TimeRef)
		{
			currentKey = sframes.FrameIndex;
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

	if (currentKey >= FrameCount) // animation for this object is ended
	{
		if (Status != AS_ENDED)
		{
			CurrentFrameIndex = FrameCount - 1;
			VAnimationAlgorithmSet::Instance.AdvanceAnimation(*this);
			ValuesChanged();
		}
		Status = AS_ENDED;
		return;
	}

	Status = AS_RUNNING;

	CurrentFrameIndex = currentKey; // can be in point -1

	Algorithm->AdvanceAnimation(*this);

	ValuesChanged();
}

void VAnimation::AddKeyFrame( ui32 frame, float v0)
{
	KeyFrameBegin(frame);
	KeyFrameWritePtr->Value[0] = v0;
	KeyFrameEnd();
}

void VAnimation::AddKeyFrame( ui32 frame, float v0,float v1)
{
	KeyFrameBegin(frame);
	KeyFrameWritePtr->Value[0] = v0;
	KeyFrameWritePtr->Value[1] = v1;
	KeyFrameEnd();
}

void VAnimation::AddKeyFrame( ui32 frame, float v0,float v1,float v2 )
{
	KeyFrameBegin(frame);
	KeyFrameWritePtr->Value[0] = v0;
	KeyFrameWritePtr->Value[1] = v1;
	KeyFrameWritePtr->Value[2] = v2;
	KeyFrameEnd();
}

void VAnimation::AddKeyFrame( ui32 frame, float v0,float v1,float v2,float v3 )
{
	KeyFrameBegin(frame);
	KeyFrameWritePtr->Value[0] = v0;
	KeyFrameWritePtr->Value[1] = v1;
	KeyFrameWritePtr->Value[2] = v2;
	KeyFrameWritePtr->Value[3] = v3;
	KeyFrameEnd();
}
void VAnimation::SetupBuffer( ui32 _ValueCount, ui32 _initialBuffer /*= 8*/ )
{
	FrameCount = 0;

	FramesPerSecond = 30.0f;
	CurrentTime= 0.0f;
	CurrentFrameIndex = 0;
	Status = AS_NOTSTARTED;
	Loop = false;

	ValueCount = _ValueCount;
	BytePerFrame = sizeof(VAnimationKeyFrame) + ((ValueCount-1) * sizeof(float));
	Buffer.InitializeByteArray(BytePerFrame * _initialBuffer);
	UpdateCurrentFramePtr();
}

void VAnimation::KeyFrameBegin( ui32 frame )
{
	if (ValueCount == 0)
	{
		throw Exception("Keyframe can't be created before keyframe size is set");
	}

	CheckCapacity();

	KeyFrameWritePtr = (VAnimationKeyFrame*)(Buffer.Data + (BytePerFrame * FrameCount));

	KeyFrameWritePtr->Frame = frame;
	CurValueIndex = 0;
}