// Copyright Epic Games, Inc. All Rights Reserved.

#include "Treasure_HunterGameMode.h"
#include "Treasure_HunterCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ATreasure_HunterGameMode::ATreasure_HunterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PrimaryActorTick.bCanEverTick = true;
};


void ATreasure_HunterGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(ETreasure_HunterPlayState::EPlaying);

	ATreasure_HunterCharacter* MyCharacter = Cast<ATreasure_HunterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	WealthToWin = 2500.f;

}

void ATreasure_HunterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*Check if we are using the characcter*/
	ATreasure_HunterCharacter* MyCharacter = Cast<ATreasure_HunterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{

		UE_LOG(LogTemp, Warning, TEXT("Tick gets called"));
		if (MyCharacter->GetCurrentWealth() >= WealthToWin)
		{
			SetCurrentState(ETreasure_HunterPlayState::EWon);
		}
		if (MyCharacter->GetCurrentHealth() <= 0)
		{
			SetCurrentState(ETreasure_HunterPlayState::EGameOver);
		}
	}
}

float ATreasure_HunterGameMode::GetWealthToWin() const
{
	return WealthToWin;
}

ETreasure_HunterPlayState ATreasure_HunterGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ATreasure_HunterGameMode::SetCurrentState(ETreasure_HunterPlayState NewState)
{
	CurrentState = NewState;

	HandleNewState(CurrentState);
}


void ATreasure_HunterGameMode::HandleNewState(ETreasure_HunterPlayState NewState)
{
	switch (NewState)
	{
	case ETreasure_HunterPlayState::EGameOver:
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
	}
	break;
	case ETreasure_HunterPlayState::EUnknown:
	{
		//do nothing
	}
	break;
	}
}