// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Paths.h"
#include "MonitoredProcess.h"
#include "MultiplayerFunctions.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONCRASHNEW_API UMultiplayerFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FMonitoredProcess *dedicatedServerProcess;

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	static void StartDedicatedServer(const FString& dedicatedServerName);
	
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	static void StopDedicatedServer();
};
