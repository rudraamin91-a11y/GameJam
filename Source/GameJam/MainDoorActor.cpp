// Fill out your copyright notice in the Description page of Project Settings.


#include "MainDoorActor.h"
#include "Components/StaticMeshComponent.h"

AMainDoorActor::AMainDoorActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMainDoorActor::BeginPlay()
{
    Super::BeginPlay();

    // Find the components created in BP_MainDoor
    TArray<UStaticMeshComponent*> MeshComponents;
    GetComponents<UStaticMeshComponent>(MeshComponents);

    for (UStaticMeshComponent* Mesh : MeshComponents)
    {
        if (!Mesh)
        {
            continue;
        }

        const FString ComponentName = Mesh->GetName();

        UE_LOG(LogTemp, Warning, TEXT("Main Door Component: %s"), *ComponentName);

        if (ComponentName.Contains(TEXT("leftDoor")))
        {
            LeftDoorMesh = Mesh;
        }
        else if (ComponentName.Contains(TEXT("rightDoor")))
        {
            RightDoorMesh = Mesh;
        }
        else if (ComponentName.Contains(TEXT("KeyLock")))
        {
            KeyLockMesh = Mesh;
        }
        else if (ComponentName.Contains(TEXT("SkullLock")))
        {
            SkullLockMesh = Mesh;
        }
    }

    // Store the original door rotations
    if (LeftDoorMesh)
    {
        LeftClosedRotation = LeftDoorMesh->GetRelativeRotation();
        LeftOpenRotation = LeftClosedRotation + FRotator(0.0f, -90.0f, 0.0f);
    }

    if (RightDoorMesh)
    {
        RightClosedRotation = RightDoorMesh->GetRelativeRotation();
        RightOpenRotation = RightClosedRotation + FRotator(0.0f, 90.0f, 0.0f);
    }

    // Report whether all components were found
    UE_LOG(LogTemp, Warning, TEXT("Left Door: %s"), *GetNameSafe(LeftDoorMesh));
    UE_LOG(LogTemp, Warning, TEXT("Right Door: %s"), *GetNameSafe(RightDoorMesh));
    UE_LOG(LogTemp, Warning, TEXT("Key Lock: %s"), *GetNameSafe(KeyLockMesh));
    UE_LOG(LogTemp, Warning, TEXT("Skull Lock: %s"), *GetNameSafe(SkullLockMesh));
}

void AMainDoorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bDoorIsOpen)
    {
        return;
    }

    // Smoothly rotate both door panels
    if (LeftDoorMesh)
    {
        FRotator NewRotation = FMath::RInterpTo(LeftDoorMesh->GetRelativeRotation(), LeftOpenRotation, DeltaTime, DoorRotationSpeed);

        LeftDoorMesh->SetRelativeRotation(NewRotation);
    }

    if (RightDoorMesh)
    {
        FRotator NewRotation = FMath::RInterpTo(RightDoorMesh->GetRelativeRotation(), RightOpenRotation, DeltaTime, DoorRotationSpeed);

        RightDoorMesh->SetRelativeRotation(NewRotation);
    }
}

void AMainDoorActor::InteractWithDoor(bool bHasRegularKey, bool bHasSkullKey)
{

    if (!bKeyLockUnlocked && bHasRegularKey)
    {
        bKeyLockUnlocked = true;

        if (KeyLockMesh)
        {
            KeyLockMesh->SetVisibility(false);
            KeyLockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        UE_LOG(LogTemp, Warning, TEXT("Regular key lock unlocked!"));
    }


    else if (!bSkullLockUnlocked && bHasSkullKey)
    {
        bSkullLockUnlocked = true;

        if (SkullLockMesh)
        {
            SkullLockMesh->SetVisibility(false);
            SkullLockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        UE_LOG(LogTemp, Warning, TEXT("Skull lock unlocked!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("You don't have the required key!"));
    }

    if (bKeyLockUnlocked && bSkullLockUnlocked && !bDoorIsOpen)
    {
        bDoorIsOpen = true;
        UE_LOG(LogTemp, Warning, TEXT("Both locks unlocked! Opening main door."));
    }
}


