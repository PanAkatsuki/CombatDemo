// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GenericTeamAgentInterface.h"

#include "CombatHeroController.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ACombatHeroController();

private:
	FGenericTeamId HeroTeamID;

public:
	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IPawnFightInterface Interface

};
