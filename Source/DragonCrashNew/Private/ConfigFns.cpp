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
	if (out_value == FString("--")) out_valid = false;

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
	else out_value = FString("--");

	return out_value;
}

bool UConfigFns::ReadValid(const FString& filename, const FString& section, const FString& var) {
	FString filepath = FPaths::Combine(FPaths::GameConfigDir(), filename);
	bool out_valid = true;
	FString out_value = FString();
	if (GConfig->DoesSectionExist(*section, filepath)) {
		GConfig->GetString(*section, *var, out_value, filepath);
		return out_value == FString();
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

	if (!GConfig->DoesSectionExist(*section, filepath)) {
		WriteCustomConfig(fname, section, key, defaultValue);
	}
}

#pragma endregion Config

#pragma region GConfig

//Presets
int UGConfigFns::preset;
TArray<FIntPoint> UGConfigFns::Resolutions;
float UGConfigFns::res_scale;
FIntPoint UGConfigFns::curr_res;
/*int UGConfigFns::TQ;
int UGConfigFns::EQ;
int UGConfigFns::SQ;
int UGConfigFns::PPQ;

//Resolution
TArray<FIntPoint> UGConfigFns::Resolutions;
int UGConfigFns::res_index;
int UGConfigFns::res_scale;
*/


void UGConfigFns::OnConstructed(){
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	checkAllConfig();
	LoadFromConfig();
}

void UGConfigFns::checkAllConfig() {
	checkConfig(GFILE, "Graphics", "Preset", "0");
	checkConfig(GFILE, "Graphics", "AAM", "0");
	checkConfig(GFILE, "Graphics", "AF", "1");
}

void UGConfigFns::LoadFromConfig() {
	if (ReadValid(GFILE, "Graphics", "Preset")) {
		preset = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "Preset"));
	}
	else preset = static_cast<int>(G_Presets::LOW);

	if (isCustomGraphics()) {
		//AAM 
		FString AAM;
		if (ReadValid(GFILE, "Graphics", "AAM")) {
			auto AAM_Int = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "AAM"));
			AAM = UKismetStringLibrary::Conv_IntToString(FMath::Clamp(AAM_Int, 0, 2));
			//GEngine->GameViewport->Exec(NULL, *FString("r.DefaultFeature.AntiAliasing " + AAM), *GLog);
		}
		else {
			WriteCustomConfig(GFILE, "Graphics", "AAM", "0");
			AAM = FString("0");
			//GEngine->GameViewport->Exec(NULL, *FString("r.DefaultFeature.AntiAliasing 0"), *GLog);
		}
		//AF 
		FString AF;
		if (ReadValid(GFILE, "Graphics", "AF")) {
			auto AF_Int = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "AF"));
			AF = UKismetStringLibrary::Conv_IntToString(FMath::Clamp(AF_Int, 0, 8));
			//GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy " + AF), *GLog);
		}
		else {
			WriteCustomConfig(GFILE, "Graphics", "AF", "0");
			AF = FString("0");
			//GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 0"), *GLog);
		}
		//Resolution
		int resx=0, resy=0;
		if (ReadValid(GFILE, "Graphics", "ResolutionX")) {
			resx = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ResolutionX"));
		}
		if (ReadValid(GFILE, "Graphics", "ResolutionY")) {
			resy = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ResolutionY"));
		}
		
		if (resx <= 0 || resy <= 0) {
			auto tmp = UGameUserSettings::GetDefaultResolution();
			resx = tmp.X;
			resy = tmp.Y;
		}

		curr_res = FIntPoint(resx, resy);
		//UGameUserSettings::GetGameUserSettings()->SetScreenResolution(curr_res);

		//Resolution Scale
		int res_scale;
		if (ReadValid(GFILE, "Graphics", "ResolutionScale")) {
			res_scale = UKismetStringLibrary::Conv_StringToInt(ReadValue(GFILE, "Graphics", "ResolutionScale"));
		}
		else {
			WriteCustomConfig(GFILE, "Graphics", "ResolutionScale", "50");
			res_scale = 50;
		}

		//Fullscreen
		int fullscreenmode;
		if (ReadValid(GFILE, "Graphics", "Fullscreen")) {
			//TODO
		}
		else {
			fullscreenmode = static_cast<int>(EWindowMode::Type::Windowed);
		}
	}
	else LoadPreset();
}

void UGConfigFns::LoadPreset() {
	switch (preset) {
		case static_cast<int>(G_Presets::LOW) :
			UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValue(50);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(2);
			GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 0"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.TextureQuality 0"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.EffectsQuality 0"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.ShadowQuality 0"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.PostProcessQuality 0"), *GLog);
				break;
		case static_cast<int>(G_Presets::MED):
			UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValue(75);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(3);
			GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 2"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.TextureQuality 1"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.EffectsQuality 1"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.ShadowQuality 1"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.PostProcessQuality 1"), *GLog);
			break;
		case static_cast<int>(G_Presets::HIGH) :
			UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValue(100);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(4);
			GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 4"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.TextureQuality 2"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.EffectsQuality 2"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.ShadowQuality 2"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.PostProcessQuality 2"), *GLog);
			break;
		case static_cast<int>(G_Presets::EPIC) :
			UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValue(100);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(5);
			GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 8"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.TextureQuality 3"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.EffectsQuality 3"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.ShadowQuality 3"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.PostProcessQuality 3"), *GLog);
			break;
		case static_cast<int>(G_Presets::EXTREME) :
			UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValue(100);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(6);
			GEngine->GameViewport->Exec(NULL, *FString("r.MaxAnisotropy 16"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.TextureQuality 3"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.EffectsQuality 3"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.ShadowQuality 3"), *GLog);
			GEngine->GameViewport->Exec(NULL, *FString("sg.PostProcessQuality 3"), *GLog);
			break;
		default :
			UE_LOG(ConfigFunctions, Warning, TEXT("CUSTOM case reached as preset!"));
			break;
	};
}

void UGConfigFns::LoadFromSettings() {
	//Resolution Scale
	float c_scale_norm;
	int32 c_scale, min_scale, max_scale;
	UGameUserSettings::GetGameUserSettings()->GetResolutionScaleInformation(c_scale_norm,c_scale,min_scale,max_scale);

	//Resolution
	//FIntPoint last_res = UGameUserSettings::GetGameUserSettings()->GetLastConfirmedScreenResolution();

	//Fullscreen
	EWindowMode::Type last_fs = UGameUserSettings::GetGameUserSettings()->GetLastConfirmedFullscreenMode();

	//VSync
	bool vsync_enabled = UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled();

}

bool UGConfigFns::isCustomGraphics() {
	return preset == static_cast<int>(G_Presets::CUSTOM);
}
#pragma endregion GConfig