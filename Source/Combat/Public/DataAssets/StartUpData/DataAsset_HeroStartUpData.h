// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "CombatTypes/CombatStructTypes.h"

#include "DataAsset_HeroStartUpData.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FCombatHeroAbilitySets> HeroStartUpAbilitySet;

public:
	virtual void GiveDataToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

};
