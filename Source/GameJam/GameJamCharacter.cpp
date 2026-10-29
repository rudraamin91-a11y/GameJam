// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJamCharacter.h"
#include "Engine/OverlapResult.h"
#include "EngineUtils.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameJam.h"
#include "KeyPickup.h"
#include "DrawerActor.h"
#include "Engine/TargetPoint.h"
#include <Kismet/GameplayStatics.h>

AGameJamCharacter::AGameJamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void AGameJamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGameJamCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGameJamCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameJamCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameJamCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGameJamCharacter::LookInput);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AGameJamCharacter::Interact);

		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &AGameJamCharacter::pickupKey);
	}
	else
	{
		UE_LOG(LogGameJam, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGameJamCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AGameJamCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AGameJamCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AGameJamCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AGameJamCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AGameJamCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AGameJamCharacter::Interact()
{

	if (keyNearby) {

		UE_LOG(LogTemp, Warning, TEXT("Key picked up!"));

		keyNearby->Destroy();
		keyNearby = nullptr;

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("F pressed - Interact called"));
	FVector start = FirstPersonCameraComponent->GetComponentLocation();

	FVector end = start + FirstPersonCameraComponent->GetForwardVector() * 500.0f;

	FHitResult Hit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool ifHit = GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_Visibility, QueryParams);

	if (!ifHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit nothing"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"),
		*GetNameSafe(Hit.GetActor()));

	UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"),
		*GetNameSafe(Hit.GetComponent()));

	ADrawerActor* drawer = Cast<ADrawerActor>(Hit.GetActor());


	/*if (ifHit) {
		ADrawerActor* drawer = Cast<ADrawerActor>(Hit.GetActor());

		if (drawer) {
			UStaticMeshComponent* HitDrawer = Cast<UStaticMeshComponent>(Hit.GetComponent());

			if (HitDrawer) {
				drawer->ToggleDrawer(HitDrawer);
			}
		} 
	}*/

	if (!drawer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor is not a DrawerActor"));
		return;
	}

	UStaticMeshComponent* HitDrawer =
		Cast<UStaticMeshComponent>(Hit.GetComponent());

	if (HitDrawer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling ToggleDrawer"));
		drawer->ToggleDrawer(HitDrawer);
	}
}

void AGameJamCharacter::respawnPlayer() {
	
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());

		UE_LOG(LogTemp, Warning, TEXT("Player respawned at PlayerStart!"));

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PlayerStart not found!"));
	}
		

	UE_LOG(LogTemp, Error, TEXT("PlayerRespawnPoint not found!"));
}

void AGameJamCharacter::pickupKey() {

	if (!keyNearby) {
		return; //ends the function if player press R when key is not detected
	}

	if (keyNearby->IsSkullKey) {

		hasSkullKey = true;
		UE_LOG(LogTemp, Warning, TEXT("Skull key added.."));

	}
	else {
		hasRegularKey = true;
		UE_LOG(LogTemp, Warning, TEXT("Regular key added.."));
	}
	

	keyNearby->Destroy();
	keyNearby = nullptr;
}
