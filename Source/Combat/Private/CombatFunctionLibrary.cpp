// Zhang All Rights Reserved.


#include "CombatFunctionLibrary.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnFightInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatGameplayTags.h"
#include "CombatTypes/CombatCountDownAction.h"
#include "CombatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/CombatSaveGame.h"

#include "CombatDebugHelper.h"


UCombatAbilitySystemComponent* UCombatFunctionLibrary::NativeGetCombatASCFromActor(AActor* InActor)
{
	checkf(InActor, TEXT("InActor is not valid."));

	/** Tries to find an ability system component on the actor, will use AbilitySystemInterface or fall back to a component search */
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = CastChecked<UCombatAbilitySystemComponent>(AbilitySystemComponent);

	return CombatAbilitySystemComponent;
}

void UCombatFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = NativeGetCombatASCFromActor(InActor);
	check(CombatAbilitySystemComponent);

	if (CombatAbilitySystemComponent->HasMatchingGameplayTag(TagToAdd))
	{
		return;
	}

	CombatAbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	// LocalASC will not be nullptr for in function UCombatFunctionLibrary::NativeGetCombatASCFromActor(AActor* InActor) there is CastChecked
	UCombatAbilitySystemComponent* LocalASC = NativeGetCombatASCFromActor(InActor);

	if (LocalASC->HasMatchingGameplayTag(TagToRemove))
	{
		LocalASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UCombatFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	// LocalASC will not be nullptr for in function UCombatFunctionLibrary::NativeGetCombatASCFromActor(AActor* InActor) there is CastChecked
	UCombatAbilitySystemComponent* LocalASC = NativeGetCombatASCFromActor(InActor);

	return LocalASC->HasMatchingGameplayTag(TagToCheck);
}

void UCombatFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, ECombatConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? ECombatConfirmType::Yes : ECombatConfirmType::No;
}

UPawnFightComponent* UCombatFunctionLibrary::NativeGetPawnFightComponentFromActor(AActor* InActor)
{
	checkf(InActor, TEXT("InActor is not valid."));

	IPawnFightInterface* PawnFightInterface = Cast<IPawnFightInterface>(InActor);

	/*if (PawnFightInterface)
	{
		PawnFightInterface->GetPawnFightComponent();
	}*/
	 
	return PawnFightInterface ? PawnFightInterface->GetPawnFightComponent() : nullptr;
}

UPawnFightComponent* UCombatFunctionLibrary::BP_GetPawnFightComponentFromActor(AActor* InActor, ECombatValidType& OutValidType)
{
	UPawnFightComponent* FightComponent = NativeGetPawnFightComponentFromActor(InActor);

	OutValidType = FightComponent ? ECombatValidType::Valid : ECombatValidType::Invalid;

	return FightComponent;
}

bool UCombatFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn);
	check(TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UCombatFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UCombatFunctionLibrary::ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, float& OutAngleDifference)
{
	check(InAttacker);
	check(InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();
	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);
	if (CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}

	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
	{
		return CombatGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference <= -135.f || OutAngleDifference >= 135.f)
	{
		return CombatGameplayTags::Shared_Status_HitReact_Back;
	}
	else if (OutAngleDifference > 45.f && OutAngleDifference < 135.f)
	{
		return CombatGameplayTags::Shared_Status_HitReact_Right;
	}
	else if (OutAngleDifference < -45.f && OutAngleDifference > -135.f)
	{
		return CombatGameplayTags::Shared_Status_HitReact_Left;
	}

	return CombatGameplayTags::Shared_Status_HitReact_Front;
}

bool UCombatFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker);
	check(InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	/*const FString DebugString = FString::Printf(TEXT("DotResult: %f %s"), DotResult, DotResult <= 0.2f ? TEXT("Valid Block") : TEXT("Not Valid Block"));

	Debug::Print(DebugString, DotResult <= 0.2f ? FColor::Green : FColor::Red);*/

	return DotResult <= 0.2f;
}

bool UCombatFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UCombatAbilitySystemComponent* SourceASC = NativeGetCombatASCFromActor(InInstigator);
	UCombatAbilitySystemComponent* TargetASC = NativeGetCombatASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UCombatFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ECombatCountDownActionInput CountDownInput, ECombatCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	
	if (!World)
	{
		return;
	}

	FLatentActionManager& LatentActionManager =  World->GetLatentActionManager();

	FCombatCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FCombatCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	if (CountDownInput == ECombatCountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FCombatCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
			);
		}
	}

	if (CountDownInput == ECombatCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

UCombatGameInstance* UCombatFunctionLibrary::GetCombatGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UCombatGameInstance>();
		}
	}

	return nullptr;
}

void UCombatFunctionLibrary::ToggleInputMode(ECombatGameInputMode InInputMode, const UObject* WorldContextObject)
{
	APlayerController* PlayerController = nullptr;

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;

	switch (InInputMode)
	{
	case ECombatGameInputMode::GameOnly:
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
		break;
	case ECombatGameInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;
		break;
	default:
		break;
	}
}

void UCombatFunctionLibrary::SaveCurrentGameDifficulty(ECombatGameDifficulty InDifficultyToSave)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UCombatSaveGame::StaticClass());

	if (UCombatSaveGame* CombatSaveGameObject = Cast<UCombatSaveGame>(SaveGameObject))
	{
		CombatSaveGameObject->SavedCurrentGameDifficuly = InDifficultyToSave;

		const bool bWasSaved = UGameplayStatics::SaveGameToSlot(CombatSaveGameObject, CombatGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		Debug::Print(bWasSaved ? TEXT("Difficulty Saved!") : TEXT("Difficulty NOT Saved!"));
	}
}

bool UCombatFunctionLibrary::TryLoadSavedGameDifficulty(ECombatGameDifficulty& OutSavedDifficulty)
{
	if (UGameplayStatics::DoesSaveGameExist(CombatGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(CombatGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		if (UCombatSaveGame* CombatSaveGameObject = Cast<UCombatSaveGame>(SaveGameObject))
		{
			OutSavedDifficulty = CombatSaveGameObject->SavedCurrentGameDifficuly;

			Debug::Print(TEXT("Loading Successful!"));

			return true;
		}
	}

	//OutSavedDifficulty = ECombatGameDifficulty::Easy;

	return false;
}
