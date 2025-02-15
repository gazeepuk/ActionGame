// replcae later


#include "EnemyUIComponent.h"

#include "UI/Widgets/AGWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UAGWidgetBase* InWidgetToRegister)
{
	if(InWidgetToRegister)
	{
		EnemyDrawnWidgets.AddUnique(InWidgetToRegister);
	}
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgets()
{
	if(!EnemyDrawnWidgets.IsEmpty())
	{
		for (UAGWidgetBase* EnemyWidget : EnemyDrawnWidgets)
		{
			EnemyWidget->RemoveFromParent();
		}

		EnemyDrawnWidgets.Empty();
	}
}
