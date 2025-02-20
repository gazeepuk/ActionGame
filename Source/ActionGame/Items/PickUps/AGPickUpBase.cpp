// replcae later


#include "Items/PickUps/AGPickUpBase.h"

#include "Components/SphereComponent.h"

AAGPickUpBase::AAGPickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	SetRootComponent(PickUpCollisionSphere);
	PickUpCollisionSphere->InitSphereRadius(50.f);
	PickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpCollisionSphereBeginOverlap);
	PickUpCollisionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpCollisionSphereEndOverlap);
}

void AAGPickUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AAGPickUpBase::OnPickUpCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

