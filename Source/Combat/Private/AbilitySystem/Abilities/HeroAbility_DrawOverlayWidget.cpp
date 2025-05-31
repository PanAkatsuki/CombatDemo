// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_DrawOverlayWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Controllers/CombatHeroController.h"
#include "Blueprint/UserWidget.h"

#include "CombatDebugHelper.h"


UHeroAbility_DrawOverlayWidget::UHeroAbility_DrawOverlayWidget()
{
	AbilityActivationPolicy = ECombatAbilityActivationPolicy::OnGiven;
}

void UHeroAbility_DrawOverlayWidget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, GetHeroControllerFromActorInfo());
	UserWidget->AddToViewport();
	//Debug::Print(TEXT("DrawWidget!"));
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UHeroAbility_DrawOverlayWidget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
