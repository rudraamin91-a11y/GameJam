// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

ADoorActor::ADoorActor()
{
 	//Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

}

//Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> DoorMeshes;
	GetComponents<UStaticMeshComponent>(DoorMeshes);

	for (UStaticMeshComponent* Mesh : DoorMeshes)
	{
		if (Mesh->GetName().Contains(TEXT("SM_Door02_D")))
		{
			rightDoor = Mesh;
		}
		else if (Mesh->GetName().Contains(TEXT("SM_Door02_E")))
		{
			leftDoor = Mesh;
		}
	}

	if (rightDoor) {

		rightClosedRotation = rightDoor->GetRelativeRotation();
		rightOpenRotation = rightClosedRotation + FRotator(0.0f, 90.0f, 0.0f);
	}

	if (leftDoor) {
		
		leftClosedRotation = leftDoor->GetRelativeRotation();
		leftOpenRotation = leftClosedRotation + FRotator(0.0f, -90.0f, 0.0f);
	}
	
}

//Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rightDoor) {

		FRotator TargetRotation;

		if (rightDoorOpen) {

			TargetRotation = rightOpenRotation;
		}
		else {
			TargetRotation = rightClosedRotation;
		}

		FRotator NewRotation = FMath::RInterpTo(rightDoor->GetRelativeRotation(), TargetRotation, DeltaTime, 3.0f);

		rightDoor->SetRelativeRotation(NewRotation);
	}

	if (leftDoor) {

		FRotator TargetRotation;

		if (leftDoorOpen) {

			TargetRotation = leftOpenRotation;
		}
		else {
			TargetRotation = leftClosedRotation;
		}

		FRotator NewRotation = FMath::RInterpTo(leftDoor->GetRelativeRotation(), TargetRotation, DeltaTime, 3.0f);

		leftDoor->SetRelativeRotation(NewRotation);
	}

}

void ADoorActor::ToggleDoor(UStaticMeshComponent* selectedDoor) {
	
	if (selectedDoor == rightDoor) {

		rightDoorOpen = !rightDoorOpen;

		if (rightDoorOpen) {
			
			if (doorOpenSound) {
				UGameplayStatics::PlaySoundAtLocation(this, doorOpenSound, rightDoor->GetComponentLocation());
			}
		}
		else {
			if (doorCloseSound) {
				UGameplayStatics::PlaySoundAtLocation(this, doorCloseSound, rightDoor->GetComponentLocation());
			}
		}
	}
	else if (selectedDoor == leftDoor) {
		leftDoorOpen = !leftDoorOpen;

		if (leftDoorOpen) {

			if (doorOpenSound) {
				UGameplayStatics::PlaySoundAtLocation(this, doorOpenSound, leftDoor->GetComponentLocation());
			}
		}
		else {

			if (doorCloseSound) {
				UGameplayStatics::PlaySoundAtLocation(this, doorCloseSound, leftDoor->GetComponentLocation());
			}
		}
	}
}

