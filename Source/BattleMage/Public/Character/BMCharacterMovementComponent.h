// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BMCharacterMovementComponent.generated.h"

class ABMCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlideStartDelegate);

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
	
	/*Slide*/
	UPROPERTY(EditDefaultsOnly)
	float Slide_MinSpeed = 350;
	UPROPERTY(EditDefaultsOnly)
	float Slide_EnterImpulse = 10;
	UPROPERTY(EditDefaultsOnly)
	float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly)
	float Slide_Friction = 1.3;
	UPROPERTY(EditDefaultsOnly)
	float Slide_CooldownDuration = 1.3;
	UPROPERTY(EditDefaultsOnly)
	float AuthSlideCooldownDuration= 3.9f;

	UPROPERTY(Transient)
	ABMCharacter* PlayerCharacter;

	bool bWantsToSprint;
	bool bPrev_WantsToCrouch;
	bool bWantsToSlide;

	FTimerHandle TimerHandle_SlideCooldown;

	float SlideStartTime;
	
	UPROPERTY(BlueprintAssignable)
	FSlideStartDelegate SlideStartDelegate;

public:

	UBmCharacterMovementComponent();
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	FORCEINLINE bool IsSliding() const {return IsCustomMovementMode(BMove_Slide);}
	
	UFUNCTION()
	void SprintPressed();

	UFUNCTION()
	void SprintReleased();

	UFUNCTION()
	void CrouchPressed();

	UFUNCTION()
	void SlidePressed();

	UFUNCTION()
	void SlideReleased();
	
	UFUNCTION()
	bool IsCustomMovementMode(EMovementModeBattleMage InCustomMovementMode) const;

protected:

	virtual void InitializeComponent() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	

private:

	void OnSlideCooldownFinished();
	bool CanSlide() const;
	void PerformSlide();

	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

	
};
