// Copyright (c) Meta Platforms, Inc. and affiliates.

#include "OUIBlueprintFunctionLibrary.h"

#include "Engine/Engine.h"
#include "Misc/EngineVersion.h" // Needed for FEngineVersion
#include "OculusXRHMDRuntimeSettings.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

bool UOUIBlueprintFunctionLibrary::IsDynamicResolutionEnabled()
{
	const UOculusXRHMDRuntimeSettings* OculusSettings = GetDefault<UOculusXRHMDRuntimeSettings>();
	return OculusSettings ? OculusSettings->bDynamicResolution : false;
}

bool UOUIBlueprintFunctionLibrary::IsPassthroughEnabled()
{
	// Use GetDefault to read the .ini-based default settings
	const UOculusXRHMDRuntimeSettings* OculusSettings = GetDefault<UOculusXRHMDRuntimeSettings>();
	return OculusSettings ? OculusSettings->bInsightPassthroughEnabled : false;
}

void UOUIBlueprintFunctionLibrary::GetEngineMajorMinorVersion(int32& OutMajor, int32& OutMinor)
{
	const FEngineVersion EngineVersion = FEngineVersion::Current();
	OutMajor = EngineVersion.GetMajor();
	OutMinor = EngineVersion.GetMinor();
}

bool UOUIBlueprintFunctionLibrary::IsUE55OrLater()
{
	const FEngineVersion EngineVersion = FEngineVersion::Current();

	if (EngineVersion.GetMajor() > 5)
	{
		return true;
	}

	if (EngineVersion.GetMajor() == 5 && EngineVersion.GetMinor() >= 5)
	{
		return true;
	}

	return false;
}

TArray<FCPUStressTestWorker*> UOUIBlueprintFunctionLibrary::CpuStressTestWorkers;
int32 UOUIBlueprintFunctionLibrary::cpuStressTestUtilizationPercent;

void UOUIBlueprintFunctionLibrary::SetNumCPUStressTestWorkers(int32 numWorkers)
{
	if (numWorkers < 0)
	{
		numWorkers = 0;
	}
	while (CpuStressTestWorkers.Num() < numWorkers)
	{
		CpuStressTestWorkers.Add(new FCPUStressTestWorker(cpuStressTestUtilizationPercent));
	}
	while (CpuStressTestWorkers.Num() > numWorkers)
	{
		DestroyCPUStressTestWorker();
	}
}

void UOUIBlueprintFunctionLibrary::SetCPUStressTestPercent(int32 utilizationPercent)
{
	cpuStressTestUtilizationPercent = utilizationPercent;
	for (FCPUStressTestWorker* Worker : CpuStressTestWorkers)
	{
		Worker->SetTargetUtilPercent(utilizationPercent);
	}
}

void UOUIBlueprintFunctionLibrary::DestroyCPUStressTestWorker()
{
	const int32 LastIndex = CpuStressTestWorkers.Num() - 1;
	if (LastIndex == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No workers to stop."));
		return;
	}

	FCPUStressTestWorker* Worker = CpuStressTestWorkers[LastIndex];
	if (Worker)
	{
		Worker->Stop();
		delete Worker;
	}

	// Remove them from our arrays
	CpuStressTestWorkers.RemoveAt(LastIndex);
}

int32 UOUIBlueprintFunctionLibrary::GetNumCPUStressTestWorker()
{
	return CpuStressTestWorkers.Num();
}

TArray<AActor*> UOUIBlueprintFunctionLibrary::CreateGPUStressTestObjects(UObject* WorldContextObject, TSubclassOf<AActor> ActorToSpawn, int32 Count)
{
	TArray<AActor*> SpawnedActors;

	if (!WorldContextObject || !ActorToSpawn)
	{
		return SpawnedActors;
	}

	// Retrieve the world from the context object
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(WorldContextObject) : nullptr;
	if (!World)
	{
		return SpawnedActors;
	}

	for (int32 i = 0; i < Count; i++)
	{
		// Determine a random location for spawning; adjust the range as needed.
		FVector SpawnLocation(
			FMath::RandRange(100.f, 2000.f),
			FMath::RandRange(-1000.f, 1000.f),
			FMath::RandRange(0.f, 100.f));
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the actor and tag it for later identification.
		AActor* SpawnedActor = World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedActor)
		{
			SpawnedActor->Tags.Add(FName("StressTest"));
			SpawnedActors.Add(SpawnedActor);
		}
	}
	return SpawnedActors;
}

int32 UOUIBlueprintFunctionLibrary::DestroyGPUStressTestObjects(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return 0;
	}

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(WorldContextObject) : nullptr;
	if (!World)
	{
		return 0;
	}

	// Retrieve all actors with the tag "StressTest" and destroy them.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(World, FName("StressTest"), FoundActors);
	int numDestroyed = 0;
	for (AActor* Actor : FoundActors)
	{
		if (Actor)
		{
			Actor->Destroy();
			numDestroyed++;
		}
	}
	return numDestroyed;
}
