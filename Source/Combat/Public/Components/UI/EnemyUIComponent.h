// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UCombatWidgetBase;

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
private:
	TArray<UCombatWidgetBase*> EnemyDrawnWidgets;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UCombatWidgetBase* InWidgetToRegister);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny();
};
