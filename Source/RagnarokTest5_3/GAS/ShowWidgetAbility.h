// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "ShowWidgetAbility.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROKTEST5_3_API UShowWidgetAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
protected:

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnInputRelease(float TimeHeld);

	void ShowWidget();

	void HideWidget();

	void ApplyHealthReducingEffect();

	void IncrementTimeHeld();
	void CreateInteractWidget();
protected:
	FActiveGameplayEffectHandle AppliedHealthReductionEffect;
	FTimerHandle HeldTimer;
	UPROPERTY(EditDefaultsOnly)
	float HeldTimerInterval = 0.1;
	UPROPERTY(EditDefaultsOnly)
	float HealthToReduce = 10;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect>ReduceHealthClass;
	UPROPERTY()
	class UInteractiveWidget* Widget = nullptr;
	UPROPERTY()
	class UAbilityTask_WaitInputRelease* WaitInputReleaseTask = nullptr;
};
