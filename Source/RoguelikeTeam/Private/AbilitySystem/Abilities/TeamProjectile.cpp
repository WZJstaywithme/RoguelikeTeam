// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TeamProjectile.h"

// Sets default values
ATeamProjectile::ATeamProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeamProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeamProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

