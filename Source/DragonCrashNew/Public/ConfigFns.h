// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/GameUserSettings.h"
#include "ConfigFns.generated.h"

/**
 * 
 */

#define GFILE  "CustomSettings.ini"


UCLASS(Config=Engine)
class DRAGONCRASHNEW_API UConfigFns : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Config")
	static void ReadCustomConfig(const FString& filename, const FString& section, const FString& var, FString& out_value, bool& out_valid, FString& out_filepath);

	UFUNCTION(BlueprintCallable, Category = "Config")
	static void WriteCustomConfig(const FString& filename, const FString& section, const FString& var, const FString& value);

	UFUNCTION(BlueprintCallable, Category = "Config")
	static void ReadEngineConfig(const FString& section, const FString& var, FString& out_value, bool& out_valid);

	UFUNCTION(BlueprintCallable, Category = "Config")
	static void WriteEngineConfig(const FString& section, const FString& var, const FString& value);
	
	//Added a GET function for GEngineIni for compatibility [DEPRECATED]
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Config")
	//static FString getEngineConfigFilePath();
	
	UFUNCTION(BlueprintCallable, Category = "Config")
	static void checkConfig(const FString& fname, const FString& section, const FString& key, const FString& defaultValue);

	static void checkFileIOEnabled();
	static bool FileIsValid(const FString& filepath, bool forceCreate = true);
	static FString ReadValue(const FString& filepath, const FString& section, const FString& var);
	static bool ReadValid(const FString & filename, const FString & section, const FString & var);
};

UCLASS(Config=Engine)
class DRAGONCRASHNEW_API UGConfigFns : public UConfigFns
{
	GENERATED_BODY()
public:
	//Vars
	enum class G_Presets {
		LOW,
		MED,
		HIGH,
		EPIC,
		EXTREME,
		CUSTOM
	};
	//Config Filename

	//Presets
	static int preset;
	static TArray<FIntPoint> Resolutions;
	static FIntPoint curr_res;
	static float res_scale;
	//Functions 

	UFUNCTION(BlueprintCallable, Category = "Config|Graphics")
	static void OnConstructed();

	//UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void checkAllConfig();

	//UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void LoadFromConfig();
	static void LoadPreset();
	static void LoadFromSettings();
	static bool isCustomGraphics();
};
