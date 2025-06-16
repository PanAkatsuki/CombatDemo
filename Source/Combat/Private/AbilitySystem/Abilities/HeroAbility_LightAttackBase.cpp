// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_LightAttackBase.h"

#include "Characters/CombatHeroCharacter.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "Components/Fight/HeroFightComponent.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/CombatProjectileBase.h"

#include "CombatDebugHelper.h"


UHeroAbility_LightAttackBase::UHeroAbility_LightAttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	OnRageStateLightAttackEndDelegate.AddDynamic(this, &ThisClass::OnRageStateAttackEnd);
}

void UHeroAbility_LightAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Hero = Cast<AActor>(GetHeroCharacterFromActorInfo());
	check(Hero);

	// Initialize Timer
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, AbilityEndTimerHandle);

	// Initialize Combo Count
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(Hero, CombatGameplayTags::Player_Status_JumpToFinisher))
	{
		CurrentAttackComboCount = AnimMontagesMap.Num();
	}

	UsedAttackComboCount = CurrentAttackComboCount;

	// Set Play Montage Task
	SetPlayMontageTask(this, FName("LightAttackMontageTask"), FindMontageToPlayWithKey(AnimMontagesMap, CurrentAttackComboCount));

	// Set Wait Event Task
	SetWaitMontageEventTask(this, CombatGameplayTags::Shared_Event_MeleeHit);

	// Update Current Attack Combo Count
	if (CurrentAttackComboCount >= AnimMontagesMap.Num())
	{
		ResetCurrentAttackComboCount();
		UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Hero, CombatGameplayTags::Player_Status_JumpToFinisher);
	}
	else
	{
		// Should Add Tag Jump to finisher
		if (CurrentAttackComboCount == AnimMontagesMap.Num() - 1)
		{
			UCombatFunctionLibrary::AddGameplayTagToActorIfNone(Hero, CombatGameplayTags::Player_Status_JumpToFinisher);
		}
		else
		{
			UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Hero, CombatGameplayTags::Player_Status_JumpToFinisher);
		}

		CurrentAttackComboCount++;
	}

	// Check Rage State And Broadcast
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Active))
	{
		OnRageStateLightAttackEndDelegate.Broadcast();
	}
}

void UHeroAbility_LightAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

UAnimMontage* UHeroAbility_LightAttackBase::FindMontageToPlayWithKey(TMap<int32, UAnimMontage*>& InAnimMontagesMap, int32 InKey)
{
	UAnimMontage* const* MontagePtr = AnimMontagesMap.Find(CurrentAttackComboCount);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_LightAttackBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_LightAttackBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_LightAttackBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_LightAttackBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_LightAttackBase::OnEventReceived(FGameplayEventData InEventData)
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
		CombatGameplayTags::Player_SetByCaller_AttackType_Light,
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

void UHeroAbility_LightAttackBase::OnAbilityEndTimerFinished()
{
	ResetCurrentAttackComboCount();

	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(CurrentActorInfo->AvatarActor.Get(), CombatGameplayTags::Player_Status_JumpToFinisher);
}

void UHeroAbility_LightAttackBase::OnRageStateAttackEnd()
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

void UHeroAbility_LightAttackBase::OnSpawnProjectileEventReceived(FGameplayEventData InEventData)
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

void UHeroAbility_LightAttackBase::ResetCurrentAttackComboCount()
{
	CurrentAttackComboCount = 1;
}