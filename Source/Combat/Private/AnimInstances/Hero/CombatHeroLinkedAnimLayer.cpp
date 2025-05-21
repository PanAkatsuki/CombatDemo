// Zhang All Rights Reserved.


#include "AnimInstances/Hero/CombatHeroLinkedAnimLayer.h"

#include "AnimInstances/Hero/CombatHeroAnimInstance.h"

UCombatHeroAnimInstance* UCombatHeroLinkedAnimLayer::GetCombatHeroAnimInstance()
{
    if (!CachedHeroAnimInstance.IsValid())
    {
        CachedHeroAnimInstance = Cast<UCombatHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
    }

    return CachedHeroAnimInstance.IsValid() ? CachedHeroAnimInstance.Get() : nullptr;
}
