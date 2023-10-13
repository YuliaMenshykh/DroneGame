// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpHealth.h"
#include "DronePawn.h"

// Sets default values
APickUpHealth::APickUpHealth()
{
	BaseCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BaseCapsuleComponent"));
	RootComponent = BaseCollisionComponent;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(false);
	//PickupMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	PickupMesh->SetupAttachment(RootComponent);

	PickupSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundWave(TEXT("/Game/StarterContent/Audio/PickupSound.PickupSound"));
	if (SoundWave.Succeeded())
	{
		PickupSound->SetSound(SoundWave.Object);
	}

}

void APickUpHealth::BeginPlay()
{
	Super::BeginPlay();
	BaseCollisionComponent->SetGenerateOverlapEvents(true);
	BaseCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUpHealth::OnOverlapBegin);
	PickupMesh->SetGenerateOverlapEvents(false);
	

}

void APickUpHealth::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADronePawn* Drone = Cast<ADronePawn>(OtherActor);
	if (Drone != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APickUpHealth::OnOverlapBegin"));
		PickupSound->Play();
		Drone->RestoreHealth();
		this->Destroy();
	}

}
