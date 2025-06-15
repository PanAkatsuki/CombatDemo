// Zhang All Rights Reserved.


#include "Items/PickUpItems/CombatStoneBase.h"

#include "Characters/CombatHeroCharacter.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

#include "CombatDebugHelper.h"


ACombatStoneBase::ACombatStoneBase()
{
}

void ACombatStoneBase::BeginPlay()
{
	if (!ConsumeFX.IsNull())
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			ConsumeFX.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ACombatStoneBase::OnAsyncLoadConsumeFXFinished)
		);
	}
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

	OnStoneConsumed();

	Destroy();
	//Debug::Print(TEXT("Destroy"));
}

void ACombatStoneBase::OnPickUpZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACombatHeroCharacter* OverlappedHeroCharacter = Cast<ACombatHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(CombatGameplayTags::Player_Ability_PickUp_Stones.GetTag().GetSingleTagContainer());
	}
}

void ACombatStoneBase::OnPickUpZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACombatStoneBase::OnStoneConsumed()
{
	if (ConsumeFX.IsValid())
	{
		check(GetWorld());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ConsumeFX.Get(),
			GetActorLocation()
		);
	}

	if (ConsumeSound)
	{
		check(GetWorld());
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ConsumeSound,
			GetActorLocation()
		);
	}
}

void ACombatStoneBase::OnAsyncLoadConsumeFXFinished()
{
	Debug::Print(TEXT("Loaded!"));
}