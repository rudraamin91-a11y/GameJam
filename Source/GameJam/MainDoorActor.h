
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainDoorActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class GAMEJAM_API AMainDoorActor : public AActor
{
    GENERATED_BODY()

public:
    AMainDoorActor();

    virtual void Tick(float DeltaTime) override;

    void InteractWithDoor(bool bHasRegularKey, bool bHasSkullKey);

protected:
    virtual void BeginPlay() override;

private:

    //References to existing Blueprint components
    UStaticMeshComponent* LeftDoorMesh = nullptr;
    UStaticMeshComponent* RightDoorMesh = nullptr;
    UStaticMeshComponent* KeyLockMesh = nullptr;
    UStaticMeshComponent* SkullLockMesh = nullptr;

    //Lock states
    bool bKeyLockUnlocked = false;
    bool bSkullLockUnlocked = false;
    bool bDoorIsOpen = false;

    //Door rotations
    FRotator LeftClosedRotation;
    FRotator RightClosedRotation;

    FRotator LeftOpenRotation;
    FRotator RightOpenRotation;

    float DoorRotationSpeed = 3.0f;
};

