// Zhang All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include "AbilitySystem/CombatAttributeSet.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


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


struct FCombatDamageCapture
{
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);
	
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
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);

		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedLightAttackComboCount"), UsedLightAttackComboCount);

		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavtAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedHeavtAttackComboCount"), UsedHeavtAttackComboCount);

		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_CounterAttack))
		{
			UsedCounterAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedCounterAttackComboCount"), UsedCounterAttackComboCount);

		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_SpecialWeaponAbility))
		{
			UsedSpecialWeaponAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedSpecialWeaponAttackComboCount"), UsedSpecialWeaponAttackComboCount);
		}
	}

	/* Calculate Zone */
	// Shoule keep base DamageIncrease = 1.f
	float DamageIncrease = 1.f;

	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreaseLight = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;
		DamageIncrease = DamageIncreaseLight;
		//Debug::Print(TEXT("DamageIncreaseLight"), DamageIncreaseLight);

	}

	if (UsedHeavtAttackComboCount != 0)
	{
		const float DamageIncreaseHeavy = (UsedHeavtAttackComboCount - 1) * 0.3f + 1.f;
		DamageIncrease = DamageIncreaseHeavy;
		//Debug::Print(TEXT("DamageIncreaseHeavy"), DamageIncreaseHeavy);

	}

	if (UsedCounterAttackComboCount != 0)
	{
		const float DamageIncreaseCounterAttack = UsedCounterAttackComboCount * 3.0f + 1.f;
		DamageIncrease = DamageIncreaseCounterAttack;
		//Debug::Print(TEXT("DamageIncreaseCounterAttack"), DamageIncreaseCounterAttack);

	}

	if (UsedSpecialWeaponAttackComboCount != 0)
	{
		const float DamageIncreaseSpecialWeaponAttack = UsedSpecialWeaponAttackComboCount * 2.0f + 1.f;
		DamageIncrease = DamageIncreaseSpecialWeaponAttack;
		//Debug::Print(TEXT("DamageIncreaseSpecialWeaponAttack"), DamageIncreaseSpecialWeaponAttack);

	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower * DamageIncrease / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

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
