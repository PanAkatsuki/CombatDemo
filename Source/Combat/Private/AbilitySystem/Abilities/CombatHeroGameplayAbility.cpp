// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"

#include "Characters/CombatHeroCharacter.h"
#include "Controllers/CombatHeroController.h"
#include "Components/Fight/HeroFightComponent.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "CombatFunctionLibrary.h"
#include "Components/UI/HeroUIComponent.h"


ACombatHeroCharacter* UCombatHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedCombatHeroCharacter.IsValid())
    {
        CachedCombatHeroCharacter = Cast<ACombatHeroCharacter>(CurrentActorInfo->AvatarActor);
    }
    
    return CachedCombatHeroCharacter.IsValid() ? CachedCombatHeroCharacter.Get() : nullptr;
}

ACombatHeroController* UCombatHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (!CachedCombatHeroController.IsValid())
    {
        CachedCombatHeroController = Cast<ACombatHeroController>(CurrentActorInfo->PlayerController);
    }

    return CachedCombatHeroController.IsValid() ? CachedCombatHeroController.Get() : nullptr;
}

UHeroFightComponent* UCombatHeroGameplayAbility::GetHeroFightComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroFightComponent();
}

UHeroUIComponent* UCombatHeroGameplayAbility::GetHeroUIComponentFromActorInfo()
{
    return CastChecked<UHeroUIComponent>(GetHeroCharacterFromActorInfo()->GetPawnUIComponent());
}

FGameplayEffectSpecHandle UCombatHeroGameplayAbility::MakeHeroDamageEffectsSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
    checkf(EffectClass, TEXT("EffectClass is not valid."));

    FGameplayEffectContextHandle EffectContextHandle = GetCombatAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    EffectContextHandle.SetAbility(this);
    EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    EffectContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle DamageEffectsSpecHandle = GetCombatAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass, 
        GetAbilityLevel(),
        EffectContextHandle
    );
    
    // Make Map for calculation
    DamageEffectsSpecHandle.Data->SetSetByCallerMagnitude(
        CombatGameplayTags::Shared_SetByCaller_BaseDamage,
        InWeaponBaseDamage
    );

    if (InCurrentAttackTypeTag.IsValid())
    {
        DamageEffectsSpecHandle.Data->SetSetByCallerMagnitude(
            InCurrentAttackTypeTag, 
            InUsedComboCount
        );
    }

    return DamageEffectsSpecHandle;
}

bool UCombatHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
    check(InCooldownTag.IsValid());

    FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

    TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

    if (!TimeRemainingAndDuration.IsEmpty())
    {
        RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
        TotalCooldownTime = TimeRemainingAndDuration[0].Value;
    }
    
    return RemainingCooldownTime > 0.f;
}

