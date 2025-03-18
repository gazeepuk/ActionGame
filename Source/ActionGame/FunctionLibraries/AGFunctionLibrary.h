// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "CoreTypes/AGEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGFunctionLibrary.generated.h"

class UAGGameInstance;
class UInputAction;
class UDataAsset_InputConfig;
struct FGameplayEffectSpecHandle;
class UAGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UAGAbilitySystemComponent* NativeGetAGAbilitySystemComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|GameplayTags")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|GameplayTags")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|GameplayTags", meta = (DisplayName = "DoesActorHaveTag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EAGConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|Combat", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EAGValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|Combat")
	static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|Utils", meta = (CompactNodeTitle = "GetValueAtLevel"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|Combat")
	static FGameplayTag ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|Combat")
	static bool IsValidBlock(const AActor* InAttacker, const AActor* InDefender);
	
	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|GameplayEffects")
	static  bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|Latent", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EAGCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EAGCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|Utils")
	static UInputAction* GetAbilityInputActionByTagFromInputConfigDataAsset(
		const UDataAsset_InputConfig* InInputConfigDataAsset, const FGameplayTag& InGameplayTag);

	UFUNCTION(BlueprintPure, Category = "ActionGame|FunctionLibrary|Utils", meta = (WorldContext = "WorldContextObject"))
	static UAGGameInstance* GetAGGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|Utils", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EAGInputMode InInputMode);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|SaveData")
	static void SaveCurrentGameDifficulty(EAGGameDifficulty InGameDifficulty);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|FunctionLibrary|SaveData")
	static bool TryLoadCurrentGameDifficulty(EAGGameDifficulty& OutGameDifficulty);
};
