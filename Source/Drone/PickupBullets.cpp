// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBullets.h"
#include "DronePawn.h"

// Sets default values
APickupBullets::APickupBullets()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BaseCapsuleComponent"));
	BaseCollisionComponent->SetupAttachment(RootComponent);
	BaseCollisionComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	PickupMesh->SetStaticMesh(CubeMesh);
	PickupMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetupAttachment(BaseCollisionComponent);


}

// Called when the game starts or when spawned
void APickupBullets::BeginPlay()
{
	Super::BeginPlay();
	BaseCollisionComponent->SetGenerateOverlapEvents(true);
	BaseCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBullets::OnOverlapBegin);
	PickupMesh->SetGenerateOverlapEvents(false);
}

// Called every frame
void APickupBullets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBullets::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADronePawn* Drone = Cast<ADronePawn>(OtherActor);
	if (Drone != nullptr)
	{
		Drone->RestoreBullets();
		this->Destroy();
	}
}

