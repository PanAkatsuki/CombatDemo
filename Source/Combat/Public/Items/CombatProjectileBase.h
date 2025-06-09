// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameplayEffectTypes.h"

#include "CombatProjectileBase.generated.h"


class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
struct FGameplayEventData;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EProtectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

USTRUCT(BlueprintType)
struct FProjectileSoundAndFXSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UNiagaraSystem> HitNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SpawnSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* FlyingSound;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UNiagaraSystem> MuzzleNiagaraSystem;
};

UCLASS()
class COMBAT_API ACombatProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<AActor*> OverlappedActors;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProtectileDamagePolicy ProtectileDamagePolicy = EProtectileDamagePolicy::OnHit;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

	UAudioComponent* FlyingSoundComponent;

	UPROPERTY(EditDefaultsOnly)
	FProjectileSoundAndFXSet SoundAndFXSet;

public:
	void SetProjectileDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InDamageEffectSpecHandle);

protected:
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayProjectileHitFX(const FHitResult& Hit);

	UFUNCTION()
	void OnAsyncLoadMuzzleNiagaraSystemFinished();

	UFUNCTION()
	void OnAsyncLoadHitNiagaraSystemFinished();

private:
	void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);
};
