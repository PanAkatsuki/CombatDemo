// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Aim.h"

#include "Controllers/CombatHeroController.h"
#include "Characters/CombatHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Widgets/CombatWidgetBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "CombatFunctionLibrary.h"
#include "Characters/CombatEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "CombatDebugHelper.h"


UHeroAbility_Aim::UHeroAbility_Aim()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Debug::Print(TEXT("Aim"));
	ACombatHeroController* HeroController = GetHeroControllerFromActorInfo();
	ACombatHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();

	HeroCharacter->bUseControllerRotationYaw = true;

	if (HeroController)
	{
		HeroCharacter->GetFollowCamera()->Deactivate();
		HeroCharacter->GetAimCamera()->Activate(true);
	}

	CreateCrosshairAndSetPosition();

	SetPlayMontageTask(this, FName("AimMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));

	// Set Timer
	GetWorld()->GetTimerManager().SetTimer(
		EnemyDetectTimerHandle,
		this,
		&UHeroAbility_Aim::DetectEnemyInCrosshair,
		0.1f * UGameplayStatics::GetGlobalTimeDilation(GetWorld()),
		true
	);
}

void UHeroAbility_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACombatHeroController* HeroController = GetHeroControllerFromActorInfo();
	ACombatHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();

	HeroCharacter->bUseControllerRotationYaw = false;

	if (HeroController)
	{
		HeroCharacter->GetAimCamera()->Deactivate();
		HeroCharacter->GetFollowCamera()->Activate(true);
	}

	if (CrosshairWidget)
	{
		CrosshairWidget->RemoveFromParent();
		CrosshairWidget = nullptr;
	}

	CrosshairWidgetSize = FVector2D::ZeroVector;

	// Clear Timer
	GetWorld()->GetTimerManager().ClearTimer(EnemyDetectTimerHandle);

	ClearTracedEnemy();
	ClearTargetInWidget();
}

void UHeroAbility_Aim::CreateCrosshairAndSetPosition()
{
	CrosshairWidget = CreateWidget<UCombatWidgetBase>(GetHeroControllerFromActorInfo(), CrosshairWidgetClass);
	check(CrosshairWidget);

	CrosshairWidget->SetPositionInViewport(GetScreenCenterPosition());
	CrosshairWidget->AddToViewport();
}

void UHeroAbility_Aim::DetectEnemyInCrosshair()
{
	ACombatHeroCharacter* Hero = GetHeroCharacterFromActorInfo();
	APlayerController* HeroController = GetHeroControllerFromActorInfo();
	if (!Hero || !HeroController)
	{
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	HeroController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 600.f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Hero);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, Params);

	if (bHit && HitResult.GetActor())
	{
		if (ACombatEnemyCharacter* EnemyCharacter = Cast<ACombatEnemyCharacter>(HitResult.GetActor()))
		{
			CachedTraceEnemyCharacter = EnemyCharacter;
			Hero->TargetInEnemyCharacter = EnemyCharacter;
			if (UCombatFunctionLibrary::IsTargetPawnHostile(GetHeroCharacterFromActorInfo(), EnemyCharacter))
			{
				//Debug::Print(TEXT("EnemyTraced"));
				if (TargetInWidget.IsEmpty())
				{
					for (size_t i = 0; i < 3; i++)
					{
						TargetInWidget.AddUnique(CreateWidget<UCombatWidgetBase>(GetHeroControllerFromActorInfo(), CrosshairWidgetClass));
					}
					

					FWidgetTransform FlipTransform0;
					FlipTransform0.Scale = FVector2D(-1.0f, -1.0f);
					AddTargetInWidgetToViewport(TargetInWidget[0], FlipTransform0, FVector2D(0.f, 20.f));

					FWidgetTransform FlipTransform1;
					FlipTransform1.Angle = 90.f;
					AddTargetInWidgetToViewport(TargetInWidget[1], FlipTransform1, FVector2D(10.f, 10.f));

					FWidgetTransform FlipTransform2;
					FlipTransform2.Angle = -90.f;
					AddTargetInWidgetToViewport(TargetInWidget[2], FlipTransform2, FVector2D(-10.f, 10.f));

					EnemyCharacter->GetMesh()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.0f);
				}

				return;
			}
		}
		else
		{
			ClearTracedEnemy();
			GetHeroCharacterFromActorInfo()->TargetInEnemyCharacter.Reset();
			ClearTargetInWidget();
		}
	}
	else
	{
		ClearTracedEnemy();
		GetHeroCharacterFromActorInfo()->TargetInEnemyCharacter.Reset();
		ClearTargetInWidget();
	}
}

FVector2D UHeroAbility_Aim::GetScreenCenterPosition()
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D ScreenCenter = ViewportSize * 0.5f;

	if (CrosshairWidgetSize == FVector2D::ZeroVector)
	{
		CrosshairWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					CrosshairWidgetSize.X = FoundSizeBox->GetWidthOverride();
					CrosshairWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	ScreenCenter.X -= CrosshairWidgetSize.X / 2;
	ScreenCenter.Y -= CrosshairWidgetSize.Y / 2;

	return ScreenCenter;
}

void UHeroAbility_Aim::AddTargetInWidgetToViewport(UCombatWidgetBase* InWidget, const FWidgetTransform& InFlipTransform, const FVector2D& InOffset)
{
	InWidget->SetRenderTransform(InFlipTransform);

	InWidget->SetPositionInViewport(GetScreenCenterPosition() += InOffset);

	InWidget->AddToViewport();
}

void UHeroAbility_Aim::ClearTracedEnemy()
{
	if (CachedTraceEnemyCharacter.IsValid())
	{
		CachedTraceEnemyCharacter->GetMesh()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0.0f);
		CachedTraceEnemyCharacter.Reset();
		return;
	}
}

void UHeroAbility_Aim::ClearTargetInWidget()
{
	if (TargetInWidget.Num())
	{
		for (UCombatWidgetBase* Widget : TargetInWidget)
		{
			Widget->RemoveFromParent();
		}
		
		TargetInWidget.Empty();
	}
}

void UHeroAbility_Aim::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Aim::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Aim::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_Aim::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}