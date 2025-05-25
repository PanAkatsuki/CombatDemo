// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HitPause.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controllers/CombatHeroController.h"

#include "CombatDebugHelper.h"


UHeroAbility_HitPause::UHeroAbility_HitPause()
{
    LatentInfo.CallbackTarget = this;
    LatentInfo.ExecutionFunction = FName("OnHitPauseDelayFinished");
    LatentInfo.Linkage = 0;
    LatentInfo.UUID = 1;
}

void UHeroAbility_HitPause::OnHitPauseDelayFinished()
{
    //Debug::Print(TEXT("Enter"));
    UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

    if (World)
    {
        //Debug::Print(TEXT("ResetDelay"));
        UGameplayStatics::SetGlobalTimeDilation(World, OriginalWorldDilation);
    }
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
