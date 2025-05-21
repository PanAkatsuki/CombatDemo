// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CombatTypes/CombatEnumTypes.h"

#include "CombatBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACombatBaseGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	ECombatGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE ECombatGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

};
