// replcae later


#include "DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FAGInputActionConfig& NativeInputAction : NativeInputActions)
	{
		if(NativeInputAction.InputTag == InInputTag && NativeInputAction.InputAction)
		{
			return NativeInputAction.InputAction;
		}
	}

	return nullptr;
}
