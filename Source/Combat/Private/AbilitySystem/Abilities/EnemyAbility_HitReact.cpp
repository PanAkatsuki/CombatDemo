// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_HitReact.h"

#include "Kismet/KismetMathLibrary.h"
#include "Characters/CombatEnemyCharacter.h"


bool UEnemyAbility_HitReact::DoesAbilityHaveMontagesToPlay()
{
	return MontagesMap.Num() > 0 ? true : false;
}

UAnimMontage* UEnemyAbility_HitReact::FindMontageToPlay()
{
	int32 RandomInt = FMath::RandRange(1, MontagesMap.Num());

	UAnimMontage* const* MontagePtr = MontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UEnemyAbility_HitReact::MakeEnemyFaceToAttacker(const FGameplayEventData& EventData)
{
	if (!bFaceToAttacker)
	{
		return;
	}

	ACombatEnemyCharacter* Enemy = GetEnemyCharacterFromActorInfo();

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), EventData.Instigator->GetActorLocation());

	Enemy->SetActorRotation(NewRotation);
}
