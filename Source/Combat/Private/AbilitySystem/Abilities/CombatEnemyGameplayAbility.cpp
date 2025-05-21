// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"

#include "Characters/CombatEnemyCharacter.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"


ACombatEnemyCharacter* UCombatEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedEnemyCharacter.IsValid())
    {
        CachedEnemyCharacter = Cast<ACombatEnemyCharacter>(CurrentActorInfo->AvatarActor.Get());
    }

    return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyFightComponent* UCombatEnemyGameplayAbility::GetEnemyFightComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyFightComponent();
}

FGameplayEffectSpecHandle UCombatEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    check(EffectClass);

    FGameplayEffectContextHandle EffectContextHandle = GetCombatAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    EffectContextHandle.SetAbility(this);
    EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    EffectContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle DamageEffectsSpecHandle = GetCombatAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        EffectContextHandle
    );

    DamageEffectsSpecHandle.Data->SetSetByCallerMagnitude(
        CombatGameplayTags::Shared_SetByCaller_BaseDamage,
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );

    return DamageEffectsSpecHandle;
}
