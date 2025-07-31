// Copyright (c) Meta Platforms, Inc. and affiliates.

#pragma once

#include "CoreMinimal.h"
#include <OculusXRFunctionLibrary.h>

class PERFORMANCESETTINGS_API FCPUStressTestWorker : public FRunnable, FNoncopyable
{
public:
	FCPUStressTestWorker(int32 InTargetUtilPercent)
		: TargetUtilPercent(InTargetUtilPercent), stopped(false)
	{
		Thread = FRunnableThread::Create(this, TEXT("CPUStressTestThread"));
	}

	~FCPUStressTestWorker()
	{
		Stop();
		if (Thread)
		{
			Thread->WaitForCompletion();
			delete Thread;
			Thread = nullptr;
		}
	}

	virtual bool Init() override
	{
		return true;
	}

	virtual uint32 Run() override
	{
		while (!stopped.load())
		{
			double fps = UOculusXRFunctionLibrary::GetCurrentDisplayFrequency();
			fps = fps > 0 ? fps : 72.0;
			double MsPerFrame = 1000.0 / fps;
			const double WorkMs = (MsPerFrame * TargetUtilPercent.load()) / 100.0; // fetch every cycle so we load updates
			const double SleepMs = MsPerFrame - WorkMs;
			double StartTime = FPlatformTime::Seconds();
			while ((FPlatformTime::Seconds() - StartTime) < WorkMs / 1000.0)
			{
				PerformHeavyWork();
			}

			if (SleepMs > 0)
			{
				FPlatformProcess::Sleep(SleepMs / 1000.0);
			}
		}
		return 0;
	}

	virtual void Stop() override
	{
		stopped.store(true);
	}

	virtual void Exit() override
	{
	}

	void SetTargetUtilPercent(int32 InTargetUtilPercent)
	{
		TargetUtilPercent.store(InTargetUtilPercent);
	}

private:
	void PerformHeavyWork()
	{
		volatile double Dummy = 0;
		for (int32 i = 0; i < 100000; ++i)
		{
			// Simulate physics or AI math (e.g., vector ops, trig, etc.)
			Dummy += FMath::Sin((float)i) * FMath::Cos((float)i);
		}
	}

	std::atomic<bool> stopped;
	FRunnableThread* Thread;
	std::atomic<int32> TargetUtilPercent;
};
