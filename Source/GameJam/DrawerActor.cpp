// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawerActor.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMath.h"

// Sets default values
ADrawerActor::ADrawerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the fixed drawer frame
	DrawerFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrawerFrame"));
	SetRootComponent(DrawerFrame);

	Drawer1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer1"));
	Drawer1->SetupAttachment(DrawerFrame);

	Drawer2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer2"));
	Drawer2->SetupAttachment(DrawerFrame);

	Drawer3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer3"));
	Drawer3->SetupAttachment(DrawerFrame);

}

// Called when the game starts or when spawned
void ADrawerActor::BeginPlay()
{
	Super::BeginPlay();

	Drawer1ClosedLocation = Drawer1->GetRelativeLocation();
	Drawer2ClosedLocation = Drawer2->GetRelativeLocation();
	Drawer3ClosedLocation = Drawer3->GetRelativeLocation();

	Drawer1TargetLocation = Drawer1ClosedLocation;
	Drawer2TargetLocation = Drawer2ClosedLocation;
	Drawer3TargetLocation = Drawer3ClosedLocation;

}

void ADrawerActor::ToggleDrawer(UStaticMeshComponent* HitDrawer) {

	const FVector offset( 0.0f, DrawerSlideDistance, 0.0f);

	if (HitDrawer == Drawer1) { //opening/closing 1st drawer

		drawer1IsOpen = !drawer1IsOpen;

		if (drawer1IsOpen) {
			Drawer1TargetLocation = Drawer1ClosedLocation + offset;
		}
		else {
			Drawer1TargetLocation = Drawer1ClosedLocation;
		}
	}
	else if(HitDrawer == Drawer2) { //opening/closing 2nd drawer
		
		drawer2IsOpen = !drawer2IsOpen;

		if (drawer2IsOpen) {

			Drawer2TargetLocation = Drawer2ClosedLocation + offset;
		}
		else {
			Drawer2TargetLocation = Drawer2ClosedLocation;
		}
		
		Drawer3TargetLocation = Drawer3ClosedLocation;
	}
	else if (HitDrawer == Drawer3) { //opening/closing 3rd drawer

		drawer3IsOpen = !drawer3IsOpen;

		if (drawer3IsOpen) {

			Drawer3TargetLocation = Drawer3ClosedLocation + offset;
		}
		else {
			Drawer3TargetLocation = Drawer3ClosedLocation;
		}

		
	}
}

// Called every frame
void ADrawerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Drawer1->SetRelativeLocation(FMath::VInterpTo(Drawer1->GetRelativeLocation(), Drawer1TargetLocation, DeltaTime, 5.0f));
	Drawer2->SetRelativeLocation(FMath::VInterpTo(Drawer2->GetRelativeLocation(), Drawer2TargetLocation, DeltaTime, 5.0f));
	Drawer3->SetRelativeLocation(FMath::VInterpTo(Drawer3->GetRelativeLocation(), Drawer3TargetLocation, DeltaTime, 5.0f));


}

