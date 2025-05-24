// Zhang All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include "AbilitySystem/CombatAttributeSet.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


struct FCombatDamageCapture
{
	/* Slow way of doing capture */

	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UCombatAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UCombatAttributeSet, AttackPower)
	//);
	//
	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDifinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//);
	//
	//RelevantAttributesToCapture.Add(AttackPowerCaptureDifinition);
	// 
 


	// Do the same thing as 
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UCombatAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UCombatAttributeSet, AttackPower)
	//);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);


	FCombatDamageCapture()
	{
		// Do the same thing as 
		//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDifinition(
		//	AttackPowerProperty,
		//	EGameplayEffectAttributeCaptureSource::Source,
		//	false
		//);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DamageTaken, Target, false);
	}
};

static const FCombatDamageCapture& GetCombatDamageCapture()
{
	static FCombatDamageCapture CombatDamageCapture;
	return CombatDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	/* Get Parameters */
	float SourceAttackPower = 0.f;
	bool bSuccessfulCalculatedSourceAttackPower = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCombatDamageCapture().AttackPowerDef, 
		EvaluateParameters, 
		SourceAttackPower
	);
	if (!bSuccessfulCalculatedSourceAttackPower)
	{
		check(0);
	}
	Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float TargetDefensePower = 0.f;
	bool bSuccessfulCalculatedTargetDefensePower = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCombatDamageCapture().DefensePowerDef, 
		EvaluateParameters, 
		TargetDefensePower
	);
	if (!bSuccessfulCalculatedTargetDefensePower)
	{
		check(0);
	}
	Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);
	
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavtAttackComboCount = 0;
	int32 UsedCounterAttackComboCount = 0;
	int32 UsedSpecialWeaponAttackComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
			Debug::Print(TEXT("UsedLightAttackComboCount"), UsedLightAttackComboCount);
		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavtAttackComboCount = TagMagnitude.Value;
			Debug::Print(TEXT("UsedHeavtAttackComboCount"), UsedHeavtAttackComboCount);
		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_CounterAttack))
		{
			UsedCounterAttackComboCount = TagMagnitude.Value;
			Debug::Print(TEXT("UsedCounterAttackComboCount"), UsedCounterAttackComboCount);
		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_SpecialWeaponAbility))
		{
			UsedSpecialWeaponAttackComboCount = TagMagnitude.Value;
			Debug::Print(TEXT("UsedSpecialWeaponAttackComboCount"), UsedSpecialWeaponAttackComboCount);
		}
	}

	/* Calculate Zone */
	// Shoule keep base DamageScaling = 1.f, for monster's attack calculate will not enter any if function below
	float DamageScaling = 1.f;

	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncrease = (UsedLightAttackComboCount - 1) * LightAttackComboDamageIncrease + LightAttackDamageScaling;
		DamageScaling = DamageIncrease;
		Debug::Print(TEXT("DamageIncreaseLight"), DamageIncrease);
	}

	if (UsedHeavtAttackComboCount != 0)
	{
		const float DamageIncrease = (UsedHeavtAttackComboCount - 1) * HeavyAttackComboDamageIncrease + HeavyAttackDamageScaling;
		DamageScaling = DamageIncrease;
		Debug::Print(TEXT("DamageIncreaseHeavy"), DamageIncrease);
	}

	if (UsedCounterAttackComboCount != 0)
	{
		const float DamageIncrease = (UsedCounterAttackComboCount - 1) * CounterAttackComboDamageIncrease + CounterAttackDamageScaling;
		DamageScaling = DamageIncrease;
		Debug::Print(TEXT("CounterAttackDamageScaling"), DamageScaling);
	}

	if (UsedSpecialWeaponAttackComboCount != 0)
	{
		const float DamageIncrease = UsedSpecialWeaponAttackComboCount * SpecialAttackComboDamageIncrease + SpecialAttackDamageScaling;
		DamageScaling = DamageIncrease;
		Debug::Print(TEXT("DamageIncreaseSpecialWeaponAttack"), DamageIncrease);
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower * DamageScaling / TargetDefensePower;
	Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCombatDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
