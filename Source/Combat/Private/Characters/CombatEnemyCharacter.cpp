// Zhang All Rights Reserved.


#include "Characters/CombatEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Fight/EnemyFightComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CombatWidgetBase.h"
#include "Components/BoxComponent.h"
#include "CombatFunctionLibrary.h"
#include "GameModes/CombatBaseGameMode.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Weapons/CombatWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/TimelineComponent.h"

#include "CombatDebugHelper.h"


ACombatEnemyCharacter::ACombatEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Set CharacterMovementComponent
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyFightComponent = CreateDefaultSubobject<UEnemyFightComponent>(TEXT("EnemyFightComponent"));

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(this->GetMesh());

	// Init Timeline
	DissolveTimelineAttributeSet.DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimeline"));

	// Set LatentInfo
	DestroyEnemyAttributeSet.DestroyEnemyCharacterLatentInfo.CallbackTarget = this;
	DestroyEnemyAttributeSet.DestroyEnemyCharacterLatentInfo.ExecutionFunction = FName("OnSpawnStoneEnd");
	DestroyEnemyAttributeSet.DestroyEnemyCharacterLatentInfo.Linkage = 0;
	DestroyEnemyAttributeSet.DestroyEnemyCharacterLatentInfo.UUID = 2;
}

void ACombatEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void ACombatEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set Timeline with DissolveCurve
	if (DissolveTimelineAttributeSet.DissolveTimeline && DissolveTimelineAttributeSet.DissolveCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("OnDissolveTimelineUpdate"));
		DissolveTimelineAttributeSet.DissolveTimeline->AddInterpFloat(DissolveTimelineAttributeSet.DissolveCurve, TimelineProgress);

		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnDissolveTimelineFinished"));
		DissolveTimelineAttributeSet.DissolveTimeline->SetTimelineFinishedFunc(TimelineFinished);
	}

	// Init Widget
	if (UCombatWidgetBase* HealthWidget = Cast<UCombatWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreateWidget(this);
	}
}

#if WITH_EDITOR
void ACombatEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			LeftHandCollisionBoxAttachBoneName
		);
	}

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			RightHandCollisionBoxAttachBoneName
		);
	}
}
#endif

void ACombatEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UCombatFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			EnemyFightComponent->OnHitTargetActor(OtherActor);
		}
	}
}

UPawnFightComponent* ACombatEnemyCharacter::GetPawnFightComponent() const
{
	return EnemyFightComponent;
}

void ACombatEnemyCharacter::OnEnemyDied(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem)
{
	GetMesh()->bPauseAnims = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyUIComponent->RemoveEnemyDrawnWidgetsIfAny();
	//Debug::Print(TEXT("EnemyDied!"));
	AsyncLoadNiagaraSystem(InDissolveNiagaraSystem);
}

UPawnUIComponent* ACombatEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

void ACombatEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	int32 AbilityApplyLevel = 1;

	if (ACombatBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<ACombatBaseGameMode>())
	{
		switch (BaseGameMode->GetCurrentGameDifficulty())
		{
		case ECombatGameDifficulty::Easy:
			AbilityApplyLevel = 1;
			break;

		case ECombatGameDifficulty::Normal:
			AbilityApplyLevel = 2;
			break;

		case ECombatGameDifficulty::Hard:
			AbilityApplyLevel = 3;
			break;

		case ECombatGameDifficulty::Insane:
			AbilityApplyLevel = 4;
			break;

		default:
			break;

		}
	}

	// Async Load
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveDataToAbilitySystemComponent(CombatAbilitySystemComponent, AbilityApplyLevel);
				}
			}
		)
	);
}

void ACombatEnemyCharacter::AsyncLoadNiagaraSystem(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem)
{
	if (InDissolveNiagaraSystem.IsNull())
	{
		Debug::Print(TEXT("In AsyncLoadNiagaraSystem, InDissolveNiagaraSystem is null"));
		return;
	}

	DissolveNiagaraSystem = InDissolveNiagaraSystem;

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		InDissolveNiagaraSystem.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ACombatEnemyCharacter::OnAsyncLoadNiagaraSystemFinished)
	);
}

void ACombatEnemyCharacter::OnAsyncLoadNiagaraSystemFinished()
{
	if (UNiagaraSystem* LoadedSystem = DissolveNiagaraSystem.Get())
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			LoadedSystem,
			GetMesh(),
			FName(),
			FVector(),
			FRotator(),
			EAttachLocation::KeepRelativeOffset,
			true
		);

		UMaterialInstanceDynamic* MaterialInstanceDynamic = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0));
		FHashedMaterialParameterInfo HashedMaterialParameterInfo = FHashedMaterialParameterInfo(FName("DissolveEdgeColor"));
		FLinearColor LinearColor;
		MaterialInstanceDynamic->GetVectorParameterValue(HashedMaterialParameterInfo, LinearColor);

		NiagaraComponent->SetVariableLinearColor(FName("DissolveParticleColor"), LinearColor);

		// Start Timeline
		DissolveTimelineAttributeSet.DissolveTimeline->SetPlayRate(1 / DissolveTimelineAttributeSet.DissolveTime);
		DissolveTimelineAttributeSet.DissolveTimeline->PlayFromStart();
	}
	else
	{
		Debug::Print(TEXT("In OnAsyncLoadNiagaraSystemFinished, NiagaraSystem load failed!"));
	}
}

void ACombatEnemyCharacter::OnDissolveTimelineUpdate(float InValue)
{
	//Debug::Print(TEXT("OnDissolveTimelineUpdate"));
	SetScalarParameterValueOnMaterial(InValue);
}

void ACombatEnemyCharacter::OnDissolveTimelineFinished()
{
	//Debug::Print(TEXT("OnDissolveTimelineFinished"));
	DestroyEnemyCharacter();
}

void ACombatEnemyCharacter::SetScalarParameterValueOnMaterial(float InParameterValue)
{
	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

	SkeletalMeshComponent->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), InParameterValue);

	if (ACombatWeaponBase* Weapon = GetEnemyFightComponent()->GetCharacterCurrentEquippedWeapon())
	{
		Weapon->GetWeaponMesh()->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), InParameterValue);
	}
}

void ACombatEnemyCharacter::DestroyEnemyCharacter()
{
	if (ACombatWeaponBase* EnemyWeapon = GetEnemyFightComponent()->GetCharacterCurrentEquippedWeapon())
	{
		EnemyWeapon->Destroy();
	}

	GetCombatAbilitySystemComponent()->TryAcitivateAbilityByTag(CombatGameplayTags::Enemy_Ability_SpawnStone);

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

	check(World);

	UKismetSystemLibrary::Delay(World, DestroyEnemyAttributeSet.DestroyEnemyCharacterDelayDuration, DestroyEnemyAttributeSet.DestroyEnemyCharacterLatentInfo);
}

void ACombatEnemyCharacter::OnSpawnStoneEnd()
{
	this->Destroy();
}