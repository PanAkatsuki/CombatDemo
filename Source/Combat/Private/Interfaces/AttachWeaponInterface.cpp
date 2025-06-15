// Zhang All Rights Reserved.


#include "Interfaces/AttachWeaponInterface.h"

#include "Items/Weapons/CombatWeaponBase.h"


// Add default functionality here for any IAttachWeaponInterface functions that are not pure virtual.
void IAttachWeaponInterface::AttachWeaponToSocket(ACombatWeaponBase* InWeapon, USkeletalMeshComponent* InSkeletalMeshComponent, FName InSocketNameToAttachTo, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool WeldSimulatedBodies)
{
	FAttachmentTransformRules AttachmentTransformRules(LocationRule, RotationRule, ScaleRule, WeldSimulatedBodies);
	InWeapon->AttachToComponent(InSkeletalMeshComponent, AttachmentTransformRules, InSocketNameToAttachTo);
}
