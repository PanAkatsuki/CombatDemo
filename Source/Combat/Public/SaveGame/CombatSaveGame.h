// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "CombatTypes/CombatEnumTypes.h"

#include "CombatSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	ECombatGameDifficulty SavedCurrentGameDifficuly = ECombatGameDifficulty::Easy;

};
