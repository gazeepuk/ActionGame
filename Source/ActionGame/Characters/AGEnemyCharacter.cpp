// replcae later


#include "AGEnemyCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "FunctionLibraries/AGFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/AGBaseGameMode.h"
#include "UI/Widgets/AGWidgetBase.h"
#include "Utils/AGDebugHelper.h"


AAGEnemyCharacter::AAGEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
}

UPawnCombatComponent* AAGEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AAGEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AAGEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AAGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UAGWidgetBase* HealthBarWidget = Cast<UAGWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject());
	if(HealthBarWidget)
	{
		HealthBarWidget->InitEnemyCreatedWidget(this);
	}
}

void AAGEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AAGEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	int32 AbilityApplyLevel = 1;
			
	if(AAGBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AAGBaseGameMode>())
	{
		switch (BaseGameMode->GetCurrentGameDifficulty())
		{
		case EAGGameDifficulty::Easy:
			AbilityApplyLevel = 1;
			break;
		case EAGGameDifficulty::Normal:
			AbilityApplyLevel = 2;
			break;
		case EAGGameDifficulty::Hard:
			AbilityApplyLevel = 3;
			break;
		case EAGGameDifficulty::VeryHard:
			AbilityApplyLevel = 4;
			break;
		default:
			break;
		}
	}
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]()
			{
				if(UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(AGAbilitySystemComponent, AbilityApplyLevel);
				}
			}
		)
	);
}
