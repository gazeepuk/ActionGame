// replcae later

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AGAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class ACTIONGAME_API AAGAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAGAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	// CrowdFollowingComponent properties 
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance")
	bool bEnableDetourCrowdAvoidance = true;
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};
