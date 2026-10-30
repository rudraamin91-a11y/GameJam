// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "DoorActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class GAMEJAM_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Sounds")
	USoundBase* doorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Sounds")
	USoundBase* doorCloseSound;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> rightDoor;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> leftDoor;

	bool rightDoorOpen = false;
	bool leftDoorOpen = false;

	//rotation settings
	FRotator rightClosedRotation;
	FRotator leftClosedRotation;

	FRotator rightOpenRotation;
	FRotator leftOpenRotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleDoor(UStaticMeshComponent* doorSelected);

};
