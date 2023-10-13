// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(27.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapAll");
	RootComponent = CollisionComp;

	StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (StaticMesh)
	{
		UStaticMeshComponent* SphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
		SphereMeshComponent->SetupAttachment(CollisionComp);
		SphereMeshComponent->SetStaticMesh(StaticMesh);
		SphereMeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


