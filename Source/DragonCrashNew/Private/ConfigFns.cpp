// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonCrashNew.h"
#include "ConfigFns.h"

#pragma region Config
void UConfigFns::ReadCustomConfig(const FString& filename, const FString& section, const FString& var, FString& out_value, bool& out_valid, FString& out_filepath) {

	//Config handler is GConfig I think...
	
	FString filepath = FPaths::Combine(FPaths::GameConfigDir(), filename);
	out_filepath = filepath;
	out_valid = true;

	FileIsValid(filepath);
	out_value = ReadValue(filename, section, var);
	if (out_value == FString()) out_valid = false;

}

void UConfigFns::WriteCustomConfig(const FString& filename, const FString& section, const FString& var, const FString& value) {
	
	if (!GConfig) return;
	checkFileIOEnabled();

	FString filepath = FPaths::Combine(FPaths::GameConfigDir(), filename);
	FileIsValid(filename);
	//Create config if it does not exist and reload values
	/*if (!FPaths::FileExists(filepath)) {
		FString cleanname = FPaths::GetCleanFilename(filepath);
		GConfig->LoadGlobalIniFile(cleanname,*FPaths::GetBaseFilename(filepath),NULL,true);
	}*/
	
	//File exists so modify the values
	GConfig->SetString(*section, *var, *value, filepath);

	//Force Flush buffers
	GConfig->Flush(false, filepath);

}

void UConfigFns::ReadEngineConfig(const FString & section, const FString & var, FString & out_value, bool & out_valid)
{
	if (!GConfig) return;
	checkFileIOEnabled();

	out_valid = true;
	if (GConfig->DoesSectionExist(*section, GEngineIni)) {
		GConfig->GetString(*section, *var, out_value, GEngineIni);
	}
	else out_valid = false;
}

void UConfigFns::WriteEngineConfig(const FString & section, const FString & var, const FString & value)
{
	if (!GConfig) return;
	checkFileIOEnabled();

	FString filepath = FPaths::Combine(FPaths::GameUserDir(), GEngineIni);

	//File exists so modify the values
	GConfig->SetString(*section, *var, *value, GEngineIni);

	//Force Flush buffers
	GConfig->Flush(false, GEngineIni);
}

void UConfigFns::checkFileIOEnabled() {
	if (GConfig->AreFileOperationsDisabled()) GConfig->EnableFileOperations();
}

bool UConfigFns::FileIsValid(const FString& filepath, bool forceCreate) {
	if (!GConfig) return false;
	checkFileIOEnabled();
	if (!FPaths::FileExists(filepath)) {
		if (forceCreate) {
			FString cleanname = FPaths::GetCleanFilename(filepath);
			GConfig->LoadGlobalIniFile(cleanname, *FPaths::GetBaseFilename(filepath), NULL, true);
		}
		return false;
	}
	return true;
}

FString UConfigFns::ReadValue(const FString& filename, const FString& section, const FString& var) {
	FString filepath = FPaths::Combine(FPaths::GameConfigDir(), filename);
	FString out_value = FString();
	if (GConfig->DoesSectionExist(*section, filepath)) {
		GConfig->GetString(*section, *var, out_value, filepath);
	}
	else out_value = FString();

	return out_value;
}

bool UConfigFns::ReadValid(const FString& filename, const FString& section, const FString& var) {
	FString filepath = FPaths::Combine(FPaths::GameConfigDir(), filename);
	bool out_valid = true;
	FString out_value = FString();
	if (GConfig->DoesSectionExist(*section, filepath)) {
		GConfig->GetString(*section, *var, out_value, filepath);
		return out_value != FString();
	}
	return false;
}


void UConfigFns::checkConfig(const FString& fname,const FString& section,const FString& key, const FString& defaultValue) {
	if (!GConfig) return;
	checkFileIOEnabled();

	FString filepath = FPaths::Combine(FPaths::GameConfigDir(), fname);

	if (!FPaths::FileExists(filepath)) {
		FString cleanname = FPaths::GetCleanFilename(filepath);
		GConfig->LoadGlobalIniFile(cleanname, *FPaths::GetBaseFilename(filepath), NULL, true);
	}

	if (!ReadValid(GFILE,section,key)) {
		WriteCustomConfig(fname, section, key, defaultValue);
	}
}

#pragma endregion Config


//Presets
int UGConfigFns::preset;
TArray<FIntPoint> UGConfigFns::Resolutions;
//int UGConfigFns::resX;
//int UGConfigFns::resY;
float UGConfigFns::res_scale;
FIntPoint UGConfigFns::curr_res;
int UGConfigFns::AAM_Type;
int UGConfigFns::AAM_Level;
int UGConfigFns::AF;
int UGConfigFns::fullscreenmode;
bool UGConfigFns::vsync_enabled;
int UGConfigFns::effects_quality;
int UGConfigFns::shadow_quality;
int UGConfigFns::texture_quality;
int UGConfigFns::postprocess_quality;

void UGConfigFns::OnConstructed(){
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	checkAllConfig();
	//LoadPreset(false);
	LoadFromConfig();
}

void UGConfigFns::RefreshPreset(int Preset) {
	setPreset(Preset);
	LoadPreset(false);
}

void UGConfigFns::checkAllConfig() {
	//May not need this since LoadFromConfig writes entries if missing
	checkConfig(GFILE, "Graphics", "Preset", "0");
	checkConfig(GFILE, "Graphics", "AAM_Type", "0");
	checkConfig(GFILE, "Graphics", "AF", "1");
}

void UGConfigFns::LoadFromConfig() {
	//Preset
#pragma region Preset
	if (ReadValid(GFILE, "Graphics", "Preset")) {
		preset = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "Preset"));
	}
	else {
		preset = static_cast<int>(G_Presets::LOW);
		WriteCustomConfig(GFILE, "Graphics", "Preset", "0");
	}
#pragma endregion	
	//AAM Type
#pragma region AAM
	if (ReadValid(GFILE, "Graphics", "AAM_Type")) {
		AAM_Type = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "AAM"));
		//AAM = UKismetStringLibrary::Conv_IntToString(FMath::Clamp(AAM_Int, 0, 2));
		//GEngine->GameViewport->Exec(NULL, *FString("r.DefaultFeature.AntiAliasing " + AAM), *GLog);
	}
	else {
		WriteCustomConfig(GFILE, "Graphics", "AAM_Type", "0");
		AAM_Type = 0;
		//GEngine->GameViewport->Exec(NULL, *FString("r.DefaultFeature.AntiAliasing 0"), *GLog);
	}
#pragma endregion
	//AF 
#pragma region AF
		if (ReadValid(GFILE, "Graphics", "AF")) {
			AF = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "AF"));
			//AF = UKismetStringLibrary::Conv_IntToString(FMath::Clamp(AF_Int, 0, 8));
			//GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy " + AF), *GLog);
		}
		else {
			WriteCustomConfig(GFILE, "Graphics", "AF", "0");
			AF = 0;
			//GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 0"), *GLog);
		}
#pragma endregion
	//Resolution
#pragma region Resolution
		int resX, resY;
		if (ReadValid(GFILE, "Graphics", "ResolutionX")) {
			resX = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ResolutionX"));
		}
		if (ReadValid(GFILE, "Graphics", "ResolutionY")) {
			resY = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ResolutionY"));
		}
		
		if (resX <= 0 || resY <= 0) {
			FIntPoint tmp = UGameUserSettings::GetGameUserSettings()->GetDefaultResolution();
			resX = tmp.X;
			resY = tmp.Y;
			WriteCustomConfig(GFILE, "Graphics", "ResolutionX", UKismetStringLibrary::Conv_IntToString(resX));
			WriteCustomConfig(GFILE, "Graphics", "ResolutionY", UKismetStringLibrary::Conv_IntToString(resY));
		}

		curr_res = FIntPoint(resX, resY);
		
#pragma endregion
	//Resolution Scale
#pragma region Resolution Scale
		if (ReadValid(GFILE, "Graphics", "ResolutionScale")) {
			res_scale = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ResolutionScale"));
		}
		else {
			WriteCustomConfig(GFILE, "Graphics", "ResolutionScale", "50");
			res_scale = 50;
		}
#pragma endregion
	//Fullscreen
#pragma region Fullscreen
		if (ReadValid(GFILE, "Graphics", "Fullscreen")) {
			fullscreenmode = static_cast<EWindowMode::Type>(UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "Fullscreen")));
		}
		else {
			fullscreenmode = static_cast<int>(EWindowMode::Type::Windowed);
			WriteCustomConfig(GFILE, "Graphics", "FullscreenMode",UKismetStringLibrary::Conv_IntToString(fullscreenmode));
		}
#pragma endregion
	//VSync
#pragma region VSync
		if (ReadValid(GFILE, "Graphics", "VSync")) {
			vsync_enabled = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "VSync")) == 0;
		}
		else {
			vsync_enabled = false;
			WriteCustomConfig(GFILE, "Graphics", "VSync", FString("0"));
		}
#pragma endregion
	//EQ
#pragma region EffectsQuality
		if (ReadValid(GFILE, "Graphics","EffectsQuality")) {
			effects_quality = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "EffectsQuality"));
		}
		else {
			effects_quality = 0;
			WriteCustomConfig(GFILE, "Graphics", "EffectsQuality", "0");
		}
#pragma endregion
	//SQ
#pragma region ShadowQuality
		if (ReadValid(GFILE, "Graphics", "ShadowQuality")) {
			shadow_quality = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ShadowQuality"));
		}
		else {
			shadow_quality = 0;
			WriteCustomConfig(GFILE, "Graphics", "ShadowQuality", "0");
		}
#pragma endregion
	//TQ
#pragma region TextureQuality
		if (ReadValid(GFILE, "Graphics", "TextureQuality")) {
			texture_quality = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "TextureQuality"));
		}
		else {
			texture_quality = 0;
			WriteCustomConfig(GFILE, "Graphics", "TextureQuality", "0");
		}
#pragma endregion
	//AAM_Level
#pragma region AA_Level
		if (ReadValid(GFILE, "Graphics", "AA_Level")) {
			AAM_Level = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "AA_Level"));
		}
		else {
			AAM_Level = 2;
			WriteCustomConfig(GFILE, "Graphics", "AA_Level", "2");
		}
#pragma endregion
	//PPQ
#pragma region PostProcessQuality
		if (ReadValid(GFILE, "Graphics", "PostProcessQuality")) {
			postprocess_quality = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "PostProcessQuality"));
		}
		else {
			postprocess_quality = 0;
			WriteCustomConfig(GFILE, "Graphics", "PostProcessQuality", "0");
		}
#pragma endregion

	ApplyCustomSettings();
		
	
	LoadPreset();
}

void UGConfigFns::ApplyCustomSettings() {
	ApplyGraphicsSettings();
	ApplyScreenSettings();
}

void UGConfigFns::ApplyGraphicsSettings() {
	GEngine->GameViewport->Exec(NULL, *FString("r.DefaultFeature.AntiAliasing " + AAM_Type), *GLog);
	GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy " + AF), *GLog);
	GEngine->GameViewport->Exec(NULL, *FString("sg.TextureQuality " + texture_quality), *GLog);
	GEngine->GameViewport->Exec(NULL, *FString("sg.EffectsQuality " + effects_quality), *GLog);
	GEngine->GameViewport->Exec(NULL, *FString("sg.ShadowQuality " + shadow_quality), *GLog);
	GEngine->GameViewport->Exec(NULL, *FString("sg.PostProcessQuality " + postprocess_quality), *GLog);
	UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValue(res_scale);
	UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(AAM_Level);
}

void UGConfigFns::ApplyScreenSettings() {
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(static_cast<EWindowMode::Type>(fullscreenmode));
	UGameUserSettings::GetGameUserSettings()->SetScreenResolution(curr_res);
	UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(vsync_enabled);
}

void UGConfigFns::LoadPreset(bool force) {
	switch (preset) {
		case static_cast<int>(G_Presets::LOW) :
			res_scale = 50;
			AAM_Level = 2;
			AF = 0;
			texture_quality = 0;
			effects_quality = 0;
			shadow_quality = 0;
			postprocess_quality = 0;
				break;
		case static_cast<int>(G_Presets::MED):
			res_scale = 75;
			AAM_Level = 3;
			AF = 2;
			texture_quality = 1;
			effects_quality = 1;
			shadow_quality = 1;
			postprocess_quality = 1;
			break;
		case static_cast<int>(G_Presets::HIGH) :
			res_scale = 100;
			AAM_Level = 4;
			AF = 4;
			texture_quality = 2;
			effects_quality = 2;
			shadow_quality = 2;
			postprocess_quality = 2;
			break;
		case static_cast<int>(G_Presets::EPIC) :
			res_scale = 100;
			AAM_Level = 5;
			AF = 8;
			texture_quality = 3;
			effects_quality = 3;
			shadow_quality = 3;
			postprocess_quality = 3;
			break;
		case static_cast<int>(G_Presets::EXTREME) :
			res_scale = 100;
			AAM_Level = 6;
			AF = 16;
			texture_quality = 3;
			effects_quality = 3;
			shadow_quality = 3;
			postprocess_quality = 3;
			break;
		case static_cast<int>(G_Presets::CUSTOM) :
			res_scale = 75;
			AAM_Level = 3;
			AF = 2;
			texture_quality = 1;
			effects_quality = 1;
			shadow_quality = 1;
			postprocess_quality = 1;
			break;
		default :
			UE_LOG(ConfigFunctions, Warning, TEXT("Unknown case reached as preset!"));
			break;
	};
	WriteCustomConfig(GFILE, "Graphics", "Preset", UKismetStringLibrary::Conv_IntToString(preset));
	WriteCustomConfig(GFILE, "Graphics", "ResolutionScale", UKismetStringLibrary::Conv_IntToString(res_scale));
	WriteCustomConfig(GFILE, "Graphics", "AA_Level", UKismetStringLibrary::Conv_IntToString(AAM_Level));
	WriteCustomConfig(GFILE, "Graphics", "AF", UKismetStringLibrary::Conv_IntToString(AF));
	WriteCustomConfig(GFILE, "Graphics", "EffectsQuality", UKismetStringLibrary::Conv_IntToString(effects_quality));
	WriteCustomConfig(GFILE, "Graphics", "ShadowQuality", UKismetStringLibrary::Conv_IntToString(shadow_quality));
	WriteCustomConfig(GFILE, "Graphics", "TextureQuality", UKismetStringLibrary::Conv_IntToString(texture_quality));
	WriteCustomConfig(GFILE, "Graphics", "PostProcessQuality", UKismetStringLibrary::Conv_IntToString(postprocess_quality));

	if (force)ApplyGraphicsSettings();
}

bool UGConfigFns::isCustomGraphics() {
	return preset == static_cast<int>(G_Presets::CUSTOM);
}
