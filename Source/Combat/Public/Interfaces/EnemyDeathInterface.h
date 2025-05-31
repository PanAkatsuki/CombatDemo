// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyDeathInterface.generated.h"

class UNiagaraSystem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyDeathInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBAT_API IEnemyDeathInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnEnemyDied(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem) = 0;
};
