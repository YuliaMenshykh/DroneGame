// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"



UCLASS()
class DRONE_API AEnemy : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* CubeMeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMesh* CubeMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Detected")
		UMaterialInterface* DefaultCubeMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Detected")
		UMaterialInterface* CubeMatDetected;

	bool IsDetected;
	bool BackToDef;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
		class UAIPerceptionComponent* AIPerComp;
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
		class UAISenseConfig_Sight* SightConfig;
	UFUNCTION()
		void OnSensed(const TArray<AActor*>& DetectedPawn);

	UFUNCTION()
		void Fire(FVector Direction);

	FTimerDelegate FireDelegate;
	

	float SpawnFrequency = 1.0f;


	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		FRotator EnemyRotation;
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		FVector BaseLocation;
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health = 100.0f;
	UPROPERTY(EditAnywhere)
		float DamageValue = 5.0f;

	void DealDamage(float DamageAmount);

	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;


protected:
	virtual void BeginPlay() override;

	
};
