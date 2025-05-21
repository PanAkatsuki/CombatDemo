// Zhang All Rights Reserved.


#include "AnimInstances/Hero/CombatHeroAnimInstance.h"

#include "Characters/CombatHeroCharacter.h"
#include "Components/Fight/PawnFightComponent.h"

#include "CombatDebugHelper.h"


void UCombatHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CastOwningBaseCharacterToHeroCharacter();
}

void UCombatHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	UpdatebShouldEnterRelaxState(DeltaSeconds);
}

void UCombatHeroAnimInstance::CastOwningBaseCharacterToHeroCharacter()
{
	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<ACombatHeroCharacter>(OwningCharacter);
	}
}

void UCombatHeroAnimInstance::UpdatebShouldEnterRelaxState(const float& DeltaSeconds)
{
	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}

