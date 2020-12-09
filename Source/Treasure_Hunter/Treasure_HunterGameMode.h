// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Treasure_HunterGameMode.generated.h"

//Enum to store current state of gameplay
UENUM(BlueprintType)
enum class ETreasure_HunterPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ATreasure_HunterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATreasure_HunterGameMode();

	virtual void Tick(float DeltaTime) override;

	/**Returns Wealth needed to win - Required for the HUD
	UFUNCTION(BlueprintPure, Category = "Wealth")
	float GetWealthToWin() const;*/

	/* here and not in the constructor because or gamemode may
	not be constructed after our Character is constructed
	but we nedd to base the WealthTo Win on the character's
	InitialWealth*/
	virtual void BeginPlay() override;

	//current Whealth needed to win
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wealth")
	float WealthToWin;

	//accesor function for Wealth to win
	UFUNCTION(BlueprintPure, Category = "Wealth")
	float GetWealthToWin() const;

	/** Returns the current playstate */
	UFUNCTION(BlueprintPure, Category = "Wealth")
	ETreasure_HunterPlayState GetCurrentState() const;

	// sets a new playstate
	void SetCurrentState(ETreasure_HunterPlayState NewState);

private:
	/** Keeps track of the current game state*/
	ETreasure_HunterPlayState CurrentState;

	void HandleNewState(ETreasure_HunterPlayState NewState);
};


