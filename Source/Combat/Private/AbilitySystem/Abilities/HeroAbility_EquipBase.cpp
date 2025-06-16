// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_EquipBase.h"

#include "Components/Fight/HeroFightComponent.h"
#include "CombatGameplayTags.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "AnimInstances/Hero/CombatHeroLinkedAnimLayer.h"
#include "Characters/CombatHeroCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_EquipBase::UHeroAbility_EquipBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_EquipBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(this, FName("EquipMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));
	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_Equip_Axe);
}

void UHeroAbility_EquipBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_EquipBase::AttachWeaponToSocket(ACombatWeaponBase* InWeapon, USkeletalMeshComponent* InSkeletalMeshComponent, FName InSocketNameToAttachTo, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool WeldSimulatedBodies)
{
	IAttachWeaponInterface::AttachWeaponToSocket(InWeapon, InSkeletalMeshComponent, InSocketNameToAttachTo, LocationRule, RotationRule, ScaleRule, WeldSimulatedBodies);
}

void UHeroAbility_EquipBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_EquipBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_EquipBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_EquipBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_EquipBase::OnEventReceived(FGameplayEventData InEventData)
{
	ACombatHeroWeapon* Weapon = GetHeroFightComponentFromActorInfo()->GetHeroCarriedWeaponByTag(TagSet.WeaponToEquipTag);

	// Attach
	AttachWeaponToSocket(Weapon, GetOwningComponentFromActorInfo(), AttachSocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

	// Set Tag And Weapon In FightComponent
	GetHeroFightComponentFromActorInfo()->SetCurrentEquippedWeaponAndTag(TagSet.WeaponToEquipTag, Weapon);

	// Link Anim
	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();
	SkeletalMeshComponent->LinkAnimClassLayers(Weapon->HeroWeaponData.WeaponAnimLayerToLink);

	// AddMappingContext to EnhancedInputLocalPlayerSubsystem
	ULocalPlayer* LocalPlayer = GetHeroCharacterFromActorInfo()->GetController<APlayerController>()->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputLocalPlayerSubsystem);
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(Weapon->HeroWeaponData.WeaponInputMappingContext, 1);

	// Assign Weapon Abilities
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = GetCombatAbilitySystemComponentFromActorInfo();

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandle;

	CombatAbilitySystemComponent->GrantHeroWeaponAbilities(
		Weapon->HeroWeaponData.DefaultWeaponAbilities,
		Weapon->HeroWeaponData.SpecialWeaponAbilities,
		GetAbilityLevel(),
		AbilitySpecHandle
	);

	GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeapon()->AssignGrantedAbilitySpecHandles(AbilitySpecHandle);

	// Set Weapon Icon
	GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnEquippedWeaponChanged.Broadcast(Weapon->HeroWeaponData.SoftWeaponIconTexture);

	// Set Abilities Icon And Cooldown
	for (FCombatHeroSpecialAbilitySet SpecialAbilitySet : Weapon->HeroWeaponData.SpecialWeaponAbilities)
	{
		GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnAbilityIconSlotUpdated.Broadcast(
			SpecialAbilitySet.InputTag,
			SpecialAbilitySet.SoftAbilityIconMaterial
		);

		float TotalCooldownTime = 0.f;
		float RemainingCooldownTime = 0.f;
		if (GetAbilityRemainingCooldownByTag(SpecialAbilitySet.AbilityCooldownTag, TotalCooldownTime, RemainingCooldownTime))
		{
			GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnAbilityCooldownBegin.Broadcast(
				SpecialAbilitySet.InputTag,
				TotalCooldownTime,
				RemainingCooldownTime
			);
		}
	}
}
