#ifndef GTIMER_H
#define GTIMER_H

#include "gobject.h"

/**
 * Time caused effect.
 */
class GTimeEffect
{
protected:
	ui32* ReadReference;
	bool RealTime;

public:
	
	inline bool get_RealTime() { return RealTime; }

	void set_RealTime(bool value);

	GTimeEffect(bool _realTime = false)
	{
		set_RealTime(_realTime);
	}

	virtual void UpdateTimeEffect() = 0;
};


/**
 * It starts repeating after activation delay passed.
 */
class GTimeEffectActivation: public GTimeEffect
{
public:
	ui32 ActivationDelay;
	ui32 RepeatDelay;

	ui32 LastCheck;
	bool TimeEffectEnabled;
	bool Activated;
	bool InitialPulse;

	GTimeEffectActivation(bool _realTime = false): GTimeEffect(_realTime)
	{
		InitialPulse = true;
		TimeEffectEnabled = false;
	}

	inline void InitializeTimeEffect(ui32 _activationDelay, ui32 _repeatDelay)
	{
		ActivationDelay = _activationDelay;
		RepeatDelay = _repeatDelay;
	}

	inline void EnableTimeEffect()
	{
		LastCheck = *ReadReference;
		TimeEffectEnabled = true;
		Activated = false;
		if (InitialPulse)
		{
			Pulse();
		}
	}

	inline void DisableTimeEffect()
	{
		TimeEffectEnabled = false;
		Activated = false;
	}

	inline void UpdateTimeEffect()
	{
		if (!TimeEffectEnabled) return;

		int timeDifference = *ReadReference - LastCheck;

		if (Activated)
		{ 
			// REPEAT STAGE
			if (timeDifference > RepeatDelay)
			{
				LastCheck = *ReadReference;
				Pulse();
				Activated = true;
			}
		}
		else
		{
			// ACTIVATION STAGE
			if (timeDifference > ActivationDelay)
			{
				LastCheck = *ReadReference;
				Pulse();
				Activated = true;
			}
		}
	}

	virtual void Pulse() = 0;
};

class GTimeEffectBool: public GTimeEffect
{
public:
	ui32 Delay;
	ui32 LastCheck;

	/**
	 * True while it works.
	 */
	bool Value;

	inline void SetHertz(ui32 hertz)
	{
		Delay = 1000 / hertz;
		if (Delay <= 0)
			Delay = 1;
	}

	GTimeEffectBool() 
	{
		SetHertz(1);
	}

	GTimeEffectBool(ui32 hertz, bool _realTime = false): GTimeEffect(_realTime)
	{
		SetHertz(hertz);
		Value = false;
	}

	inline void ResetValue(bool val)
	{
		Value = val;
		LastCheck = *ReadReference;
	}

	inline void UpdateTimeEffect()
	{
		if (*ReadReference - LastCheck >= Delay)
		{
			LastCheck = *ReadReference;
			Value = !Value;
		}
	}
};

// TODO: make this derived from GTimeEffect though..
class GTimer: public GObject // I don't know why I derived this from GObject?
{
private:

public:
	bool RealTime;
	ui32 Delay;
	ui32 LastCheck;

	GTimer();

	event<NoArgEvent> Elapsed;

	void Render() {};
	void Update();

	void SetLastCheck();
	ui32 GetDiff();
};



#endif