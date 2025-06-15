// Zhang All Rights Reserved.


#include "GameModes/CombatSurvialGameMode.h"

#include "Engine/AssetManager.h"
#include "Characters/CombatEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "CombatFunctionLibrary.h"

#include "CombatDebugHelper.h"


void ACombatSurvialGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	ECombatGameDifficulty SavedGameDifficulty;

	if (UCombatFunctionLibrary::TryLoadSavedGameDifficulty(SavedGameDifficulty))
	{
		CurrentGameDifficulty = SavedGameDifficulty;
	}
}

void ACombatSurvialGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnDataTable, TEXT("Forgot to assign a valid data table to survial game mode in blueprint!"));

	SetCurrentSurvialGameModeState(ECombatSurvialGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void ACombatSurvialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSurvialGameModeState == ECombatSurvialGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;

			SetCurrentSurvialGameModeState(ECombatSurvialGameModeState::SpawningNewWave);
		}
	}

	if (CurrentSurvialGameModeState == ECombatSurvialGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.f;

			SetCurrentSurvialGameModeState(ECombatSurvialGameModeState::InProgress);
		}
	}

	if (CurrentSurvialGameModeState == ECombatSurvialGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;

			CurrentWaveCount++;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvialGameModeState(ECombatSurvialGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvialGameModeState(ECombatSurvialGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void ACombatSurvialGameMode::RegisterSpawnedEnemies(const TArray<ACombatEnemyCharacter*>& InEnemiesToRegister)
{
	for (ACombatEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;

			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}

void ACombatSurvialGameMode::SetCurrentSurvialGameModeState(ECombatSurvialGameModeState InSurvialGameModeState)
{
	CurrentSurvialGameModeState = InSurvialGameModeState;

	OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}

bool ACombatSurvialGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void ACombatSurvialGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}

	PreLoadedEnemyClassMap.Empty();

	for (const FCombatEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				}
			)
		);
	}
}

FCombatEnemyWaveSpawnerTableRow* ACombatSurvialGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
	FCombatEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnDataTable->FindRow<FCombatEnemyWaveSpawnerTableRow>(RowName, FString());
	checkf(FoundRow, TEXT("Could not find a valid row under the	name %s in the data table!"), *RowName.ToString());

	return FoundRow;
}

int32 ACombatSurvialGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}

	checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level: %s for spawning enemies"), *GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FCombatEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector	RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f);

			ACombatEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}

			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
}

bool ACombatSurvialGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void ACombatSurvialGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;

	//Debug::Print(FString::Printf(TEXT("CurrentSpawnedEnemiesCounter: %i, TotalSpawnedEnemiesThisWaveCounter: %i"), CurrentSpawnedEnemiesCounter, TotalSpawnedEnemiesThisWaveCounter));

	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if (CurrentSpawnedEnemiesCounter == 0)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;

		SetCurrentSurvialGameModeState(ECombatSurvialGameModeState::WaveCompleted);
	}
}
