// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HeavyAttackBase.h"

#include "CombatFunctionLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "CombatGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Fight/HeroFightComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/CombatProjectileBase.h"

#include "CombatDebugHelper.h"


UHeroAbility_HeavyAttackBase::UHeroAbility_HeavyAttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	OnRageStateHeavyAttackEndDelegate.AddDynamic(this, &ThisClass::OnRageStateAttackEnd);
}

void UHeroAbility_HeavyAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Initialize Timer
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, AbilityEndTimerHandle);

	// Initialize Combo Count
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher))
	{
		CurrentAttackComboCount = AnimMontagesMap.Num();
	}

	UsedAttackComboCount = CurrentAttackComboCount;

	// Set Play Montage Task
	SetPlayMontageTask(this, FName("HeavyAttackMontageTask"), FindMontageToPlayWithKey(AnimMontagesMap, CurrentAttackComboCount));
	
	// Set Wait Montage Task
	SetWaitMontageEventTask(this, CombatGameplayTags::Shared_Event_MeleeHit);

	// Update Attack Combo Count
	if (CurrentAttackComboCount == AnimMontagesMap.Num())
	{
		ResetCurrentAttackComboCount();
	}
	else
	{
		CurrentAttackComboCount++;
	}

	// Check Rage State And Broadcast
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Active))
	{
		OnRageStateHeavyAttackEndDelegate.Broadcast();
	}
}
void UHeroAbility_HeavyAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);

	// Set Timer
	AbilityEndTimerDelegate.BindUObject(this, &ThisClass::OnAbilityEndTimerFinished);

	GetWorld()->GetTimerManager().SetTimer(
		AbilityEndTimerHandle,
		AbilityEndTimerDelegate,
		0.6f,
		false
	);
}

UAnimMontage* UHeroAbility_HeavyAttackBase::FindMontageToPlayWithKey(TMap<int32, UAnimMontage*>& InAnimMontagesMap, int32 InKey)
{
	UAnimMontage* const* MontagePtr = AnimMontagesMap.Find(InKey);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_HeavyAttackBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_HeavyAttackBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_HeavyAttackBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_HeavyAttackBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_HeavyAttackBase::OnEventReceived(FGameplayEventData InEventData)
{
	// Play Gameplay Cue
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TagSet.WeaponHitSoundGameplayCueTag, GameplayCueParameters);

	// Make Damage Effect Spec Handle
	float BaseWeaponDamage = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		EffectSet.DealDamageEffectClass,
		BaseWeaponDamage,
		CombatGameplayTags::Player_SetByCaller_AttackType_Heavy,
		UsedAttackComboCount
	);

	// Apply Effect Spec Handle
	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, GameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, CombatGameplayTags::Shared_Event_HitReact, InEventData);
		ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSet.GainRageEffectClass.GetDefaultObject(), GetAbilityLevel());
	}
}

void UHeroAbility_HeavyAttackBase::OnAbilityEndTimerFinished()
{
	ResetCurrentAttackComboCount();
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(CurrentActorInfo->AvatarActor.Get(), CombatGameplayTags::Player_Status_JumpToFinisher);
}

void UHeroAbility_HeavyAttackBase::OnRageStateAttackEnd()
{
	UAbilityTask_WaitGameplayEvent* WaitSpawnProjectileEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Shared_Event_SpawnProjectile,
		nullptr,
		false,
		true
	);

	WaitSpawnProjectileEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnSpawnProjectileEventReceived);

	WaitSpawnProjectileEventTask->ReadyForActivation();
}

void UHeroAbility_HeavyAttackBase::OnSpawnProjectileEventReceived(FGameplayEventData InEventData)
{
	FTransform SocketTransform = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeapon()->GetWeaponMesh()->GetSocketTransform(
		FName("RageSlashSocket")
	);

	FVector SpawnLocation = SocketTransform.GetLocation();
	SpawnLocation.Z += 25.f;

	FVector ForwardVector = GetHeroCharacterFromActorInfo()->GetActorForwardVector();
	FRotator SpawnRotation = FRotator(0.0f, UKismetMathLibrary::MakeRotFromX(ForwardVector).Yaw, SocketTransform.GetRotation().Rotator().Roll);

	FTransform Transform;
	Transform.SetLocation(SpawnLocation);
	Transform.SetRotation(SpawnRotation.Quaternion());

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ActorSpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	check(GetWorld());
	AActor* ProjectileActor = GetWorld()->SpawnActor(ProjectileClass, &Transform, ActorSpawnParameters);

	ACombatProjectileBase* Projectile = Cast<ACombatProjectileBase>(ProjectileActor);
	Projectile->SetOwner(GetHeroCharacterFromActorInfo());
	Projectile->SetInstigator(GetHeroCharacterFromActorInfo());

	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		EffectSet.DealDamageEffectClass,
		GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel()),
		CombatGameplayTags::Player_SetByCaller_AttackType_Light,
		UsedAttackComboCount
	);
	Projectile->SetProjectileDamageEffectSpecHandle(GameplayEffectSpecHandle);
}

void UHeroAbility_HeavyAttackBase::ResetCurrentAttackComboCount()
{
	CurrentAttackComboCount = 1;
}