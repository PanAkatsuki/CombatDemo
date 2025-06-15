// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Death.h"

#include "CombatGameplayTags.h"
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

	SetPlayMontageTask(this, FName("DeathMontageTask"), FindMontageToPlay(AnimMontagesMap));
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
		SurvialGameMode->OnSurvialGameModeStateChanged.Broadcast(ECombatSurvialGameModeState::PlayerDied);
	}

	if (ACombatHeroController* HeroController = GetHeroControllerFromActorInfo())
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetHeroControllerFromActorInfo());
		GetHeroControllerFromActorInfo()->bShowMouseCursor = false;
	}
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