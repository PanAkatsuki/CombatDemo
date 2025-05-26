// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/SharedAbility_SpawnWeapon.h"

#include "Items/Weapons/CombatWeaponBase.h"
#include "Components/Fight/PawnFightComponent.h"


USharedAbility_SpawnWeapon::USharedAbility_SpawnWeapon()
{
	AbilityActivationPolicy = ECombatAbilityActivationPolicy::OnGiven;
}

void USharedAbility_SpawnWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(GetWorld());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
	SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ACombatWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<ACombatWeaponBase>(WeaponToSpawn, FVector(), FRotator(), SpawnParams);

	check(SpawnedWeapon);

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	SpawnedWeapon->AttachToComponent(GetOwningComponentFromActorInfo(), AttachmentTransformRules, SocketNameToAttachTo);

	GetPawnFightComponentFromActorInfo()->RegisterSpawnedWeapon(WeaponTagToRigister, SpawnedWeapon, RegisterAsEquippedWeapon);

	SpawnedWeapon->GetWeaponMesh()->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), 1.0f);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void USharedAbility_SpawnWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}