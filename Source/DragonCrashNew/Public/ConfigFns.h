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

UCLASS()
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
	UFUNCTION(BlueprintCallable, Category = "Config|Test")
	static FString ReadValue(const FString& filepath, const FString& section, const FString& var);
	UFUNCTION(BlueprintCallable, Category="Config|Test")
	static bool ReadValid(const FString & filename, const FString & section, const FString & var);
};

UCLASS()
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
	//static int resX;
	//static int resY;
	static FIntPoint curr_res;
	static float res_scale;
	static int AAM_Type;
	static int AAM_Level;
	static int AF;
	static int fullscreenmode;
	static bool vsync_enabled;
	static int effects_quality;
	static int shadow_quality;
	static int texture_quality;
	static int postprocess_quality;
	//Functions 

	//Accessors
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getPreset() { return preset; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static TArray<FIntPoint> getResolutions() { return Resolutions; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static FIntPoint getCurrentResolution() { return curr_res; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static float getResolutionScale() { return res_scale; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getAAM_Type() { return AAM_Type; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getAAM_Level() { return AAM_Level; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getAF() { return AF; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getFullscreenMode() { return fullscreenmode; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static bool getVSyncEnabled() { return vsync_enabled; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getEffectsQuality() { return effects_quality; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getShadowQuality() { return shadow_quality; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getTextureQuality() { return texture_quality; }
	UFUNCTION(BlueprintPure, Category = "Config|Graphics|Variables")
	static int getPostProcessQuality() { return postprocess_quality; }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void getCustomSettings(int& o_preset, FIntPoint& o_curr_res, float& o_res_scale, int& o_aamtype, int& o_aamlevel, int& o_af, int& o_fsm, bool& o_vs, int& o_eq, int& o_sq, int& o_tq, int& o_ppq) {
		o_preset = getPreset();
		//o_curr_resX = resX;
		//o_curr_resY = resY;
		o_curr_res = getCurrentResolution();
		o_res_scale = getResolutionScale();
		o_aamtype = getAAM_Type();
		o_aamlevel = getAAM_Level();
		o_af = getAF();
		o_fsm = getFullscreenMode();
		o_vs = getVSyncEnabled();
		o_eq = getEffectsQuality();
		o_sq = getShadowQuality();
		o_tq = getTextureQuality();
		o_ppq = getPostProcessQuality();
	}
	//Mutators
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setPreset(int n) { preset = FMath::Clamp(n, 0, 5); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setCurrentResolution(FIntPoint n) {
		if (Resolutions.Find(n) != INDEX_NONE)curr_res = n;
		//else UE_LOG(ConfigFunctions, Warning, TEXT("Attempted to set invalid or unsupported resolution!"));
	}
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setResolutionScale(float n) { res_scale = FMath::Clamp(n, 25.0f, 100.0f); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setAAM_Type(int n) { AAM_Type = FMath::Clamp(n, 0, 2); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setAAM_Level(int n) { AAM_Level = FMath::Clamp(n, 0, 6); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setAF(int n) { AF = FMath::Clamp(n, 0, 16); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setFullscreenMode(int n) { fullscreenmode = FMath::Clamp(n, 0, 2); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setVSyncEnabled(bool b) { vsync_enabled = b; }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setEffectsQuality(int n) { effects_quality = FMath::Clamp(n, 0, 3); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setShadowQuality(int n) { shadow_quality = FMath::Clamp(n, 0, 3); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setTextureQuality(int n) { texture_quality = FMath::Clamp(n, 0, 3); }
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void setPostProcessQuality(int n) { postprocess_quality = FMath::Clamp(n, 0, 3); }

	UFUNCTION(BlueprintCallable, Category = "Config|Graphics")
	static void OnConstructed();
	UFUNCTION(BlueprintCallable, Category = "Config|Graphics|Variables")
	static void RefreshPreset(int Preset);

	//UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void checkAllConfig();

	//UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void LoadFromConfig();
	static void LoadPreset(bool force = true);

	UFUNCTION(BlueprintCallable, Category="Config|Graphics")
	static void ApplyCustomSettings();
	
	static void ApplyGraphicsSettings();
	static void ApplyScreenSettings();
	
	static void syncToFile();
	
	UFUNCTION(BlueprintPure, Category = "Config|Graphics")
	static bool isCustomGraphics();

	UFUNCTION(BlueprintCallable,Category="Config|Test")
	static FString getEUDir() {
		return FPaths::Combine(FPaths::EngineUserDir(), "testConfig.ini"); 
	}
};

UCLASS()
class DRAGONCRASHNEW_API UAConfigFns: public UConfigFns {
	GENERATED_BODY()
public:
	static float master_volume;
	static float sfx_volume;
	static float crowd_volume;
	static float bgm_volume;

	//Accessors
	UFUNCTION(BlueprintPure, Category="Config|Audio|Variables")
	static float getMasterVolume() { return master_volume; }
	UFUNCTION(BlueprintPure, Category = "Config|Audio|Variables")
	static float getSFXVolume() { return sfx_volume; }
	UFUNCTION(BlueprintPure, Category = "Config|Audio|Variables")
	static float getCrowdVolume() { return crowd_volume; }
	UFUNCTION(BlueprintPure, Category = "Config|Audio|Variables")
	static float getBGMVolume() { return bgm_volume; }
	UFUNCTION(BlueprintPure, Category = "Config|Audio|Variables")
	static void getCustomAudioSettings(float& MasterVolume, float& SFXVolume, float& CrowdVolume, float& BGMVolume) {
		MasterVolume = getMasterVolume();
		SFXVolume = getSFXVolume();
		CrowdVolume = getCrowdVolume();
		BGMVolume = getBGMVolume();
	}

	//Mutators
	UFUNCTION(BlueprintCallable, Category = "Config|Audio|Variables")
	static void setMasterVolume(float MasterVolume) { master_volume = FMath::Clamp(MasterVolume, 0.0f, 1.0f); }
	UFUNCTION(BlueprintCallable, Category = "Config|Audio|Variables")
	static void setSFXVolume(float SFX_Volume) { sfx_volume = FMath::Clamp(SFX_Volume, 0.0f, 1.0f); }
	UFUNCTION(BlueprintCallable, Category = "Config|Audio|Variables")
	static void setCrowdVolume(float CrowdVolume) { crowd_volume = FMath::Clamp(CrowdVolume, 0.0f, 1.0f); }
	UFUNCTION(BlueprintCallable, Category = "Config|Audio|Variables")
	static void setBGMVolume(float BGMVolume) { bgm_volume = FMath::Clamp(BGMVolume, 0.0f, 1.0f); }

	//Nodes
	UFUNCTION(BlueprintCallable, Category = "Config|Audio")
	static void onConstructed();

	UFUNCTION(BlueprintCallable, Category = "Config|Audio")
	static void ApplyCustomSettings() { syncToFile(); }

	static void syncToFile();
};
