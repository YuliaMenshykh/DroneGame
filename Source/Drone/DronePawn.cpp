// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"


ADronePawn::ADronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"))

		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);
	DamageCollision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	DamageCollision->SetCollisionProfileName("BlockAll");

	


	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle location"));
	MuzzleLocation->SetupAttachment(DamageCollision);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 60.0f; 
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; 

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
	CurrentRightSpeed = 500.0f;
	CurrentHightSpeed = 500.0f;

	//Set fire sound
	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundWave(TEXT("/Game/StarterContent/Audio/ShootingSound"));
	if (SoundWave.Succeeded())
	{
		FireSound->SetSound(SoundWave.Object);
	}

	//Health
	CurrentHealth = MaxHealth;
	AmountBullets = MaxBullets;
	World = GetWorld();
}

void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ADronePawn::OnHit);
}


void ADronePawn::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, CurrentRightSpeed * DeltaSeconds, CurrentHightSpeed * DeltaSeconds);
	AddActorLocalOffset(LocalMove, true);

	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	{
		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -60.0f, 0.0f);
		SpringArm->SetWorldRotation(NewRotation);
	}

	{
		if (!MovementInput.IsZero())
		{
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaSeconds;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaSeconds;
			SetActorLocation(NewLocation);
		}
	}

	Super::Tick(DeltaSeconds);
}

void ADronePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Thrust", this, &ADronePawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADronePawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADronePawn::MoveRightInput);
	PlayerInputComponent->BindAxis("Turns", this, &ADronePawn::Turn);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ADronePawn::Fire);
	PlayerInputComponent->BindAxis("LookUp", this, &ADronePawn::LookUpInput);
}


void ADronePawn::ThrustInput(float Val)
{
	CurrentForwardSpeed = Val != 0.0f ? (Val * Acceleration) : (0.0f);
}

void ADronePawn::MoveUpInput(float Val)
{
	CurrentHightSpeed = Val != 0.0f ? (Val * Acceleration) : (0.0f);
	
}

void ADronePawn::MoveRightInput(float Val)
{
	CurrentRightSpeed = Val != 0.0f ? (Val * Acceleration) : (0.0f);

}

void ADronePawn::LookUpInput(float Val)
{
	CameraInput.Y = Val;
}


void ADronePawn::Turn(float Val)
{
	CameraInput.X = Val;
}

void ADronePawn::Fire()
{
	if (World != NULL)
	{
		if (AmountBullets > 0)
		{
			SpawnRotation = Camera->GetComponentRotation();
			SpawnLocation = MuzzleLocation->GetComponentLocation();
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			World->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (FireSound != nullptr)
			{
				FireSound->Play();
			}
			AmountBullets -= 1;
			//UE_LOG(LogTemp, Warning, TEXT("AmountBullets: %f"), AmountBullets);
		}
		
	}

	
}
void ADronePawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ADronePawn:: OnHit"));
	AProjectile* TempProj = Cast<AProjectile>(OtherActor);
	if (TempProj)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADronePawn:: OnHit in if"));
		this->DealDamage(50.0f);
		TempProj->Destroy();
	}

}
void ADronePawn::DealDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0.0f)
	{
		Destroy();
	}
}

void ADronePawn::RestoreBullets()
{
	AmountBullets = 10;
	UE_LOG(LogTemp, Warning, TEXT("AmountBullets: %f"), AmountBullets);
}

void ADronePawn::RestoreHealth()
{
	CurrentHealth = 100.0f;
}


