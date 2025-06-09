// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_SummonEnemies.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "CombatGameplayTags.h"
#include "Characters/CombatEnemyCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/CombatSurvialGameMode.h"

#include "CombatDebugHelper.h"


UEnemyAbility_SummonEnemies::UEnemyAbility_SummonEnemies()
{
}

void UEnemyAbility_SummonEnemies::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask();
	SetWaitSpawnEventTask();
}

void UEnemyAbility_SummonEnemies::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UEnemyAbility_SummonEnemies::SetPlayMontageTask()
{
	check(MontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlaySummonMontageTask"),
		FindMontageToPlay(),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UEnemyAbility_SummonEnemies::FindMontageToPlay()
{
	int32 RandomInt = FMath::RandRange(1, MontagesMap.Num());
	UAnimMontage* const* MontagePtr = MontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UEnemyAbility_SummonEnemies::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_SummonEnemies::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_SummonEnemies::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_SummonEnemies::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_SummonEnemies::SetWaitSpawnEventTask()
{
	UAbilityTask_WaitSpawnEnemies* WaitSpawnTask = UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(
		this,
		CombatGameplayTags::Enemy_Event_SummonEnemies,
		EnemyClassToSpawn,
		NumToSpawn,
		GetEnemyCharacterFromActorInfo()->GetActorLocation(),
		RandomSpawnRadius
	);

	WaitSpawnTask->OnSpawnFinished.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitSpawnTask->ReadyForActivation();
}

void UEnemyAbility_SummonEnemies::OnEventReceived(const TArray<ACombatEnemyCharacter*>& SpawnedEnemies)
{
	UBlackboardComponent* BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(GetEnemyCharacterFromActorInfo());
	BlackBoard->SetValueAsBool(FName("HasSpawnedEnemies"), true);

	/*check(GetWorld());
	ACombatSurvialGameMode* GameMode = Cast<ACombatSurvialGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	check(GameMode);
	GameMode->RegisterSpawnedEnemies(SpawnedEnemies);*/
}
