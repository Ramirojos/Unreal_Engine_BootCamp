// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class TREASURE_HUNTER_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

	//Return the mesh for the pickup
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	//return whether or not the pickup is active
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	//Allows other classes to change safely wheter or not is active
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	//function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
		void WasCollected();
	virtual void WasCollected_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//true when pickup can be used, false when it's deactivated
	bool bIsActive;

private:
	//Static mesh to represent the pickup in the level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PickupMesh;
};
