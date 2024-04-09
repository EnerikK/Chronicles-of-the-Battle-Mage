// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BMCharacterMovementComponent.generated.h"

class ABMCharacter;

UENUM()
enum EMovementModeBattleMage
{
	BMove_None    UMETA(DisplayName = "None"),
	BMove_Slide   UMETA(DisplayName = "Slide"),
	
	BMove_MAX     UMETA(DisplayName = "MAX"),

};

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBmCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_BattleMage : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		/*Flag*/
		uint8 Saved_bWantsToSprint : 1;
		
		uint8 Saved_bWantsToSlide : 1;

	public:
		
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
	
	class FNetworkPredictionData_Client_BattleMage : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_BattleMage(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	/*Sprint*/
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Walk_MaxWalkSpeed;
	
	bool bWantsToSprint;
	bool bPrev_WantsToCrouch;
	bool bWantsToSlide;
	
	/*Slide*/
	UPROPERTY(EditDefaultsOnly)
	float Slide_MinSpeed = 350;
	UPROPERTY(EditDefaultsOnly)
	float Slide_EnterImpulse = 500;
	UPROPERTY(EditDefaultsOnly)
	float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly)
	float Slide_Friction = 1.3;

	UPROPERTY(Transient)
	ABMCharacter* PlayerCharacter;
	


public:

	UBmCharacterMovementComponent();
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	UFUNCTION()
	void SprintPressed();

	UFUNCTION()
	void SprintReleased();

	UFUNCTION()
	void CrouchPressed();

	UFUNCTION()
	void SlidePressed();

	UFUNCTION()
	bool IsCustomMovementMode(EMovementModeBattleMage InCustomMovementMode) const;

protected:

	virtual void InitializeComponent() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:

	/*Slide*/
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime,int32 It);
	bool GetSlideSurface(FHitResult& Hit)const;
	
};
