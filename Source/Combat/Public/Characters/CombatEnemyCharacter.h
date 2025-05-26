// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CombatBaseCharacter.h"
#include "CombatEnemyCharacter.generated.h"

//DECLARE_DELEGATE_OneParam(FOnAsyncLoadFinishedDelegate, TSoftObjectPtr<UNiagaraSystem>&);

class UEnemyFightComponent;
class UEnemyUIComponent;
class UWidgetComponent;
class UBoxComponent;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatEnemyCharacter : public ACombatBaseCharacter, public IPawnFightInterface
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
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface
#endif
	
public:
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

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyFightComponent* GetEnemyFightComponent() const { return EnemyFightComponent; }
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }

	//~ Begin IPawnFightInterface Interface.
	virtual UPawnFightComponent* GetPawnFightComponent() const override;
	//~ End IPawnFightInterface Interface

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	//// Set Niagara System on enemy's material when enemy dead
	//UFUNCTION(BlueprintCallable)
	//void AsyncLoadDeathAsset(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem);

	//FOnAsyncLoadFinishedDelegate OnAsyncLoadFinishedDelegate;

	//void OnAsyncLoadFinished(TSoftObjectPtr<UNiagaraSystem>& InDissolveNiagaraSystem);

	// Set death effect on enemy's material
	UFUNCTION(BlueprintCallable)
	void SetScalarParameterValueOnMaterial(float InParameterValue);

	//~ For Destroy Character
	// Set DestroyEnemyCharacterDelayDuration to make sure stone spawned before destroy Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLatentActionInfo DestroyEnemyCharacterLatentInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DestroyEnemyCharacterDelayDuration = 0.5f;

	UFUNCTION(BlueprintCallable)
	void DestroyEnemyCharacter();

	UFUNCTION()
	void OnSpawnStoneEnd();
	//~ For Destroy Character

};
