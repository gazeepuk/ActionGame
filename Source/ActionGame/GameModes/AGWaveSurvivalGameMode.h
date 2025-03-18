// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGBaseGameMode.h"
#include "AGWaveSurvivalGameMode.generated.h"

class AAGEnemyCharacter;

UENUM(BlueprintType)
enum class EAGWaveSurvivalGameModeState : uint8
{
	WaitToSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesCleared,
	PlayerDied
};

USTRUCT(BlueprintType)
struct FAGEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AAGEnemyCharacter> SoftEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawn = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawn = 3;
};

USTRUCT(BlueprintType)
struct FAGEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FAGEnemyWaveSpawnerInfo> EnemyWaveSpawnersDefinitions;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemySpawnThisWave = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveSurvivalGameModeStateChangedDelegate, EAGWaveSurvivalGameModeState, NewState);

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGWaveSurvivalGameMode : public AAGBaseGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemy(const TArray<AAGEnemyCharacter*>& InEnemiesToRegister);
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
private:
	virtual void Tick(float DeltaSeconds) override;

	void SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState InNewState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawningEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedEnemy);
	
	FAGEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	
	UPROPERTY()
	EAGWaveSurvivalGameModeState CurrentWaveSurvivalGameModeState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnWaveSurvivalGameModeStateChangedDelegate OnWaveSurvivalGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Definition", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> EnemyWaveSpawnerDataTable; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave Definition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Definition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	UPROPERTY()
	int32 TotalSpawnedEnemiesCounter = 0;
	
	UPROPERTY()
	float TimePassedSinceStateUpdated = 0.f;

	UPROPERTY()
	TArray<AActor*> TargetPointsArray;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Definition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Definition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Definition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap<TSoftClassPtr<AAGEnemyCharacter>, UClass*> PreloadedEnemyClassMap;
};
