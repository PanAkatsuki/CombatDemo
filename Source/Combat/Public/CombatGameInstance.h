// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GameplayTagContainer.h"

#include "CombatGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FCombatGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void OnPreLoadMap(const FString& MapName);
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);

public:
	virtual void Init() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCombatGameLevelSet> GameLevelSets;

public:
	UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag);
};
