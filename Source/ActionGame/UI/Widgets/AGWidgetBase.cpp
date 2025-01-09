// replcae later


#include "AGWidgetBase.h"

#include "Components/UI/EnemyUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

void UAGWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if(UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}

void UAGWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if(IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		if(UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent())
		{
			checkf(EnemyUIComponent, TEXT("Failed to find EnemyUIComponent from %s"), *GetNameSafe(OwningEnemyActor));

			BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent); 
		}
	}
}