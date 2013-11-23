#ifndef TEST_AUDIO_H
#define TEST_AUDIO_H

#include "rtestsuite.h"
#include "tsound.h"
#include "gcomponents.h"

class GSchemedSkin;

class RWaveformWindow: public GObject
{
private:
	TArray<Point2> Lines;

public:	
	raise::Sound::Sampler* Sampler;

	/// Current sample position
	ui32 Position;

	/// 1.0f means width = 1 seconds, 2.0f means width = 0.5 seconds, samplerate = 1px per sample
	float Zoom;
	

	RWaveformWindow(raise::Sound::Sampler* sampler)
	{
		this->Sampler = sampler;
		ForeColor = TColor32::Green;
		Position = 0;
		Zoom = 1.0f;
	}

	void SetPosition(ui32 newPosition);

	void SetZoom(float newZoom);

	void UpdateLines();

	void Render();
};

class RTestAudio: public RTestScene
{
public:
	RTestAudio()
	{
		SceneName = "Engine audio test";
	}

	GDesktop* TestDesktop;
	GSchemedSkin* TestSkin;

	RWaveformWindow* Wfw;
	GButton* ZoomInButton;
	GButton* ZoomOutButton;


	void ZoomInButton_Click();
	void ZoomOutButton_Click();

	void Initialize();

	void Render();


	void OnActivated();
	void OnDeactivated();
};

extern RTestAudio TestAudio;


#endif