// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UInteractiveWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (MyButton)
	{
		MyButton->OnPressed.AddDynamic(this, &UInteractiveWidget::OnButtonPressed);
	}
	UpdateLabelText();
	UpdateButtonText();
	SetupAttributeListener();
}


void UInteractiveWidget::SetupAttributeListener()
{
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Char)
	{
		//setup listener for future changes
		AActor* Actor = Cast<AActor>(Char);
		UAbilitySystemComponent* Component = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
		Component->GetGameplayAttributeValueChangeDelegate(HealthAttribute).AddUObject(this, &UInteractiveWidget::OnHealthChanged);
		bool bFound = false;
		//setup initial max health and percent
		MaxHealth = Component->GetGameplayAttributeValue(MaxHealthAttribute, bFound);
		float Percent = Component->GetGameplayAttributeValue(HealthAttribute, bFound) / MaxHealth;
		ProgressBar->SetPercent(Percent);
	}

}

void UInteractiveWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float Percent=Data.NewValue / MaxHealth;
	ProgressBar->SetPercent(Percent);
}

void UInteractiveWidget::UpdateButtonText()
{
	if (ButtonTextBlock)
	{
		FString MyText = ButtonText;
		MyText += " ";
		MyText += FString::Printf(TEXT("%.2f"), TimerHeld);
		ButtonTextBlock->SetText(FText::FromString(MyText));
	}
}

void UInteractiveWidget::OnButtonPressed()
{
	bToggle = !bToggle;
	UpdateLabelText();
}

void UInteractiveWidget::UpdateLabelText()
{
	if (LabelTextBlock)
	{
		if (bToggle)
		{
			LabelTextBlock->SetText(FText::FromString("ButtonToggle On"));
		}
		else
		{
			LabelTextBlock->SetText(FText::FromString("ButtonToggle Off"));
		}
	}
}

void UInteractiveWidget::IncrementTimeHeld(float Held)
{
	TimerHeld += Held;
	UpdateButtonText();
}

void UInteractiveWidget::ResetTimeHeld()
{
	TimerHeld = 0;
}

void UInteractiveWidget::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UInteractiveWidget::Show()
{
	SetVisibility(ESlateVisibility::Visible);
}