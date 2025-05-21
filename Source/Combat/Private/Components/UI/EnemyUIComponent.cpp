// Zhang All Rights Reserved.


#include "Components/UI/EnemyUIComponent.h"

#include "Widgets/CombatWidgetBase.h"


void UEnemyUIComponent::RegisterEnemyDrawnWidget(UCombatWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.AddUnique(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UCombatWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
