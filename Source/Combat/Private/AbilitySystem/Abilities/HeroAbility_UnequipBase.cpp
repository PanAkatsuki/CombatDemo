// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_UnequipBase.h"

#include "Components/Fight/HeroFightComponent.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "AnimInstances/Hero/CombatHeroLinkedAnimLayer.h"
#include "Characters/CombatHeroCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/UI/HeroUIComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_UnequipBase::UHeroAbility_UnequipBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_UnequipBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(MontageToPlay);
	SetWaitMontageEventTask(WaitMontageEventTag);
}
void UHeroAbility_UnequipBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_UnequipBase::SetPlayMontageTask(UAnimMontage* InMontageToPlay)
{
	check(InMontageToPlay);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayUnequipMontageTask"),
		InMontageToPlay,
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
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

void UHeroAbility_UnequipBase::SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag)
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

void UHeroAbility_UnequipBase::OnEventReceived(FGameplayEventData InEventData)
{
	HandleUnequippedWeapon();
}

void UHeroAbility_UnequipBase::HandleUnequippedWeapon()
{
	AttachWeapon(AttachSocketName);
	UnlinkAnimLayer();
	RemoveMappingContext();
	RemoveWeaponAbilitySet();
	UpdateUISystem();
	DeleteRegisterInfo();// Should put at last line in UHeroAbility_UnequipBase::HandleUnequippedWeapon()
}

void UHeroAbility_UnequipBase::AttachWeapon(FName& InAttachSocketName)
{
	ACombatHeroWeapon* Weapon = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeapon();

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	Weapon->AttachToComponent(GetOwningComponentFromActorInfo(), AttachmentTransformRules, InAttachSocketName);
}

ACombatHeroWeapon* UHeroAbility_UnequipBase::GetCurrentEquippedWeapon()
{
	UHeroFightComponent* HeroFightComponent = GetHeroFightComponentFromActorInfo();

	check(HeroFightComponent);

	return HeroFightComponent->GetHeroCurrentEquippedWeapon();
}

void UHeroAbility_UnequipBase::UnlinkAnimLayer()
{
	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();

	SkeletalMeshComponent->UnlinkAnimClassLayers(HeroWeapon->HeroWeaponData.WeaponAnimLayerToLink);
}

void UHeroAbility_UnequipBase::RemoveMappingContext()
{
	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	check(HeroWeapon);

	ULocalPlayer* LocalPlayer = GetHeroCharacterFromActorInfo()->GetController<APlayerController>()->GetLocalPlayer();

	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(EnhancedInputLocalPlayerSubsystem);

	// RemoveMappingContext to EnhancedInputLocalPlayerSubsystem
	EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(HeroWeapon->HeroWeaponData.WeaponInputMappingContext);
}

void UHeroAbility_UnequipBase::RemoveWeaponAbilitySet()
{
	TArray<FGameplayAbilitySpecHandle>& AbilitySpecHandles = GetCurrentEquippedWeapon()->GetGrantedAbilitySpecHandles();

	GetCombatAbilitySystemComponentFromActorInfo()->RemoveGrantHeroWeaponAbilities(
		AbilitySpecHandles
	);
}

void UHeroAbility_UnequipBase::UpdateUISystem()
{
	ACombatHeroWeapon* HeroWeapon = GetHeroCharacterFromActorInfo()->GetHeroFightComponent()->GetHeroCurrentEquippedWeapon();
	GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnEquippedWeaponChanged.Broadcast(TSoftObjectPtr<UTexture2D>());
}

void UHeroAbility_UnequipBase::DeleteRegisterInfo()
{
	GetHeroFightComponentFromActorInfo()->SetCurrentEquippedWeaponAndTag(FGameplayTag(), nullptr);
}
