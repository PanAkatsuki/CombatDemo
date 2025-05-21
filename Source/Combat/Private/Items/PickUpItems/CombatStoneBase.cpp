// Zhang All Rights Reserved.


#include "Items/PickUpItems/CombatStoneBase.h"

#include "Characters/CombatHeroCharacter.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"


void ACombatStoneBase::OnPickUpZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACombatHeroCharacter* OverlappedHeroCharacter = Cast<ACombatHeroCharacter>(OtherActor))
	{
		//OverlappedHeroCharacter->GetCombatAbilitySystemComponent()->TryAcitivateAbilityByTag(CombatGameplayTags::Player_Ability_PickUp_Stones);
		OverlappedHeroCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(CombatGameplayTags::Player_Ability_PickUp_Stones.GetTag().GetSingleTagContainer());
	}
}

void ACombatStoneBase::OnPickUpZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACombatStoneBase::Consume(UCombatAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);

	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnStoneConsumed();

	Destroy();
}
