// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "GameJamCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"

AKeyPickup::AKeyPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    keyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
    SetRootComponent(keyMesh);

    PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));

    PickupRange->SetupAttachment(RootComponent);
    PickupRange->SetSphereRadius(200.0f);

    PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickupRange->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickupRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    PickupRange->OnComponentBeginOverlap.AddDynamic(this, &AKeyPickup::onPickupRangeStart);
    PickupRange->OnComponentEndOverlap.AddDynamic(this, &AKeyPickup::onPickupRangeEnd);
}

void AKeyPickup::onPickupRangeStart(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

    AGameJamCharacter* player = Cast<AGameJamCharacter>(otherActor);

    if (player) {
        closePlayer = player;

        UE_LOG(LogTemp, Warning, TEXT("Player near key...Press R to pickup."));
    }
}

void AKeyPickup::onPickupRangeEnd(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex) {

    if (otherActor == closePlayer) {
        closePlayer = nullptr;

        UE_LOG(LogTemp, Warning, TEXT("Player left the key range"));
    }
}

void AKeyPickup::NotifyActorBeginOverlap(AActor* otherActor) {
    
    Super::NotifyActorBeginOverlap(otherActor);

    AGameJamCharacter* player = Cast<AGameJamCharacter>(otherActor);

    if (player) {
        player->keyNearby = this;

        UE_LOG(LogTemp, Warning, TEXT("Player near key...Press R to pickup."));
    }
}

void AKeyPickup::NotifyActorEndOverlap(AActor* otherActor) {

    Super::NotifyActorEndOverlap(otherActor);

    AGameJamCharacter* player = Cast<AGameJamCharacter>(otherActor);

    if (player && player->keyNearby == this) {

        player->keyNearby = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("Player left key range."));
    }
}

