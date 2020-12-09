// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Treasure_HunterCharacter.generated.h"

UCLASS(config=Game)
class ATreasure_HunterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/* First Person Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FPSCamera;

	//Collection Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collection Sphere", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;

public:
	ATreasure_HunterCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpRate;

	//accesor function for initial healt
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetInitialHealth();

	//accesor function for current health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth();

	//accesor function for initial wealth
	UFUNCTION(BlueprintPure, Category = "Wealth")
	float GetInitialWealth();

	//accesor function for current wealth
	UFUNCTION(BlueprintPure, Category = "Wealth")
	float GetCurrentWealth();

	/**
		Function to pdate the character's health
		@param HEalthChange is the amount to change the Health by,
		and it can only be negative at the moment ( there is no health pickups yet)
	*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float UpdateHealth(float HealthChange);

	/**function to update the character's wealth
		@param WealthChange This is the amount to change the wealth by,
		and it can be possitive or negative.
	**/
	UFUNCTION(BlueprintCallable, Category = "Wealth")
	float UpdateWealth(float WealthChange);

	//called when we press a key to toggle between first and third camera
	UFUNCTION(BlueprintCallable, Category = "Camera Mode")
	void ToggleCamera();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	//Called when we press a key to collect any pickups inside the 
	//collection Sphere
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void CollectPickups();

	//The Starting Health of the Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float InitialHealth;

	//The starting Wealth on the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wealth")
		float InitialWealth;

public:

	//current character health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CharacterHealth;

	//current character wealth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wealth")
		float CharacterWealth;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns FPSCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFPSCamera() const { return FPSCamera; }
	//Returns Collection Sphere Sub oBject
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
};

