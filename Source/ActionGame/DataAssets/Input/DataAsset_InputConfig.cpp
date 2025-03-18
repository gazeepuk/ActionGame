// replcae later


#include "DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	return FindInputActionByTag(InInputTag, NativeInputActions);
}

UInputAction* UDataAsset_InputConfig::FindAbilityInputActionByTag(const FGameplayTag& InInputTag) const
{
	return FindInputActionByTag(InInputTag, AbilityInputActions);
}

UInputAction* UDataAsset_InputConfig::FindInputActionByTag(const FGameplayTag& InInputTag, const TArray<FAGInputActionConfig>& InInputActions) const
{
	for (const FAGInputActionConfig& InputAction : InInputActions)
	{
		if(InputAction.InputTag == InInputTag && InputAction.InputAction)
		{
			return InputAction.InputAction;
		}
	}

	return nullptr;
}
