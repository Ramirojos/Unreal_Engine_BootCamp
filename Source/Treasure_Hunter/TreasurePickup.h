// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "TreasurePickup.generated.h"

/**
 * 
 */
UCLASS()
class TREASURE_HUNTER_API ATreasurePickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATreasurePickup();

	//Override WasColleted function - use implementation because is a 
	//blueprint Native Event
	void WasCollected_Implementation() override;

	//Public way to acces the Treasure "monetary" value
	float GetTreasureValue();

protected:

	//Sets the ammount of gold the treasure gives to the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold Value", Meta = (BlueprintProtected = "true"))
		float TreasureValue;
};
