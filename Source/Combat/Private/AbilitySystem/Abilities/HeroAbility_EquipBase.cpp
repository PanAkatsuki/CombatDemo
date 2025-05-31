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
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/UI/HeroUIComponent.h"

#include "CombatDebugHelper.h"

UHeroAbility_EquipBase::UHeroAbility_EquipBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_EquipBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(MontageToPlay);
	SetWaitMontageEventTask(TagSet.WaitMontageEventTag);
}

void UHeroAbility_EquipBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_EquipBase::SetPlayMontageTask(UAnimMontage* InMontageToPlay)
{
	check(InMontageToPlay);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayEquipMontageTask"),
		InMontageToPlay,
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
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

void UHeroAbility_EquipBase::SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag)
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		InWaitMontageEventTag,
		nullptr,
		false,
		true
	);

	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitEventTask->ReadyForActivation();
}

void UHeroAbility_EquipBase::OnEventReceived(FGameplayEventData InEventData)
{
	
	HandleEquippedWeapon();
}

ACombatHeroWeapon* UHeroAbility_EquipBase::GetCurrentEquippedWeapon()
{
	UHeroFightComponent* HeroFightComponent = GetHeroFightComponentFromActorInfo();

	check(HeroFightComponent);

	return HeroFightComponent->GetHeroCurrentEquippedWeapon();
}

void UHeroAbility_EquipBase::HandleEquippedWeapon()
{
	AttachWeapon(TagSet.WeaponToEquipTag, AttachSocketName);
	RegisterCurrentEquippedWeapon(TagSet.WeaponToEquipTag);
	LinkAnimLayer();
	AddMappingContext();
	AssignWeaponAbilitySet();
	UpdateUISystem();
}

void UHeroAbility_EquipBase::AttachWeapon(FGameplayTag& InWeaponToEquipTag, FName& InAttachSocketName)
{
	ACombatHeroWeapon* Weapon = GetHeroFightComponentFromActorInfo()->GetHeroCarriedWeaponByTag(InWeaponToEquipTag);

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	Weapon->AttachToComponent(GetOwningComponentFromActorInfo(), AttachmentTransformRules, InAttachSocketName);
}

void UHeroAbility_EquipBase::RegisterCurrentEquippedWeapon(FGameplayTag& InWeaponToRegister)
{
	ACombatHeroWeapon* WeaponAxe = GetHeroFightComponentFromActorInfo()->GetHeroCarriedWeaponByTag(InWeaponToRegister);
	GetHeroFightComponentFromActorInfo()->SetCurrentEquippedWeaponAndTag(InWeaponToRegister, WeaponAxe);
}

void UHeroAbility_EquipBase::LinkAnimLayer()
{
	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();

	SkeletalMeshComponent->LinkAnimClassLayers(HeroWeapon->HeroWeaponData.WeaponAnimLayerToLink);
}

void UHeroAbility_EquipBase::AddMappingContext()
{
	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	check(HeroWeapon);

	ULocalPlayer* LocalPlayer = GetHeroCharacterFromActorInfo()->GetController<APlayerController>()->GetLocalPlayer();

	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(EnhancedInputLocalPlayerSubsystem);

	// AddMappingContext to EnhancedInputLocalPlayerSubsystem
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(HeroWeapon->HeroWeaponData.WeaponInputMappingContext, 1);
}

void UHeroAbility_EquipBase::AssignWeaponAbilitySet()
{
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = GetCombatAbilitySystemComponentFromActorInfo();

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandle;

	CombatAbilitySystemComponent->GrantHeroWeaponAbilities(
		GetCurrentEquippedWeapon()->HeroWeaponData.DefaultWeaponAbilities,
		GetCurrentEquippedWeapon()->HeroWeaponData.SpecialWeaponAbilities,
		GetAbilityLevel(),
		AbilitySpecHandle
	);

	GetCurrentEquippedWeapon()->AssignGrantedAbilitySpecHandles(AbilitySpecHandle);
}

void UHeroAbility_EquipBase::UpdateUISystem()
{
	ACombatHeroWeapon* HeroWeapon = GetHeroCharacterFromActorInfo()->GetHeroFightComponent()->GetHeroCurrentEquippedWeapon();
	GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnEquippedWeaponChanged.Broadcast(HeroWeapon->HeroWeaponData.SoftWeaponIconTexture);
}
