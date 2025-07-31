// Copyright (c) Meta Platforms, Inc. and affiliates.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CPUStressTester.h"

#include "OUIBlueprintFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class PERFORMANCESETTINGS_API UOUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	static TArray<FCPUStressTestWorker*> CpuStressTestWorkers;
	static int32 cpuStressTestUtilizationPercent;

public:
	UFUNCTION(BlueprintPure, Category = "Oculus|OUI")
	static bool IsDynamicResolutionEnabled();

	UFUNCTION(BlueprintPure, Category = "Oculus|OUI")
	static bool IsPassthroughEnabled();

	UFUNCTION(BlueprintPure, Category = "Oculus|OUI")
	static void GetEngineMajorMinorVersion(int32& OutMajor, int32& OutMinor);

	UFUNCTION(BlueprintPure, Category = "Oculus|OUI")
	static bool IsUE55OrLater();

	UFUNCTION(BlueprintCallable, Category = "Oculus|CPU Stress Test")
	static void SetNumCPUStressTestWorkers(int32 num);

	UFUNCTION(BlueprintCallable, Category = "Oculus|CPU Stress Test")
	static void SetCPUStressTestPercent(int32 utilizationPercent);

	UFUNCTION(BlueprintCallable, Category = "Oculus|CPU Stress Test")
	static void DestroyCPUStressTestWorker();

	UFUNCTION(BlueprintPure, Category = "Oculus|CPU Stress Test")
	static int32 GetNumCPUStressTestWorker();

	UFUNCTION(BlueprintCallable, Category = "Oculus|GPU Stress Test", meta = (WorldContext = "WorldContextObject"))
	static TArray<AActor*> CreateGPUStressTestObjects(UObject* WorldContextObject, TSubclassOf<AActor> ActorToSpawn, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Oculus|GPU Stress Test", meta = (WorldContext = "WorldContextObject"))
	static int32 DestroyGPUStressTestObjects(UObject* WorldContextObject);
};
