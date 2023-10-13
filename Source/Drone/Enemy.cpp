// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.h"
#include "DronePawn.h"
#include "TimerManager.h"
#include "Perception/AISenseConfig_Sight.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// Sets default values
AEnemy::AEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);
	DamageCollision->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.2f));
	DamageCollision->SetCollisionProfileName("BlockAll"); 
	

	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	CubeMeshComponent->SetStaticMesh(CubeMesh);
	CubeMeshComponent->SetupAttachment(DamageCollision);

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sense Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1300.0f;
		SightConfig->LoseSightRadius = 1301.0f;
		SightConfig->PeripheralVisionAngleDegrees = 36.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->SetMaxAge(0.1f);
	}

	
	UE_LOG(LogTemp, Warning, TEXT("Sight Radius: %f"), SightConfig->SightRadius);
	UE_LOG(LogTemp, Warning, TEXT("Lose Sight Radius: %f"), SightConfig->LoseSightRadius);
	UE_LOG(LogTemp, Warning, TEXT("Peripheral Vision Angle: %f"), SightConfig->PeripheralVisionAngleDegrees);
	UE_LOG(LogTemp, Warning, TEXT("Detect Enemies: %d"), SightConfig->DetectionByAffiliation.bDetectEnemies);
	UE_LOG(LogTemp, Warning, TEXT("Detect Friendlies: %d"), SightConfig->DetectionByAffiliation.bDetectFriendlies);
	UE_LOG(LogTemp, Warning, TEXT("Detect Neutrals: %d"), SightConfig->DetectionByAffiliation.bDetectNeutrals);

	

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	
	
	
}

void AEnemy::DealDamage(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("In DealDamage"));
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is dead"));
		Destroy();
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);
	CubeMeshComponent->SetMaterial(0, DefaultCubeMaterial);
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemy::OnHit"));
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile)
	{
		this->DealDamage(50.0f);
		Projectile->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("AEnemy::OnHit in if"));
	}
	

}

void AEnemy::OnSensed(const TArray<AActor*>& DetectedPawn)
{
	for (int i = 0; i < DetectedPawn.Num(); i++)
	{
		//Information about sensed actor
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(DetectedPawn[i], Info);

		if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			CubeMeshComponent->SetMaterial(0, CubeMatDetected);
			FVector ShootDir = DetectedPawn[i]->GetActorLocation() - GetActorLocation();
			FTimerHandle TimerHandle_SpawnActor;
			FireDelegate.BindLambda([this, ShootDir]()
				{
					Fire(ShootDir);
				});

			GetWorldTimerManager().SetTimer(TimerHandle_SpawnActor, FireDelegate, 0.5f, false, 0.5f);

		}
		else if(!Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			CubeMeshComponent->SetMaterial(0, DefaultCubeMaterial);
		}
	}
}

void AEnemy::Fire(FVector Direction)
{
	if (ProjectileClass)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
			// Spawn the projectile at the enemy's location with the desired direction
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z+100) , FRotator::ZeroRotator, ActorSpawnParams);
			
			if (Projectile)
			{
				// Get the projectile's movement component
				UProjectileMovementComponent* ProjectileMovement = Projectile->FindComponentByClass<UProjectileMovementComponent>();
				if (ProjectileMovement)
				{
					// Set the initial velocity and direction of the projectile
					ProjectileMovement->Velocity = Direction * 10.0f;
				}
			}
		}
	}
}





