// replcae later

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "AGEnhancedInputComponent.generated.h"

class UDataAsset_InputConfig;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);

};

template <class UserObject, typename CallbackFunc>
void UAGEnhancedInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
	const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input config Data Asset is nullptr"));

	UInputAction* FoundedAction = InInputConfig->FindNativeInputActionByTag(InInputTag);
	if(FoundedAction)
	{
		BindAction(FoundedAction, TriggerEvent, ContextObject, Func);
	}
}

template <class UserObject, typename CallbackFunc>
void UAGEnhancedInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,
	UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input config Data Asset is nullptr"));

	for (const FAGInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if(!AbilityInputActionConfig.IsValid())
		{
			continue;
		}

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	} 
}

