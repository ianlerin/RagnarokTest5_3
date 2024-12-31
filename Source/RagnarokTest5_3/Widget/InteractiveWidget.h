// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

/**
 * 
 */
UCLASS()
class RAGNAROKTEST5_3_API UInteractiveWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnButtonClicked OnButtonClickedEvent;
protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* MyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ButtonTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LabelTextBlock;

	float MaxHealth = 10;

	bool bToggle = false;

	float TimerHeld = 0;

	UPROPERTY(EditDefaultsOnly)
	FString ButtonText = "MyButton";
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute HealthAttribute;
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute MaxHealthAttribute;
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonPressed();
	void UpdateLabelText();
	void UpdateButtonText();
	void SetupAttributeListener();
	void OnHealthChanged(const FOnAttributeChangeData& Data);

public:
	void IncrementTimeHeld(float Held);
	void ResetTimeHeld();

	void Hide();

	void Show();
};
