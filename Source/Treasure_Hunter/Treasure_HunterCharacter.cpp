// Copyright Epic Games, Inc. All Rights Reserved.

#include "Treasure_HunterCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Pickup.h"
#include "TreasurePickup.h"

//////////////////////////////////////////////////////////////////////////
// ATreasure_HunterCharacter

ATreasure_HunterCharacter::ATreasure_HunterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//create a fps camera
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetCapsuleComponent()); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FPSCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCamera->bUsePawnControlRotation = true;// Allow the pawn to control camera rotation.

	//Create The collection Sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Set Base Health for the character
	InitialHealth = 100.f;
	CharacterHealth = InitialHealth;

	//Set Base whealth level for the character
	InitialWealth = 0.f;
	CharacterWealth = InitialWealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATreasure_HunterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &ATreasure_HunterCharacter::CollectPickups);
	PlayerInputComponent->BindAction("ToggleCamera", IE_Pressed, this, &ATreasure_HunterCharacter::ToggleCamera);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATreasure_HunterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATreasure_HunterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATreasure_HunterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATreasure_HunterCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATreasure_HunterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATreasure_HunterCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATreasure_HunterCharacter::OnResetVR);
}

void ATreasure_HunterCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATreasure_HunterCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATreasure_HunterCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATreasure_HunterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATreasure_HunterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATreasure_HunterCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATreasure_HunterCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATreasure_HunterCharacter::CollectPickups()
{
	//get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	//keep track of the collected treasure Wealth
	float CollectedWealth = 0;

	//for each actor we collect 
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		//cast the actor to APickup
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
		//if the cast is succesful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			//call the pickup's WasCollected Function
			TestPickup->WasCollected();
			//check to see if the pickup is a Treasure
			ATreasurePickup* const TestTreasure = Cast<ATreasurePickup>(TestPickup);
			if (TestTreasure)
			{
				//increase the collected wealth value
				CollectedWealth += TestTreasure->GetTreasureValue();
			}
			//deactivate the Pickup to avoid collection spamming
			TestPickup->SetActive(false);
		}
	}
	if (CollectedWealth > 0)
	{
		UpdateWealth(CollectedWealth);
	}
}

//reports starting health
float ATreasure_HunterCharacter::GetInitialHealth()
{
	return InitialHealth;
}
//reports current health
float ATreasure_HunterCharacter::GetCurrentHealth()
{
	return CharacterHealth;
}

/**
	updates the Character health when the charcater recieves damage
	The damage comes from the Enemiy Actor or the enviromet/traps
	This will be implemented later
*/
float ATreasure_HunterCharacter::UpdateHealth(float HealthChange)
{
	return CharacterHealth = CharacterHealth + HealthChange;
}

//reports starting wealth
float ATreasure_HunterCharacter::GetInitialWealth()
{
	return InitialWealth;
}
//reports current wealth
float ATreasure_HunterCharacter::GetCurrentWealth()
{
	return CharacterWealth;
}

//updates character whealth when a pickups is collected
float ATreasure_HunterCharacter::UpdateWealth(float WealthChange)
{
	return CharacterWealth = CharacterWealth + WealthChange;
}

//toggles between first persona nd third person camera
void ATreasure_HunterCharacter::ToggleCamera()
{
	//check if the follow camera is active and the fpscamera is inactive
	if (FollowCamera->IsActive() && !FPSCamera->IsActive())
	{
		FollowCamera->Deactivate();
		bUseControllerRotationYaw = true;
		FPSCamera->Activate();
	}
	else
	{
		FollowCamera->Activate();
		bUseControllerRotationYaw = false;
		FPSCamera->Deactivate();
	}
}

/*
void ATreasure_HunterCharacter::ToggleCharacterBase()
{
	//Check if theCurrentWealth is bigger than 0
	CollectionSphere->IsOverlappingActor();
	if (CharacterWealth > 0)
	{
		//if it'sbigger than 0
		FString CharacterBaseDebugString = GetName();
		UE_LOG(LogClass, Log, TEXT("You arrived at your Camp (%s)"), *FString(CharacterBaseDebugString));
	}
	else if(CharacterWealth <= 0){
		FString CharacterBaseDebugString = GetName();
		UE_LOG(LogClass, Log, TEXT(" You need at leas 1 gold to retire !!!(%s)"), *FString(CharacterBaseDebugString));
	}
}*/