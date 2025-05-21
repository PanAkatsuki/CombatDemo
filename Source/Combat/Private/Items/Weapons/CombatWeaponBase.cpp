// Zhang All Rights Reserved.


#include "Items/Weapons/CombatWeaponBase.h"

#include "Components/BoxComponent.h"
#include "CombatFunctionLibrary.h"

#include "CombatDebugHelper.h"

// Sets default values
ACombatWeaponBase::ACombatWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void ACombatWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forget to assign an instigator as the the owning pawn for the weapon: %s"), *GetName());

	APawn* HitPawn = Cast<APawn>(OtherActor);

	// If Weapon collide other weapon, cast will fail
	if (!HitPawn)
	{
		return;
	}

	check(HitPawn);

	if (UCombatFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
	{
		OnWeaponHitTarget.ExecuteIfBound(OtherActor);
	}
}

void ACombatWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forget to assign an instigator as the the owning pawn for the weapon: %s"), *GetName());

	APawn* HitPawn = Cast<APawn>(OtherActor);

	// If Weapon collide other weapon, cast will fail
	if (!HitPawn)
	{
		return;
	}

	check(HitPawn);

	if (UCombatFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
	{
		OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
	}
}

