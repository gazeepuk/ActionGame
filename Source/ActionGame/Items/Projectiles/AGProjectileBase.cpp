// replcae later


#include "AGProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"


AAGProjectileBase::AAGProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBox =  CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileCollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 700.f;
	ProjectileMovementComp->MaxSpeed = 900.f;
	ProjectileMovementComp->Velocity = FVector(1.f,0.f,0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

void AAGProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if(ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	}
}

void AAGProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if(!HitPawn || !UAGFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UAGFunctionLibrary::NativeDoesActorHaveTag(OtherActor, AGGameplayTags::Player_Status_Blocking);
	if(bIsPlayerBlocking)
	{
		bIsValidBlock = UAGFunctionLibrary::IsValidBlock(this, OtherActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = HitPawn;
	
	if(bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, AGGameplayTags::Player_Event_SuccessfulBlock, EventData);
	}
	else
	{
		HandleApplyProjectileDamage(HitPawn, EventData);
	}

	Destroy();
}

void AAGProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AAGProjectileBase::HandleApplyProjectileDamage(APawn* InTarget, const FGameplayEventData& InPayload) const
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Projectile {%s} Effect Spec Handle is nullptr"), *GetNameSafe(this));
	const bool bWasApplied = UAGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InTarget, ProjectileDamageEffectSpecHandle);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InTarget, AGGameplayTags::Shared_Event_HitReact, InPayload);
}
