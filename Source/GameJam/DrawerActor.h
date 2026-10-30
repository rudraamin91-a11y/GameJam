// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "DrawerActor.generated.h"

UCLASS()
class GAMEJAM_API ADrawerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawerActor();

	void ToggleDrawer(UStaticMeshComponent* HitDrawer);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//created references for the drawers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* DrawerFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* Drawer1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* Drawer2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* Drawer3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawer Sounds")
	USoundBase* drawerOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawer Sounds")
	USoundBase* drawerCloseSound;


private:
	bool drawer1IsOpen = false;
	bool drawer2IsOpen = false;
	bool drawer3IsOpen = false;

	FVector Drawer1ClosedLocation;
	FVector Drawer2ClosedLocation;
	FVector Drawer3ClosedLocation;

	FVector Drawer1TargetLocation;
	FVector Drawer2TargetLocation;
	FVector Drawer3TargetLocation;

	float DrawerSlideDistance = 30.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
