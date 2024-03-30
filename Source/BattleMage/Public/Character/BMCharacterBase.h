// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BMCharacterBase.generated.h"

UCLASS()
class BATTLEMAGE_API ABMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ABMCharacterBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly)
	FName RightHandSocket;

	bool bIsDead = false;
	
	

};
