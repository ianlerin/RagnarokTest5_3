// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowWidgetAbility.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "RagnarokTest5_3/Widget/InteractiveWidget.h"
#include "RagnarokTest5_3/RagnarokTest5_3Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UShowWidgetAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{

	Super::OnGiveAbility(ActorInfo, Spec);
	
}


void UShowWidgetAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// show widget
	if (!Widget)
	{
		ARagnarokTest5_3Character* Char = Cast<ARagnarokTest5_3Character>(ActorInfo->AvatarActor);
		if (Char)
		{
			Widget=Char->GetMyInteractWidget();
		}
	}
	WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UShowWidgetAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
	ShowWidget();

	//Bonus task 1
	FTimerDelegate HeldDelegate;
	HeldDelegate.BindUObject(this, &UShowWidgetAbility::IncrementTimeHeld);
	GetWorld()->GetTimerManager().SetTimer(HeldTimer, HeldDelegate, HeldTimerInterval,true);
	// Bonus task 2
	ApplyHealthReducingEffect();
}


void UShowWidgetAbility::OnInputRelease(float TimeHeld)
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UShowWidgetAbility::ApplyHealthReducingEffect()
{
    const auto Spec = MakeOutgoingGameplayEffectSpec(ReduceHealthClass);
    auto AppliedSpec=UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, FGameplayTag::RequestGameplayTag("Data.HealthReduction"), HealthToReduce);
	AppliedHealthReductionEffect=K2_ApplyGameplayEffectSpecToOwner(Spec);
}

void UShowWidgetAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->EndTask();
	}
	HideWidget();
	GetWorld()->GetTimerManager().ClearTimer(HeldTimer);
	if (Widget)
	{
		Widget->ResetTimeHeld();
	}
	BP_RemoveGameplayEffectFromOwnerWithHandle(AppliedHealthReductionEffect, 1);
}

void UShowWidgetAbility::ShowWidget()
{
	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
	FInputModeGameAndUI GameAndUIMode;
	GetActorInfo().PlayerController->SetInputMode(GameAndUIMode);
	GetActorInfo().PlayerController->bShowMouseCursor = true;
}

void UShowWidgetAbility::HideWidget()
{
	if (Widget)
	{
		Widget->Hide();
	}
	FInputModeGameOnly GameMode;
	GetActorInfo().PlayerController->SetInputMode(GameMode);
	GetActorInfo().PlayerController->bShowMouseCursor = false;
}

void UShowWidgetAbility::IncrementTimeHeld()
{
	if (Widget)
	{
		Widget->IncrementTimeHeld(HeldTimerInterval);
	}
}