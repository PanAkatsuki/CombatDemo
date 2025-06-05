// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CombatTypes/CombatEnumTypes.h"

#include "CombatFunctionLibrary.generated.h"

class UCombatAbilitySystemComponent;
class UPawnFightComponent;
struct FScalableFloat;
struct FGameplayEffectSpecHandle;
class UCombatGameInstance;

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UCombatAbilitySystemComponent* NativeGetCombatASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);


	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, ECombatConfirmType& OutConfirmType);


	static UPawnFightComponent* NativeGetPawnFightComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary", meta = (DisplayName = "Get Pawn Fight Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnFightComponent* BP_GetPawnFightComponentFromActor(AActor* InActor, ECombatValidType& OutValidType);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Combat|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "Combat|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "Combat|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ECombatCountDownActionInput CountDownInput, ECombatCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "Combat|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UCombatGameInstance* GetCombatGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(ECombatGameInputMode InInputMode, const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static void SaveCurrentGameDifficulty(ECombatGameDifficulty InDifficultyToSave);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static bool TryLoadSavedGameDifficulty(ECombatGameDifficulty& OutSavedDifficulty);

};
