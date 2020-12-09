// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasurePickup.h"

//Set default values
ATreasurePickup::ATreasurePickup()
{
	GetMesh();

	//ther base monetary value of the Treasure
	TreasureValue = 150.f;
}

void ATreasurePickup::WasCollected_Implementation()
{
	//use the base pickup behavior
	Super::WasCollected_Implementation();
	//Destroy the Treasure
	Destroy();
}

//report the monetary value of the treasure
float ATreasurePickup::GetTreasureValue()
{
	return TreasureValue;
}