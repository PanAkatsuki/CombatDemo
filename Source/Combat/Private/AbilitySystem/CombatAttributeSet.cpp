// Zhang All Rights Reserved.


#include "AbilitySystem/CombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "Characters/CombatHeroCharacter.h"

#include "CombatDebugHelper.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	InitMaxHealth(1.f);
	InitCurrentHealth(1.f);
	InitMaxRage(1.f);
	InitCurrentRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement PawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("Could't extract a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel())

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		// If Data.Target is not a hero character, return
		if (!Cast<ACombatHeroCharacter>(Data.Target.GetAvatarActor()))
		{
			return;
		}

		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UCombatFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), CombatGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() == 0.f)
		{
			UCombatFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), CombatGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), CombatGameplayTags::Player_Status_Rage_Full);
			UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), CombatGameplayTags::Player_Status_Rage_None);
		}

		// Check if PawnUIComponent can cast to UHeroUIComponent, only when PawnUIComponent from hero character can success
		if (UHeroUIComponent* HeroUIComponent = Cast<UHeroUIComponent>(PawnUIComponent))
		{
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		/*const FString DebugString = FString::Printf(
			TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);

		Debug::Print(DebugString);*/

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (NewCurrentHealth == 0.f)
		{
			UCombatFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), CombatGameplayTags::Shared_Status_Death);
		}
	}
}
