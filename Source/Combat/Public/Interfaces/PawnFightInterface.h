// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnFightInterface.generated.h"

class UPawnFightComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnFightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBAT_API IPawnFightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Should override in child class
	virtual UPawnFightComponent* GetPawnFightComponent() const = 0;
};
