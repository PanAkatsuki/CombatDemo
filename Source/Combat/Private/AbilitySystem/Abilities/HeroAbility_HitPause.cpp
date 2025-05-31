// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HitPause.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controllers/CombatHeroController.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


UHeroAbility_HitPause::UHeroAbility_HitPause()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set AbilityTriggerData
    FAbilityTriggerData AbilityTriggerData = FAbilityTriggerData();
    AbilityTriggerData.TriggerTag = CombatGameplayTags::Player_Event_HitPause;
    AbilityTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    AbilityTriggers.Add(AbilityTriggerData);

    // Set LatentInfo
    LatentInfo.CallbackTarget = this;
    LatentInfo.ExecutionFunction = FName("OnHitPauseDelayFinished");
    LatentInfo.Linkage = 0;
    LatentInfo.UUID = 1;
}

void UHeroAbility_HitPause::OnHitPauseDelayFinished()
{
    UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null in ActivateHitPauseAbility!"));

        return;
    }

    UGameplayStatics::SetGlobalTimeDilation(World, OriginalWorldDilation);

    GetHeroControllerFromActorInfo()->ClientStartCameraShake(CameraShakeClass);

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_HitPause::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ActivateHitPauseAbility();
}

void UHeroAbility_HitPause::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_HitPause::ActivateHitPauseAbility()
{
    check(GEngine);

    UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null in ActivateHitPauseAbility!"));

        return;
    }

    OriginalWorldDilation = World->GetWorldSettings()->TimeDilation;

    UGameplayStatics::SetGlobalTimeDilation(World, HitPauseTimeDilation);

    UKismetSystemLibrary::Delay(World, DelayDuration, LatentInfo);
}
