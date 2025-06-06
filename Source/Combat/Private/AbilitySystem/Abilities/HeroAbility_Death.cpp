// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Death.h"

#include "CombatGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/CombatHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/CombatSurvialGameMode.h"
#include "Controllers/CombatHeroController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "CombatDebugHelper.h"


UHeroAbility_Death::UHeroAbility_Death()
{
	// Set AbilityTriggerData
	FAbilityTriggerData AbilityTriggerData = FAbilityTriggerData();
	AbilityTriggerData.TriggerTag = CombatGameplayTags::Shared_Status_Death;
	AbilityTriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	AbilityTriggers.Add(AbilityTriggerData);
}

void UHeroAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(MontagesMap);
}

void UHeroAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetOwningComponentFromActorInfo()->bPauseAnims = true;
	GetHeroCharacterFromActorInfo()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	check(GetWorld());
	ACombatSurvialGameMode* SurvialGameMode = Cast<ACombatSurvialGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (SurvialGameMode)
	{
		//SurvialGameMode->OnSurvialGameModeStateChanged.Broadcast()
		//todo
	}

	check(GetHeroControllerFromActorInfo());
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetHeroControllerFromActorInfo());
	GetHeroControllerFromActorInfo()->bShowMouseCursor = false;
}

void UHeroAbility_Death::SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	check(MontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayHeroDeathMontageTask"),
		FindMontageToPlay(InMontagesMap),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UHeroAbility_Death::FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	int32 RandomInt = FMath::RandRange(1, InMontagesMap.Num());

	UAnimMontage* const* MontagePtr = InMontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_Death::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Death::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Death::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_Death::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}