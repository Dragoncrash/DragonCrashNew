// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonCrashNew.h"
#include "MultiplayerFunctions.h"

FMonitoredProcess *UMultiplayerFunctions::dedicatedServerProcess = nullptr;

void UMultiplayerFunctions::StartDedicatedServer(const FString& dedicatedServerName) {
	if (UMultiplayerFunctions::dedicatedServerProcess) return;

	FString filename = FPaths::Combine(FPlatformProcess::BaseDir(), dedicatedServerName);
	UMultiplayerFunctions::dedicatedServerProcess = new FMonitoredProcess(filename, TEXT("-log"), true);
	UMultiplayerFunctions::dedicatedServerProcess->Launch();
}

void UMultiplayerFunctions::StopDedicatedServer() {
	if (!UMultiplayerFunctions::dedicatedServerProcess || !UMultiplayerFunctions::dedicatedServerProcess->IsRunning()) return;

	UMultiplayerFunctions::dedicatedServerProcess->Cancel();
}