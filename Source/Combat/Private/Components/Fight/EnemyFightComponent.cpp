// Zhang All Rights Reserved.


#include "Components/Fight/EnemyFightComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"
#include "CombatFunctionLibrary.h"
#include "Characters/CombatEnemyCharacter.h"
#include "Components/BoxComponent.h"

#include "CombatDebugHelper.h"


void UEnemyFightComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);

	// Block Check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UCombatFunctionLibrary::NativeDoesActorHaveTag(HitActor, CombatGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;
	/*const bool bIsMyAttackUnblockable = UCombatFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), CombatGameplayTags::Enemy_Status_Unblockable);*/

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UCombatFunctionLibrary::IsValidBlock(GetOwner(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwner();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			CombatGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatGameplayTags::Shared_Event_MeleeHit, EventData);
	}
}

void UEnemyFightComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Super::OnWeaponPulledFromTargetActor(InteractedActor);


}

void UEnemyFightComponent::ToggleBodyAttackCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		checkf(
			false,
			TEXT("In EnemyFightComponent, Function ToggleBodyBoxCollision should not pass in ToggleDamageType with EToggleDamageType::CurrentEquippedWeapon")
		);
	}

	ACombatEnemyCharacter* OwningEnemyCharacter = Cast<ACombatEnemyCharacter>(GetOwner());
	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	check(LeftHandCollisionBox);
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();
	check(RightHandCollisionBox);

	switch (ToggleDamageType)
	{
	case EToggleDamageType::CurrentEquippedWeapon:
		break;
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::BothHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}

	if (!bShouldEnable)
	{
		AttackOverlappedActors.Empty();
	}
}
