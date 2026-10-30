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

    
    TArray<UStaticMeshComponent*> MeshComponents;
    GetComponents<UStaticMeshComponent>(MeshComponents);

    for (UStaticMeshComponent* Mesh : MeshComponents)
    {
        if (!Mesh)
        {
            continue;
        }

        const FString ComponentName = Mesh->GetName();


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

    //Stored the original door rotations
    if (LeftDoorMesh)
    {
        LeftClosedRotation = LeftDoorMesh->GetRelativeRotation();
        LeftOpenRotation = LeftClosedRotation + FRotator(0.0f, -90.0f, 0.0f);
    }

    if (RightDoorMesh)
    {
        RightClosedRotation = RightDoorMesh->GetRelativeRotation();
        RightClosedLocation = RightDoorMesh->GetRelativeLocation();

        RightOpenLocation = FVector(110.0f, 0.0f, 142.509f);
        RightOpenRotation = RightClosedRotation + FRotator(0.0f, 90.0f, 0.0f);

    }
}

void AMainDoorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bDoorIsOpen)
    {
        return;
    }

    //smoothly rotate both door panels
    if (LeftDoorMesh)
    {
        FRotator NewRotation = FMath::RInterpTo(LeftDoorMesh->GetRelativeRotation(), LeftOpenRotation, DeltaTime, DoorRotationSpeed);

        LeftDoorMesh->SetRelativeRotation(NewRotation);
    }

    if (RightDoorMesh)
    {
        FRotator NewRotation = FMath::RInterpTo(RightDoorMesh->GetRelativeRotation(), RightOpenRotation, DeltaTime, DoorRotationSpeed);
        FVector NewLocation = FMath::VInterpTo(RightDoorMesh->GetRelativeLocation(), RightOpenLocation, DeltaTime, DoorRotationSpeed);

        RightDoorMesh->SetRelativeRotation(NewRotation);
        RightDoorMesh->SetRelativeLocation(NewLocation);
    }
}

void AMainDoorActor::InteractWithDoor(bool& HasRegularKey, bool& HasSkullKey)
{

    if (!bKeyLockUnlocked && HasRegularKey)
    {
        bKeyLockUnlocked = true;
        HasRegularKey = false;

        if (KeyLockMesh)
        {
            KeyLockMesh->SetVisibility(false);
            KeyLockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }

    else if (!bSkullLockUnlocked && HasSkullKey)
    {
        bSkullLockUnlocked = true;
        HasSkullKey = false;

        if (SkullLockMesh)
        {
            SkullLockMesh->SetVisibility(false);
            SkullLockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

    }
    else
    {
        return;
    }

    if (bKeyLockUnlocked && bSkullLockUnlocked && !bDoorIsOpen)
    {
        bDoorIsOpen = true;
    }
}


