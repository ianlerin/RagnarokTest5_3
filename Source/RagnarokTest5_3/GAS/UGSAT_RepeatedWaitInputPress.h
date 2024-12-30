// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UGSAT_RepeatedWaitInputPress.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROKTEST5_3_API UGSAT_RepeatedWaitInputPress : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FInputPressDelegate	OnPress;

	UFUNCTION()
	void OnPressCallback();

	virtual void Activate() override;

	/** Wait until the user presses the input button for this ability's activation. Returns time this node spent waiting for the press. Will return 0 if input was already down. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGSAT_RepeatedWaitInputPress* RepeatedWaitInputPress(UGameplayAbility* OwningAbility, bool bTestAlreadyPressed = false);

public:

	float StartTime = 0.0;

	bool bTestInitialState = false;

	FDelegateHandle DelegateHandle;
	
};
