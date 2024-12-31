// Copyright Epic Games, Inc. All Rights Reserved.

#include "RagnarokTest5_3Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/BaseGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "RagnarokTest5_3/Widget/InteractiveWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Controller.h"
#include "RagnarokTest5_3/GAS/MyAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"


#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARagnarokTest5_3Character

ARagnarokTest5_3Character::ARagnarokTest5_3Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	AttributeSetBase = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSetBase"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ARagnarokTest5_3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	CreateInteractWidget();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARagnarokTest5_3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARagnarokTest5_3Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARagnarokTest5_3Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	BindASCInput();
}

void ARagnarokTest5_3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARagnarokTest5_3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARagnarokTest5_3Character::InitializeAttributes()
{
	if (!IsValid(GetAbilitySystemComponent()))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}
}


void ARagnarokTest5_3Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetAbilitySystemComponent())
	{
		APlayerState* PS = GetPlayerState<APlayerState>();
		if (PS)
		{
			GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		}
		InitializeAttributes();
	}
}


UAbilitySystemComponent* ARagnarokTest5_3Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ARagnarokTest5_3Character::AddCharacterAbilities()
{
	for (auto& StartupAbilityPtr : CharacterSoftAbilities)
	{
	
		TSubclassOf< UBaseGameplayAbility> StartupAbility = StartupAbilityPtr.LoadSynchronous();
		if (StartupAbility)
		{
			int32 ID = static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID);
			auto Handle = GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, ID, this));
		}
	}
}

void ARagnarokTest5_3Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APlayerState* PS = GetPlayerState<APlayerState>();
	if (PS&& GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}


	if (NewController)
	{
		AddCharacterAbilities();
	}
}

void ARagnarokTest5_3Character::CreateInteractWidget()
{
	APlayerController* PC=Cast<APlayerController>(GetController());
	MyInteractWidget = Cast<UInteractiveWidget>(UWidgetBlueprintLibrary::Create(this, InteractWidgetClass, PC));
	if (MyInteractWidget)
	{
		MyInteractWidget->AddToViewport();
		MyInteractWidget->Hide();
	}
}


UInteractiveWidget* ARagnarokTest5_3Character::GetMyInteractWidget()
{
	return MyInteractWidget;
}

void ARagnarokTest5_3Character::BindASCInput()
{
	if (InputComponent && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("/Script/RAGNAROKTEST5_3.EGSAbilityInputID"), static_cast<int32>(EGSAbilityInputID::Confirm), static_cast<int32>(EGSAbilityInputID::Cancel)));
	}
}

float ARagnarokTest5_3Character::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}