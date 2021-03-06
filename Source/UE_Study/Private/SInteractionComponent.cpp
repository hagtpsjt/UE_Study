// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract() 
{
	constexpr float RADIUS = 30.f;
	constexpr bool bDrawDebug = true;
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(GetOwner());
	
	FVector EyeLocation;
	FRotator EyeRotation;	

	FCollisionShape Shape;
	Shape.SetSphere(RADIUS);
	
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape, QueryParams);
	FColor LineColor = bBlockingHit? FColor::Green : FColor::Red;
	if(bDrawDebug) DrawDebugLine(GetWorld(), EyeLocation, End, LineColor,false, 2.0f, 0, 2.0f);
	
	for(FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor && HitActor->Implements<USGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn, AActor>(GetOwner());
			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}

		if(bDrawDebug) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, RADIUS, 32, LineColor, false, 2.0f);
	}	
}
