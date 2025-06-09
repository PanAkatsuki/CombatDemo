// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/BossAbility_DrawBossBar.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widgets/CombatWidgetBase.h"

#include "CombatDebugHelper.h"


UBossAbility_DrawBossBar::UBossAbility_DrawBossBar()
{
	AbilityActivationPolicy = ECombatAbilityActivationPolicy::OnGiven;
}

void UBossAbility_DrawBossBar::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*check(GetWorld());
	UUserWidget* Widget = UWidgetBlueprintLibrary::Create(
		GetWorld(),
		HealthBarWidgetClass,
		nullptr
	);
	HealthBarWidget = Cast<UCombatWidgetBase>(Widget);

	HealthBarWidget*/

	// This ability complete in blueprint
}

void UBossAbility_DrawBossBar::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
