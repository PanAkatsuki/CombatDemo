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

}

void ACombatEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void ACombatEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

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

UPawnFightComponent* ACombatEnemyCharacter::GetPawnFightComponent() const
{
	return EnemyFightComponent;
}

UPawnUIComponent* ACombatEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}
