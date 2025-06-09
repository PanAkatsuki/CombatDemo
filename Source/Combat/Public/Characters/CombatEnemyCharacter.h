// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CombatBaseCharacter.h"

#include "Interfaces/EnemyDeathInterface.h"

#include "CombatEnemyCharacter.generated.h"


class UEnemyFightComponent;
class UEnemyUIComponent;
class UWidgetComponent;
class UBoxComponent;
class UNiagaraSystem;
class UTimelineComponent;

USTRUCT(BlueprintType)
struct FEnterTimelineAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTimelineComponent* EnterTimeline;

	UPROPERTY(EditDefaultsOnly)
	float EnterTime = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* EnterCurve;
};

USTRUCT(BlueprintType)
struct FDissolveTimelineAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTimelineComponent* DissolveTimeline;

	UPROPERTY(EditDefaultsOnly)
	float DissolveTime = 4.0f;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* DissolveCurve;
};

USTRUCT(BlueprintType)
struct FDestroyEnemyAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FLatentActionInfo DestroyEnemyCharacterLatentInfo;

	UPROPERTY(EditDefaultsOnly)
	float DestroyEnemyCharacterDelayDuration = 0.5f;
};

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatEnemyCharacter : public ACombatBaseCharacter, public IEnemyDeathInterface, public IPawnFightInterface
{
	GENERATED_BODY()

public:
	ACombatEnemyCharacter();

protected:
	//~ Begin APawn Interface.
	// Load Start Up Data
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void BeginPlay() override;

#if WITH_EDITOR
	//~ Begin UObject Interface.
	// For Boss Enemy
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface
#endif
	
public:
	// For Boss Enemy
	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyFightComponent* EnemyFightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	FEnterTimelineAttributeSet EnterTimelineAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TMap<int32, UAnimMontage*> EnterMontagesMap;

	UPROPERTY(EditDefaultsOnly)
	FDissolveTimelineAttributeSet DissolveTimelineAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	FDestroyEnemyAttributeSet DestroyEnemyAttributeSet;

private:
	// Use this to get niagara system after async load
	TSoftObjectPtr<UNiagaraSystem> DissolveNiagaraSystem;

public:
	FORCEINLINE UEnemyFightComponent* GetEnemyFightComponent() const { return EnemyFightComponent; }
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }

	//~ Begin IPawnFightInterface Interface.
	virtual UPawnFightComponent* GetPawnFightComponent() const override;
	//~ End IPawnFightInterface Interface

	//~ Begin IEnemyDeathInterface Interface.
	virtual void OnEnemyDied(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem) override;
	//~ End IEnemyDeathInterface Interface
	// 
	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	void InitEnemyStartUpData();

	// Set Niagara System on enemy's material when enemy dead
	void AsyncLoadNiagaraSystem(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem);
	void OnAsyncLoadNiagaraSystemFinished();

	// Timeline
	UFUNCTION()
	void OnEnterTimelineUpdate(float InValue);

	UFUNCTION()
	void OnEnterTimelineFinished();

	UFUNCTION()
	void OnDissolveTimelineUpdate(float InValue);

	UFUNCTION()
	void OnDissolveTimelineFinished();

	// Set death effect on enemy's material
	void SetScalarParameterValueOnMaterial(float InParameterValue);

	// Set DestroyEnemyCharacterDelayDuration to make sure stone spawned before destroy Character
	void DestroyEnemyCharacter();

	UFUNCTION()
	void OnDelayForSpawnStoneEnd();

};
