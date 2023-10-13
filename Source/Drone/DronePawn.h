// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "DronePawn.generated.h"


class UInputComponent;
class USceneComponent;
class USoundBase;


UCLASS()
class DRONE_API ADronePawn : public APawn
{
	GENERATED_BODY()

	class AEnemy* Enemy;

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
public:
	ADronePawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;

protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the thrust axis */
	void ThrustInput(float Val);

	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);

	/** Bound to the horizontal axis */
	void LookUpInput(float Val);

	UFUNCTION()
		void Turn(float Value);

	virtual void BeginPlay() override;

private:

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
		float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
		float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Yaw, EditAnywhere)
		float MinSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	FVector2D MovementInput;
	FVector2D CameraInput;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	float CurrentRightSpeed;

	float CurrentHightSpeed;

	UAudioComponent* FireSound;


public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AProjectile> Projectile;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Fire();
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* MuzzleLocation;

	class UWorld* World;

	FRotator SpawnRotation;
	FVector SpawnLocation;
	
	void RestoreHealth();

	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurrentHealth = 0.0f;

	UPROPERTY(EditAnywhere)
		float DamageValue = 5.0f;

	void DealDamage(float DamageAmount);

	UPROPERTY(EditAnywhere)
		int AmountBullets= 0;

	int MaxBullets = 10;

	void RestoreBullets();
};
