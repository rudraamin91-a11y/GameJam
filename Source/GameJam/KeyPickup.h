// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class GAMEJAM_API AKeyPickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKeyPickup();

	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;
	virtual void NotifyActorEndOverlap(AActor* otherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool IsSkullKey = false;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> keyMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> PickupRange;

	UFUNCTION()
	void onPickupRangeStart(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void onPickupRangeEnd(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex);

	UPROPERTY()

	TObjectPtr<class AGameJamCharacter> closePlayer;
};
