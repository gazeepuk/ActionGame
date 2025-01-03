// replcae later


#include "AbilitySystem/ExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "CoreTypes/AGGameplayTags.h"

struct FAGDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);

	FAGDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, DamageTaken, Target, false);
	}
};

static const FAGDamageCapture& GetAGDamageCapture()
{
	static FAGDamageCapture DamageCapture;
	return DamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetAGDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetAGDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetAGDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                     FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAGDamageCapture().AttackPowerDef, EvaluateParameters,
	                                                           SourceAttackPower);
	
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAGDamageCapture().DefensePowerDef, EvaluateParameters,
	                                                           TargetDefensePower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboAttack = 0.f;
	int32 UsedHeavyAttackComboAttack = 0.f;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(AGGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(AGGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboAttack = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(AGGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboAttack = TagMagnitude.Value;
		}
	}

	if (UsedLightAttackComboAttack != 0.f)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboAttack - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	}

	if (UsedHeavyAttackComboAttack != 0.f)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboAttack * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	
	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetAGDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
