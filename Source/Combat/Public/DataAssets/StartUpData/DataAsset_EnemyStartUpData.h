// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UCombatEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class COMBAT_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UCombatEnemyGameplayAbility>> EnemyFightAbilities;

public:
	virtual void GiveDataToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
};
