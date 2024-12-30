// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGSAbilityInputID : uint8
{

	None		        UMETA(DisplayName = "None"),
	Activate			UMETA(DisplayName = "Activate"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
};

UCLASS()
class RAGNAROKTEST5_3_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGSAbilityInputID AbilityInputID = EGSAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGSAbilityInputID AbilityID = EGSAbilityInputID::None;
};
