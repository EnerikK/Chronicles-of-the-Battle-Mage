// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BMCharacterBase.h"

ABMCharacterBase::ABMCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
}
void ABMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


