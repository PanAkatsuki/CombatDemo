// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_UnequipBase.h"

#include "Components/Fight/HeroFightComponent.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "AnimInstances/Hero/CombatHeroLinkedAnimLayer.h"
#include "Characters/CombatHeroCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


UHeroAbility_UnequipBase::UHeroAbility_UnequipBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_UnequipBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(this, FName("UnequipMontageTask"), FindMontageToPlay(AnimMontagesMap));
	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_Unequip_Axe);
}
void UHeroAbility_UnequipBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_UnequipBase::AttachWeaponToSocket(ACombatWeaponBase* InWeapon, USkeletalMeshComponent* InSkeletalMeshComponent, FName InSocketNameToAttachTo, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool WeldSimulatedBodies)
{
	IAttachWeaponInterface::AttachWeaponToSocket(InWeapon, InSkeletalMeshComponent, InSocketNameToAttachTo, LocationRule, RotationRule, ScaleRule, WeldSimulatedBodies);
}

void UHeroAbility_UnequipBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_UnequipBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_UnequipBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_UnequipBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_UnequipBase::OnEventReceived(FGameplayEventData InEventData)
{
	ACombatHeroWeapon* Weapon = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeapon();

	// Attach
	AttachWeaponToSocket(Weapon, GetOwningComponentFromActorInfo(), AttachSocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

	// Unlink Anim Layers
	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();
	SkeletalMeshComponent->UnlinkAnimClassLayers(Weapon->HeroWeaponData.WeaponAnimLayerToLink);

	// Remove Mapping Context
	ULocalPlayer* LocalPlayer = GetHeroCharacterFromActorInfo()->GetController<APlayerController>()->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputLocalPlayerSubsystem);
	EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(Weapon->HeroWeaponData.WeaponInputMappingContext);

	// Remove Weapon Abilities
	TArray<FGameplayAbilitySpecHandle>& AbilitySpecHandles = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeapon()->GetGrantedAbilitySpecHandles();

	GetCombatAbilitySystemComponentFromActorInfo()->RemoveGrantHeroWeaponAbilities(
		AbilitySpecHandles
	);

	// Set Weapon Icon
	GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnEquippedWeaponChanged.Broadcast(TSoftObjectPtr<UTexture2D>());

	// Set Abilities Icon
	for (FCombatHeroSpecialAbilitySet SpecialAbilitySet : Weapon->HeroWeaponData.SpecialWeaponAbilities)
	{
		GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnAbilityIconSlotUpdated.Broadcast(
			SpecialAbilitySet.InputTag,
			TSoftObjectPtr<UMaterialInterface>()
		);
	}

	// Delete Weapon Tag And Weapon In Fight Component
	GetHeroFightComponentFromActorInfo()->SetCurrentEquippedWeaponAndTag(FGameplayTag(), nullptr); // Should put at last line
}
