// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_Projectile.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/CombatEnemyCharacter.h"
#include "Items/CombatProjectileBase.h"

#include "CombatDebugHelper.h"


UEnemyAbility_Projectile::UEnemyAbility_Projectile()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEnemyAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(MontagesMap);
	SetWaitMontageEventTask(TagSet.WaitMontageEventTag);
}

void UEnemyAbility_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UEnemyAbility_Projectile::SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	check(InMontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayProjectileAttackMontageTask"),
		FindMontageToPlay(InMontagesMap),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UEnemyAbility_Projectile::FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	int32 RandomInt = FMath::RandRange(1, InMontagesMap.Num());
	UAnimMontage* const* MontagePtr = InMontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UEnemyAbility_Projectile::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_Projectile::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_Projectile::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_Projectile::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_Projectile::SetWaitMontageEventTask(FGameplayTag& InEventTag)
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		InEventTag,
		nullptr,
		false,
		true
	);

	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitEventTask->ReadyForActivation();
}

void UEnemyAbility_Projectile::OnEventReceived(FGameplayEventData InEventData)
{
	FTransform Transform;
	Transform.SetLocation(GetOwningComponentFromActorInfo()->GetSocketLocation(FName("ProjectileFireSocket")));
	Transform.SetRotation(GetEnemyCharacterFromActorInfo()->GetActorQuat());
	Transform.SetScale3D(FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ActorSpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	check(GetWorld());
	AActor* ProjectileActor = GetWorld()->SpawnActor(ProjectileToSpawn, &Transform, ActorSpawnParameters);

	ACombatProjectileBase* Projectile = Cast<ACombatProjectileBase>(ProjectileActor);
	check(Projectile);
	Projectile->SetOwner(GetEnemyCharacterFromActorInfo());
	Projectile->SetInstigator(GetEnemyCharacterFromActorInfo());

	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeEnemyDamageEffectSpecHandle(DealDamageEffect, ScalableFloat);
	Projectile->SetProjectileDamageEffectSpecHandle(DamageEffectSpecHandle);
}