#include "Pickup.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//all pickups start active
	bIsActive = true;

	//create the static mesh component
	//changed CreateAbstractDefaultSubobject by CreateDefaultObject 
	//by recommendation of error list.
	//check how this works
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
}



// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

}

//returns pickupstate
bool APickup::IsActive()
{
	return bIsActive;
}

//changes active state
void APickup::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}

void APickup::WasCollected_Implementation()
{
	//log a debug message
	FString PickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected %s"), *FString(PickupDebugString));
}