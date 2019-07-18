////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NoesisRuntimeModule.h"

// Core includes
#include "CoreMinimal.h"
#include "Misc/CoreDelegates.h"
#include "Modules/ModuleManager.h"
#include "Stats/Stats.h"
#include "Stats/Stats2.h"

// RenderCore includes
#include "ShaderCore.h"

// ApplicationCore includes
#include "HAL/PlatformApplicationMisc.h"

// Slate includes
#include "Widgets/Input/IVirtualKeyboardEntry.h"
#include "Framework/Application/SlateApplication.h"

// Engine includes
#include "ActiveSound.h"

// Projects includes
#include "Interfaces/IPluginManager.h"

// NoesisRuntime includes
#include "NoesisResourceProvider.h"
#include "Render/NoesisRenderDevice.h"
#include "NoesisTypeClass.h"
#include "NoesisSettings.h"
#include "NoesisSupport.h"

// Noesis includes
#include "NoesisSDK.h"

extern "C" void NsRegisterReflectionAppInteractivity(Noesis::ComponentFactory*, bool);

static void NoesisErrorHandler(const char* Filename, uint32 Line, const char* Desc, bool Fatal)
{
	if (Fatal)
	{
		UE_LOG(LogNoesis, Error, TEXT("%s"), *NsStringToFString(Desc));
		FPlatformMisc::RequestExit(true);
	}
	UE_LOG(LogNoesis, Warning, TEXT("%s"), *NsStringToFString(Desc));
}

DECLARE_DWORD_COUNTER_STAT(TEXT("NoesisMemory"), STAT_NoesisMemory, STATGROUP_Noesis);
void* NoesisAllocationCallbackUserData = nullptr;
void* NoesisAlloc(void* UserData, size_t Size)
{
	void* Result = FMemory::Malloc(Size);
	INC_DWORD_STAT_BY(STAT_NoesisMemory, FMemory::GetAllocSize(Result));
	return Result;
}

void* NoesisRealloc(void* UserData, void* Ptr, size_t Size)
{
	DEC_DWORD_STAT_BY(STAT_NoesisMemory, FMemory::GetAllocSize(Ptr));
	void* Result = FMemory::Realloc(Ptr, Size);
	INC_DWORD_STAT_BY(STAT_NoesisMemory, FMemory::GetAllocSize(Result));
	return Result;
}

void NoesisDealloc(void* UserData, void* Ptr)
{
	DEC_DWORD_STAT_BY(STAT_NoesisMemory, FMemory::GetAllocSize(Ptr));
	FMemory::Free(Ptr);
}

size_t NoesisAllocSize(void* UserData, void* Ptr)
{
	return FMemory::GetAllocSize(Ptr);
}

static void NoesisLogHandler(const char* File, uint32_t Line, uint32_t Level, const char* Channel, const char* Message)
{
	if (UObjectInitialized())
	{
		ENoesisLoggingSettings LogVerbosity = GetDefault<UNoesisSettings>()->LogVerbosity;
		if (LogVerbosity == ENoesisLoggingSettings::Bindings || (LogVerbosity == ENoesisLoggingSettings::Normal && FCStringAnsi::Strcmp(Channel, "Bindings") != 0))
		{
			switch (Level)
			{
			case 0:
				UE_LOG(LogNoesis, VeryVerbose, TEXT("%s"), *NsStringToFString(Message));
				break;
			case 1:
				UE_LOG(LogNoesis, Verbose, TEXT("%s"), *NsStringToFString(Message));
				break;
			case 2:
				UE_LOG(LogNoesis, Log, TEXT("%s"), *NsStringToFString(Message));
				break;
			case 3:
				UE_LOG(LogNoesis, Warning, TEXT("%s"), *NsStringToFString(Message));
				break;
			case 4:
				UE_LOG(LogNoesis, Error, TEXT("%s"), *NsStringToFString(Message));
				break;
			}
		}
	}
}

#if WITH_EDITOR
class NotifyEnumChanged : public FEnumEditorUtils::INotifyOnEnumChanged
{
public:
	virtual void PreChange(const UUserDefinedEnum* Changed, FEnumEditorUtils::EEnumEditorChangeInfo ChangedType) override
	{

	}
	virtual void PostChange(const UUserDefinedEnum* Changed, FEnumEditorUtils::EEnumEditorChangeInfo ChangedType) override
	{
		NoesisDestroyAllTypes();
	}
};

class NotifyStructChanged : public FStructureEditorUtils::INotifyOnStructChanged
{
public:
	virtual void PreChange(const UUserDefinedStruct* Changed, FStructureEditorUtils::EStructureEditorChangeInfo ChangedType) override
	{

	}
	virtual void PostChange(const UUserDefinedStruct* Changed, FStructureEditorUtils::EStructureEditorChangeInfo ChangedType) override
	{
		NoesisDestroyAllTypes();
	}
};

void OnBlueprintPreCompile(UBlueprint* Blueprint)
{
	NoesisDestroyTypeClassForBlueprint(Blueprint);
}

void OnAssetRenamed(const FAssetData&, const FString&)
{
	NoesisDestroyAllTypes();
}
#endif

void OnPostEngineInit()
{
#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->OnBlueprintPreCompile().AddStatic(OnBlueprintPreCompile);
		FEnumEditorUtils::FEnumEditorManager::Get().AddListener(new NotifyEnumChanged);
		FStructureEditorUtils::FStructEditorManager::Get().AddListener(new NotifyStructChanged);

		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
		AssetRegistry.OnAssetRenamed().AddStatic(&OnAssetRenamed);
	}

	// https://answers.unrealengine.com/questions/526745/413-plugin-custom-kismet-bp-node-standalone-game-c.html
	// Workaround: Standalone mode doesn't load the editor modules, so blueprints that use the set and notify don't work correctly.
	FModuleManager::Get().LoadModule("NoesisEditor");
#endif

	NoesisRegisterTypes();
}

void ShowTextBoxVirtualKeyboard(Noesis::TextBox*);
void ShowPasswordBoxVirtualKeyboard(Noesis::PasswordBox*);

static Noesis::Ptr<Noesis::TextBox> LastSelectedTextBox;
static Noesis::Ptr<Noesis::PasswordBox> LastSelectedPasswordBox;

void OnTouchUpShowTextBoxVirtualKeyboard(Noesis::BaseComponent*, const Noesis::TouchEventArgs&)
{
	if (LastSelectedTextBox)
	{
		ShowTextBoxVirtualKeyboard(LastSelectedTextBox.GetPtr());
	}
}

void OnTouchUpShowPasswordBoxVirtualKeyboard(Noesis::BaseComponent*, const Noesis::TouchEventArgs&)
{
	if (LastSelectedPasswordBox)
	{
		ShowPasswordBoxVirtualKeyboard(LastSelectedPasswordBox.GetPtr());
	}
}

class NoesisTextBoxVirtualKeyboardEntry : public IVirtualKeyboardEntry
{
public:
	NoesisTextBoxVirtualKeyboardEntry(Noesis::TextBox* InTextBox)
		: TextBox(InTextBox)
	{
		TextBox->SelectAll();
		InitialText = NsStringToFString(TextBox->GetSelectedText());
		IsMultiline = TextBox->GetMaxLines() > 1;
		BeginIndex = TextBox->GetSelectionStart();
	}

	virtual void SetTextFromVirtualKeyboard(const FText& InNewText, ETextEntryType TextEntryType) override
	{
		if (TextEntryType == ETextEntryType::TextEntryCanceled || TextEntryType == ETextEntryType::TextEntryAccepted)
		{
			int32 Length = InitialText.Len();
			FString CurrentText = InitialText;
			FString NewString = InNewText.ToString();
			FString NewText = CurrentText.Left(BeginIndex) + NewString + CurrentText.RightChop(BeginIndex + Length);
			TextBox->SetText(TCHARToNsString(*NewText).c_str());
			int32 NewLength = NewString.Len();
			TextBox->SetSelectionStart(BeginIndex);
			TextBox->SetSelectionLength(NewLength);
		}
		else if (TextEntryType == ETextEntryType::TextEntryUpdated)
		{
			int32 Length = InitialText.Len();
			FString CurrentText = InitialText;
			FString NewString = InNewText.ToString();
			FString NewText = CurrentText.Left(BeginIndex) + NewString + CurrentText.RightChop(BeginIndex + Length);
			TextBox->SetText(TCHARToNsString(*NewText).c_str());
			int32 NewLength = NewString.Len();
			TextBox->SetSelectionStart(BeginIndex);
			TextBox->SetSelectionLength(NewLength);
		}

		if (LastSelectedTextBox != TextBox)
		{
			LastSelectedTextBox.Reset(TextBox);
			LastSelectedTextBox->TouchUp() += OnTouchUpShowTextBoxVirtualKeyboard;
		}
	}

	virtual void SetSelectionFromVirtualKeyboard(int InSelStart, int SelEnd) override
	{
		TextBox->SetSelectionStart(InSelStart);
		TextBox->SetSelectionLength(SelEnd - InSelStart);
	}

	virtual FText GetText() const override
	{
		return FText::FromString(InitialText);
	}

	virtual FText GetHintText() const override
	{
		return FText();
	}

	virtual EKeyboardType GetVirtualKeyboardType() const override
	{
		return Keyboard_Default;
	}

	virtual FVirtualKeyboardOptions GetVirtualKeyboardOptions() const
	{
		return FVirtualKeyboardOptions();
	}

	virtual bool IsMultilineEntry() const override
	{
		return IsMultiline;
	}

private:
	Noesis::TextBox* TextBox;
	FString InitialText;
	int32 BeginIndex;
	bool IsMultiline;
};

class NoesisPasswordBoxVirtualKeyboardEntry : public IVirtualKeyboardEntry
{
public:
	NoesisPasswordBoxVirtualKeyboardEntry(Noesis::PasswordBox* InPasswordBox)
		: PasswordBox(InPasswordBox)
	{
		InitialText = NsStringToFString(PasswordBox->GetPassword());
	}

	virtual void SetTextFromVirtualKeyboard(const FText& InNewText, ETextEntryType TextEntryType) override
	{
		FString NewString = InNewText.ToString();
		PasswordBox->SetPassword(TCHARToNsString(*NewString).c_str());

		if (LastSelectedPasswordBox != PasswordBox)
		{
			LastSelectedPasswordBox.Reset(PasswordBox);
			LastSelectedPasswordBox->TouchUp() += OnTouchUpShowPasswordBoxVirtualKeyboard;
		}
	}

	virtual void SetSelectionFromVirtualKeyboard(int InSelStart, int SelEnd) override
	{
	}

	virtual FText GetText() const override
	{
		return FText::FromString(InitialText);
	}

	virtual FText GetHintText() const override
	{
		return FText();
	}

	virtual EKeyboardType GetVirtualKeyboardType() const override
	{
		return Keyboard_Password;
	}

	virtual FVirtualKeyboardOptions GetVirtualKeyboardOptions() const
	{
		return FVirtualKeyboardOptions();
	}

	virtual bool IsMultilineEntry() const override
	{
		return false;
	}

private:
	Noesis::PasswordBox* PasswordBox;
	FString InitialText;
};

TSharedPtr<NoesisTextBoxVirtualKeyboardEntry> TextBoxVirtualKeyboardEntry;
TSharedPtr<NoesisPasswordBoxVirtualKeyboardEntry> PasswordBoxVirtualKeyboardEntry;

void ShowTextBoxVirtualKeyboard(Noesis::TextBox* TextBox)
{
	if (LastSelectedTextBox)
	{
		LastSelectedTextBox->TouchUp() -= OnTouchUpShowTextBoxVirtualKeyboard;
		LastSelectedTextBox.Reset();
	}
	TSharedPtr<NoesisTextBoxVirtualKeyboardEntry> NewTextBoxVirtualKeyboardEntry = MakeShared<NoesisTextBoxVirtualKeyboardEntry>(TextBox);
	FSlateApplication::Get().ShowVirtualKeyboard(true, FSlateApplication::Get().GetUserIndexForKeyboard(), NewTextBoxVirtualKeyboardEntry);
	TextBoxVirtualKeyboardEntry = NewTextBoxVirtualKeyboardEntry;
}

void ShowPasswordBoxVirtualKeyboard(Noesis::PasswordBox* PasswordBox)
{
	if (LastSelectedPasswordBox)
	{
		LastSelectedPasswordBox->TouchUp() -= OnTouchUpShowPasswordBoxVirtualKeyboard;
		LastSelectedPasswordBox.Reset();
	}
	TSharedPtr<NoesisPasswordBoxVirtualKeyboardEntry> NewPasswordBoxVirtualKeyboardEntry = MakeShared<NoesisPasswordBoxVirtualKeyboardEntry>(PasswordBox);
	FSlateApplication::Get().ShowVirtualKeyboard(true, FSlateApplication::Get().GetUserIndexForKeyboard(), NewPasswordBoxVirtualKeyboardEntry);
	PasswordBoxVirtualKeyboardEntry = NewPasswordBoxVirtualKeyboardEntry;
}

void NoesisSoftwareKeyboardCallback(void* UserData, Noesis::UIElement* FocusedElement, bool Open)
{
	if (Open)
	{
		if (FocusedElement)
		{
			const Noesis::TypeClass* NewFocusClass = FocusedElement->GetClassType();
			const Noesis::TypeClass* TextBoxClass = Noesis::TextBox::StaticGetClassType(nullptr);
			const Noesis::TypeClass* PasswordBoxClass = Noesis::PasswordBox::StaticGetClassType(nullptr);
			if (FPlatformApplicationMisc::RequiresVirtualKeyboard())
			{
				if (NewFocusClass == TextBoxClass || NewFocusClass->IsDescendantOf(TextBoxClass))
				{
					Noesis::TextBox* TextBox = (Noesis::TextBox*)FocusedElement;

					ShowTextBoxVirtualKeyboard(TextBox);
				}
				else if (NewFocusClass == PasswordBoxClass || NewFocusClass->IsDescendantOf(PasswordBoxClass))
				{
					Noesis::PasswordBox* PasswordBox = (Noesis::PasswordBox*)FocusedElement;

					ShowPasswordBoxVirtualKeyboard(PasswordBox);
				}
			}
		}
	}
	else
	{
		if (FSlateApplication::IsInitialized() && FPlatformApplicationMisc::RequiresVirtualKeyboard())
		{
			FSlateApplication::Get().ShowVirtualKeyboard(false, FSlateApplication::Get().GetUserIndexForKeyboard());
		}
	}
}

void NoesisPlaySoundCallback(void* UserData, const char* Filename, float Volume)
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		bool IsInGame = false;
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
			{
				IsInGame = true;
			}
		}

		if (!IsInGame)
			return;
	}
#endif

	FString SoundPath = NsProviderPathToAssetPath(Filename);
	USoundWave* Sound = LoadObject<USoundWave>(nullptr, SoundPath[0] == TEXT('/') ? *SoundPath : *(FString(TEXT("/")) + SoundPath));

	if (!Sound)
		return;

	if (GEngine)
	{
		FAudioDevice* const AudioDevice = GEngine->GetActiveAudioDevice();
		if (AudioDevice)
		{
			FActiveSound NewActiveSound;
			NewActiveSound.SetSound(Sound);
			NewActiveSound.bIsUISound = true;
			NewActiveSound.UserIndex = FSlateApplication::Get().GetUserIndexForKeyboard();
			NewActiveSound.Priority = Sound->Priority;
			NewActiveSound.VolumeMultiplier = Volume;

			AudioDevice->AddNewActiveSound(NewActiveSound);
		}
	}
}

class FNoesisRuntimeModule : public INoesisRuntimeModuleInterface
{
public:
	// IModuleInterface interface
	virtual void StartupModule() override
	{
#if PLATFORM_WINDOWS
		if (FCStringAnsi::Strcmp(Noesis::GetBuildVersion(), NOESIS_VERSION_NAME) != 0)
		{
			void* NoesisDllHandle = FPlatformProcess::GetDllHandle(TEXT("Noesis.dll"));
			TCHAR NoesisDllPath[WINDOWS_MAX_PATH];
			GetModuleFileName((HMODULE)NoesisDllHandle, NoesisDllPath, WINDOWS_MAX_PATH);
			UE_LOG(LogNoesis, Error, TEXT("Loaded incorrect Noesis.dll from path %s. Please delete it manually."), NoesisDllPath);
		}
#endif

		Noesis::MemoryCallbacks MemoryCallbacks{ NoesisAllocationCallbackUserData, &NoesisAlloc, &NoesisRealloc, &NoesisDealloc, &NoesisAllocSize };
		Noesis::GUI::Init(&NoesisErrorHandler, &NoesisLogHandler, &MemoryCallbacks);
		NsRegisterReflectionAppInteractivity(nullptr, true);

		NoesisXamlProvider = new FNoesisXamlProvider();
		NoesisTextureProvider = new FNoesisTextureProvider();
		NoesisFontProvider = new FNoesisFontProvider();
		Noesis::GUI::SetXamlProvider(NoesisXamlProvider);
		Noesis::GUI::SetTextureProvider(NoesisTextureProvider);
		Noesis::GUI::SetFontProvider(NoesisFontProvider);

		LastSelectedTextBox.Reset();
		LastSelectedPasswordBox.Reset();
		Noesis::GUI::SetSoftwareKeyboardCallback(nullptr, &NoesisSoftwareKeyboardCallback);

		Noesis::GUI::SetPlaySoundCallback(nullptr, &NoesisPlaySoundCallback);

		NoesisRuntimeModuleInterface = this;

		PostGarbageCollectConditionalBeginDestroyDelegateHandle = FCoreUObjectDelegates::PostGarbageCollectConditionalBeginDestroy.AddStatic(NoesisGarbageCollected);

		PostEngineInitDelegateHandle = FCoreDelegates::OnPostEngineInit.AddStatic(OnPostEngineInit);

		NsGetKernel()->GetReflectionRegistry()->SetFallbackHandler(&NoesisReflectionRegistryCallback);

		FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("NoesisGUI"))->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/NoesisGUI"), PluginShaderDir);
	}

	virtual void ShutdownModule() override
	{
		NsGetKernel()->GetReflectionRegistry()->SetFallbackHandler(nullptr);

		FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitDelegateHandle);

		FCoreUObjectDelegates::PostGarbageCollectConditionalBeginDestroy.Remove(PostGarbageCollectConditionalBeginDestroyDelegateHandle);

		void NoesisDeleteMaps();
		NoesisDeleteMaps();

		FNoesisRenderDevice::Destroy();

		Noesis::GUI::SetXamlProvider(nullptr);
		Noesis::GUI::SetTextureProvider(nullptr);
		Noesis::GUI::SetFontProvider(nullptr);
		delete NoesisXamlProvider;
		NoesisXamlProvider = nullptr;
		delete NoesisTextureProvider;
		NoesisTextureProvider = nullptr;
		delete NoesisFontProvider;
		NoesisFontProvider = nullptr;

		NoesisRuntimeModuleInterface = 0;
		Noesis::GUI::Shutdown();
	}
	// End of IModuleInterface interface

	// INoesisRuntimeModuleInterface interface
	virtual void RegisterFont(class UFont* Font) override
	{
		NoesisFontProvider->RegisterFont(Font);
	}
	// End of INoesisRuntimeModuleInterface interface

	static INoesisRuntimeModuleInterface* NoesisRuntimeModuleInterface;
	FNoesisXamlProvider* NoesisXamlProvider;
	FNoesisTextureProvider* NoesisTextureProvider;
	FNoesisFontProvider* NoesisFontProvider;
	FDelegateHandle PostGarbageCollectConditionalBeginDestroyDelegateHandle;
	FDelegateHandle PostEngineInitDelegateHandle;
};

INoesisRuntimeModuleInterface* FNoesisRuntimeModule::NoesisRuntimeModuleInterface = 0;

INoesisRuntimeModuleInterface& INoesisRuntimeModuleInterface::Get()
{
	return *FNoesisRuntimeModule::NoesisRuntimeModuleInterface;
}

IMPLEMENT_MODULE(FNoesisRuntimeModule, NoesisRuntime);
DEFINE_LOG_CATEGORY(LogNoesis);
