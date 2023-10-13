// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "PickUpHealth.generated.h"
class USoundBase;

UCLASS()
class DRONE_API APickUpHealth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpHealth();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	UAudioComponent* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		UCapsuleComponent* BaseCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		UStaticMeshComponent* PickupMesh;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
