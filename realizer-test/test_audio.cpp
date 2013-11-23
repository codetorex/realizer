#include "stdafx.h"
#include "test_audio.h"
#include "gschemedskin.h"
#include "tbitmapcodecs.h"
#include "vtexture.h"

RTestAudio TestAudio;

void RTestAudio::Initialize()
{
	raise::Sound::AudioFormat* fmt = new raise::Sound::AudioFormat();
	fmt->SampleRate = 44100;
	fmt->Format = 1;
	fmt->BitsPerSample = 16;

	raise::Sound::Oscillator* osc = new raise::Sound::SquareOscillator();
	osc->Format = fmt;
	osc->SetFrequency(440);


	TestDesktop = Engine.GUI.CreateDesktop(false);
	TestSkin = (GSchemedSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");
	OnActivated();
	Wfw = new RWaveformWindow(osc);
	Wfw->SetRectangle(100,100,300,200);
	TestDesktop->AddChild(Wfw);
	Wfw->UpdateLines();

	ZoomOutButton = new GButton();
	ZoomOutButton->SetRectangle(450,100,100,30);
	ZoomOutButton->Text = "Zoom Out";
	ZoomOutButton->Click += GetHandler(this, &RTestAudio::ZoomOutButton_Click);

	ZoomInButton = new GButton();
	ZoomInButton->SetRectangle(450,140,100,30);
	ZoomInButton->Text = "Zoom In";
	ZoomInButton->Click += GetHandler(this, &RTestAudio::ZoomInButton_Click);

	TestDesktop->AddChild(ZoomOutButton);
	TestDesktop->AddChild(ZoomInButton);

	Engine.GUI.Desktop->Layout();

}

void RTestAudio::Render()
{
	Engine.Renderer.Enter2D();

	Engine.GUI.Render();

	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}

void RTestAudio::ZoomInButton_Click()
{
	Wfw->SetZoom( Wfw->Zoom * 1.1f );
}

void RTestAudio::ZoomOutButton_Click()
{
	Wfw->SetZoom( Wfw->Zoom / 1.1f );
}

void RWaveformWindow::Render()
{
	//Skin->RenderSunkEdge(this);

	if (Lines.Count < 2)
	{
		return;
	}

	TStringBuilderStack<128> sb;
	sb.Append(sff(Zoom));

	TCharacterEnumerator ce(sb);
	Skin->SystemFont->RenderText(ce, this->DrawRegion.X() + 10.0f , this->DrawRegion.Y() + 10.0f, TColor32::Black);

	Engine.Draw.NoTexture();
	
	Engine.Draw.DrawRectangle(this->DrawRegion, TColor32::LimeGreen);

	Engine.Draw.Translate(this->DrawRegion.X(), this->DrawRegion.Y(), 0.0f);


	Engine.Draw.DrawLine(0,Height / 2, Width, Height/2, TColor32::Aqua);
	for (int i=0;i<Lines.Count-1;i++)
	{
		Point2& p = Lines[i];
		Point2& n = Lines[i+1];
		Engine.Draw.DrawLine(p.X,p.Y,n.X,n.Y, TColor32::LightGreen);
	}

	Engine.Draw.ResetTransform();
}

void RWaveformWindow::SetPosition( ui32 newPosition )
{
	this->Position = newPosition;
	UpdateLines();
}

void RWaveformWindow::UpdateLines()
{
	Lines.Count = 0; // fast clear

	float myWidth = (float)this->Width;
	float sampleRate = (float)Sampler->Format->SampleRate;
	float sampleStep = (sampleRate / Zoom) / myWidth;

	int pixelStep = 1;
	if (sampleStep < 1.0f)
	{
		pixelStep = (1.0f / sampleStep);
		sampleStep = 1.0f;
	}

	int sampleIndex = Position;

	float halfHeight = (float)this->Height / 2.0f;
	float offset = halfHeight;
	halfHeight /= 2.0f;

	Point2 currentPoint;
	for (int x = 0;x < this->Width;x+=pixelStep)
	{
		float y;
		if (sampleStep == 1)
		{
			y = (Sampler->GetSample(sampleIndex) * halfHeight) + offset;
		}
		else
		{
			y = (Sampler->GetAverage(sampleIndex, sampleIndex + sampleStep) * halfHeight) + offset;
		}
		 
		sampleIndex += sampleStep;
		currentPoint.X = x;
		currentPoint.Y = y;
		Lines.Add(currentPoint);
	}
}

void RWaveformWindow::SetZoom( float newZoom )
{
	this->Zoom = newZoom;
	UpdateLines();
}

void RTestAudio::OnActivated()
{
	Engine.GUI.EnableGUI(TestSkin,TestDesktop);
}

void RTestAudio::OnDeactivated()
{
	Engine.GUI.DisableGUI();
}

