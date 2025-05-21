// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/CombatWidgetBase.h"
#include "Controllers/CombatHeroController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "CombatDebugHelper.h"


void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();

		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
	);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	if (InAvailableActors.IsEmpty())
	{
		return nullptr;
	}

	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentLockedActorNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor)
		{
			continue;
		}

		const FVector PlayerToAvailableActorsNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentLockedActorNormalized, PlayerToAvailableActorsNormalized);

		if (CrossResult.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!TargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Should assign a valid widget class in blueprint"));

		// Create
		TargetLockWidget = CreateWidget<UCombatWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(TargetLockWidget);

		TargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	 if (!TargetLockWidget || !CurrentLockedActor)
	 {
		 CancelTargetLockAbility();
		 return;
	 }

	 FVector2D ScreenPosition;
	 UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		 GetHeroControllerFromActorInfo(),
		 CurrentLockedActor->GetActorLocation(),
		 ScreenPosition,
		 true
	 );

	 if (TargetLockWidgetSize == FVector2D::ZeroVector)
	 {
		 TargetLockWidget->WidgetTree->ForEachWidget(
			 [this](UWidget* FoundWidget)
			 {
				 if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				 {
					 TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					 TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				 }
			 }
		 );
	 }

	 ScreenPosition -= (TargetLockWidgetSize / 2.f);

	 TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;

	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	checkf(TargetLockMappingContext, TEXT("Forget to assign Local Mapping Context in Blueprint"));

	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);

	CleanUp();
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (TargetLockWidget)
	{
		TargetLockWidget->RemoveFromParent();
	}

	TargetLockWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;
}

void UHeroGameplayAbility_TargetLock::ClearAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
		CachedDefaultMaxWalkSpeed = 0.f;
	}
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo())
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CleanUp();
	ClearAvailableActorsToLock();
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor || 
		UCombatFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, CombatGameplayTags::Shared_Status_Death) ||
		UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Shared_Status_Death)
		)
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	if (
		!UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rolling) &&
		!UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Blocking)
		)
	{
		check(CurrentLockedActor);
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetHeroControllerFromActorInfo()->SetControlRotation(TargetRot);
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}

}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == CombatGameplayTags::Player_Event_SwithchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);

	}
	else if (InSwitchDirectionTag == CombatGameplayTags::Player_Event_SwithchTarget_Right)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);

	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;

	}
}
