// replcae later


#include "AGWaveSurvivalGameMode.h"

#include "NavigationSystem.h"
#include "Characters/AGEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void AAGWaveSurvivalGameMode::RegisterSpawnedEnemy(const TArray<AAGEnemyCharacter*>& InEnemiesToRegister)
{
	for (AAGEnemyCharacter* EnemyToRegister : InEnemiesToRegister)
	{
		if(EnemyToRegister)
		{
			CurrentSpawnedEnemiesCounter++;

			EnemyToRegister->OnDestroyed.AddUniqueDynamic(this, &ThisClass::AAGWaveSurvivalGameMode::OnEnemyDestroyed);
		}
	}
}

void AAGWaveSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("Enemy Wave Spawner Data Table is not assigned"));

	SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState::WaitToSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AAGWaveSurvivalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(CurrentWaveSurvivalGameModeState == EAGWaveSurvivalGameModeState::WaitToSpawnNewWave)
	{
		TimePassedSinceStateUpdated += DeltaSeconds;

		if(TimePassedSinceStateUpdated >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStateUpdated = 0.f;

			SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState::SpawningNewWave);
		}
	}

	if(CurrentWaveSurvivalGameModeState == EAGWaveSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStateUpdated += DeltaSeconds;

		if(TimePassedSinceStateUpdated >= SpawnEnemiesDelayTime)
		{
			const int32 SpawnedEnemiesAmount = TrySpawnWaveEnemies();
			
			CurrentSpawnedEnemiesCounter += SpawnedEnemiesAmount;
			
			TimePassedSinceStateUpdated = 0.f;
			
			SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState::InProgress);
		}
	}

	if(CurrentWaveSurvivalGameModeState == EAGWaveSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStateUpdated += DeltaSeconds;
		
		if(TimePassedSinceStateUpdated >= WaveCompletedWaitTime)
		{
			TimePassedSinceStateUpdated = 0.f;

			CurrentWaveCount++;
			
			if(HasFinishedAllWaves())
			{
				SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState::AllWavesCleared);
			}
			else
			{
				PreLoadNextWaveEnemies();
				SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState::WaitToSpawnNewWave);
			}
		}
	}
}

void AAGWaveSurvivalGameMode::SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState InNewState)
{
	CurrentWaveSurvivalGameModeState = InNewState;

	OnWaveSurvivalGameModeStateChanged.Broadcast(CurrentWaveSurvivalGameModeState);
}

bool AAGWaveSurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void AAGWaveSurvivalGameMode::PreLoadNextWaveEnemies()
{
	if(HasFinishedAllWaves())
	{
		return;
	}

	PreloadedEnemyClassMap.Empty();
	
	for (const FAGEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnersDefinitions)
	{
		if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([&SpawnerInfo, this]()
			{
				if(UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
				{
					if(!PreloadedEnemyClassMap.Contains(SpawnerInfo.SoftEnemyClassToSpawn))
					{
						PreloadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				}
			}));
	}
}

int32 AAGWaveSurvivalGameMode::TrySpawnWaveEnemies()
{
	if(TargetPointsArray.IsEmpty() || !ShouldKeepSpawningEnemies())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}

	checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid Target Points for spawning Enemies"));

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (const FAGEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnersDefinitions)
	{
		if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawn, SpawnerInfo.MaxPerSpawn);

		UClass* LoadedEnemyClass = PreloadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; ++i)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);

			const FVector SpawnLocation = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnLocation, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f);

			AAGEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AAGEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParameters);
			if(SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
				
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesCounter++;
			}

			if(!ShouldKeepSpawningEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
}

bool AAGWaveSurvivalGameMode::ShouldKeepSpawningEnemies() const
{
	return TotalSpawnedEnemiesCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemySpawnThisWave;
}

void AAGWaveSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedEnemy) 
{
	CurrentSpawnedEnemiesCounter--;
	if(ShouldKeepSpawningEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if(CurrentSpawnedEnemiesCounter <= 0)
	{
		TotalSpawnedEnemiesCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;

		SetCurrentWaveSurvivalGameModeState(EAGWaveSurvivalGameModeState::WaveCompleted);
	}
}

FAGEnemyWaveSpawnerTableRow* AAGWaveSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
	FAGEnemyWaveSpawnerTableRow* CurrentWaveSpawnerTableRow =  EnemyWaveSpawnerDataTable->FindRow<FAGEnemyWaveSpawnerTableRow>(RowName, FString());

	checkf(CurrentWaveSpawnerTableRow, TEXT("Can not find a valid Spawner TableRow {%s} for Current Wave {%d}"), *RowName.ToString(), CurrentWaveCount);

	return CurrentWaveSpawnerTableRow;
}
