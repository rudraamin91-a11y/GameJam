// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"

AGhostAIController::AGhostAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	SetPerceptionComponent(*AIPerception);

	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AGhostAIController::OnTargetDetected);

}

void AGhostAIController::UpdateChase() {
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Player) {
		MoveToActor(Player, 150.0f);
	}
}

void AGhostAIController::OnTargetDetected(AActor* Actor, FAIStimulus stimulus) {
	
	if (!Actor || !Actor->IsA(ACharacter::StaticClass())) {
		return;
	}

	if (stimulus.WasSuccessfullySensed()) {

		UpdateChase();

		GetWorldTimerManager().SetTimer(ChaseTimerHandle, this, &AGhostAIController::UpdateChase, 0.5f, true);
		
	}
	else {
		GetWorldTimerManager().ClearTimer(ChaseTimerHandle);
	}
}

void AGhostAIController::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Ghost AI Controller started"));

	if (GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ghost is possessed"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ghost is NOT possessed"));
	}
}

