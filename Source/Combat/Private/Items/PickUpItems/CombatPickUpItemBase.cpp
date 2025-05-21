// Zhang All Rights Reserved.


#include "Items/PickUpItems/CombatPickUpItemBase.h"

#include "Components/SphereComponent.h"


// Sets default values
ACombatPickUpItemBase::ACombatPickUpItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickUpZone = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpZone"));
	SetRootComponent(PickUpZone);
	PickUpZone->InitSphereRadius(50.f);
	PickUpZone->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpZoneBeginOverlap);
	PickUpZone->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpZoneEndOverlap);
}

void ACombatPickUpItemBase::OnPickUpZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACombatPickUpItemBase::OnPickUpZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

