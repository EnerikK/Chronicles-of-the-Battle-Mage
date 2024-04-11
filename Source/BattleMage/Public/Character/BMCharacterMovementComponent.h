// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BMCharacterMovementComponent.generated.h"

class ABMCharacter;
class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlideStartDelegate);

UENUM()
enum EMovementModeBattleMage
{
	BMove_None      UMETA(DisplayName = "None"),
	BMove_Slide     UMETA(DisplayName = "Slide"),
	BMove_Jump		UMETA(DisplayName = "Jumping"),
	BMove_Walking   UMETA(DisplayName = "Walking"),
	BMove_Falling   UMETA(DisplayName = "Falling"),
	
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
		uint8 Saved_bPressedBattleMageJump : 1;

		uint8 Saved_bHadAnimRootMotion : 1;
		uint8 Saved_bTranstionFinished : 1;

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
	float Slide_EnterImpulse = 10;
	UPROPERTY(EditDefaultsOnly)
	float Slide_CooldownDuration = 1.3;
	UPROPERTY(EditDefaultsOnly)
	float AuthSlideCooldownDuration= 3.9f;

	/*Mantle*/
	UPROPERTY(EditDefaultsOnly)
	float MantleMaxDistance = 200;
	UPROPERTY(EditDefaultsOnly)
	float MantleReachHeight = 50;
	UPROPERTY(EditDefaultsOnly)
	float MinMantle = 30;
	UPROPERTY(EditDefaultsOnly)
	float MantleMinWallSteepnessAngle = 75;
	UPROPERTY(EditDefaultsOnly)
	float MantleMaxSurfaceAngle = 40;
	UPROPERTY(EditDefaultsOnly)
	float MantleMaxAlignmentAngle = 45;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TallMantleMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ProxyShortMantleMontage;
	
	bool bWantsToSprint;
	bool bPrev_WantsToCrouch;
	bool bWantsToSlide;
	bool bHadAnimRootMotion;
	bool bTransitionFinished;
	float SlideStartTime;
	

	/*Timers*/
	FTimerHandle TimerHandle_SlideCooldown;

	/*Delegates*/
	UPROPERTY(BlueprintAssignable)
	FSlideStartDelegate SlideStartDelegate;

	/*Replicated Variables*/
	UPROPERTY(ReplicatedUsing = OnRep_SlideStart)
	bool Proxy_bSlideStart;

	UPROPERTY(ReplicatedUsing = OnRep_ShortMantle)
	bool Proxy_bShortMantle;

	UPROPERTY(ReplicatedUsing = OnRep_TallMantle);
	bool Proxy_bTallMantle;

public:

	UBmCharacterMovementComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	FORCEINLINE bool IsSliding() const {return IsCustomMovementMode(BMove_Slide);}
	FORCEINLINE bool GetMantle() const {return TryMantle();}
	
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
	
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(EMovementModeBattleMage InCustomMovementMode) const;

protected:
	UPROPERTY(Transient)
	ABMCharacter* PlayerCharacter;

	virtual void InitializeComponent() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

private:

	/*Slide*/
	UFUNCTION()
	void OnRep_SlideStart();
	void OnSlideCooldownFinished();
	bool CanSlide() const;
	void PerformSlide();

	/*Mantle*/
	UFUNCTION()
	void OnRep_ShortMantle();
	UFUNCTION()
	void OnRep_TallMantle();
	bool TryMantle() const;
	FVector GetMantleStartLocation(FHitResult FrontHit , FHitResult SurfaceHit , bool bTallMantle);

	/*Helpers*/
	bool IsServer() const;
	float CapR() const;
	float CapHH() const;

	
};
