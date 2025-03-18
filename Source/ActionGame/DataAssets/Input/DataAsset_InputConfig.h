// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FAGInputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTags"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	bool IsValid() const { return InputTag.IsValid() && InputAction; }
};

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
	UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InInputTag) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FAGInputActionConfig> NativeInputActions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FAGInputActionConfig> AbilityInputActions;
private:
	UInputAction* FindInputActionByTag(const FGameplayTag& InInputTag, const TArray<FAGInputActionConfig>& InInputActions) const;
};