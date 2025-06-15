// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttachWeaponInterface.generated.h"

class ACombatWeaponBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttachWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBAT_API IAttachWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttachWeaponToSocket(ACombatWeaponBase* InWeapon, USkeletalMeshComponent* InSkeletalMeshComponent, FName InSocketNameToAttachTo, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool WeldSimulatedBodies);
};
