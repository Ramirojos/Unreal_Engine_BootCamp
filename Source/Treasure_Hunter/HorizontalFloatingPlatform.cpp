// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizontalFloatingPlatform.h"

// Sets default values
AHorizontalFloatingPlatform::AHorizontalFloatingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));

	if (CylinderVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CylinderVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AHorizontalFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHorizontalFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*will make our Cylinder float up and down.*/
	FVector NewLocation = GetActorLocation();

	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));

	//Scale our height by FloatSpeed
	NewLocation.X += DeltaHeight * FloatSpeed;
	SetActorLocation(NewLocation);
}

