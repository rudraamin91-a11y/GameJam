// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Engine/TargetPoint.h"

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

void AGhostAIController::OnTargetDetected(
	AActor* Actor,
	FAIStimulus Stimulus)
{
	if (!Actor || Actor == GetPawn())
	{
		return;
	}

	ACharacter* Player =
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Actor != Player)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		bIsChasing = true;

		UpdateChase();

		GetWorldTimerManager().SetTimer(
			ChaseTimerHandle,
			this,
			&AGhostAIController::UpdateChase,
			0.5f,
			true
		);
	}
	else
	{
		bIsChasing = false;

		GetWorldTimerManager().ClearTimer(ChaseTimerHandle);

		MoveToNextPatrolPoint();
	}
}

void AGhostAIController::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), PatrolPoints);

	if (PatrolPoints.Num() > 0) {
		MoveToNextPatrolPoint();
	}
}

void AGhostAIController::MoveToNextPatrolPoint() {
	if (PatrolPoints.Num() == 0) {
		return;
	}

	AActor* target = PatrolPoints[currentPatrolIndex];

	if (target) {
		MoveToActor(target, 50.0f);
	}

	currentPatrolIndex = (currentPatrolIndex + 1) % PatrolPoints.Num();
}

void AGhostAIController::OnMoveCompleted(
	FAIRequestID RequestID,
	const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (!bIsChasing && Result.IsSuccess())
	{
		MoveToNextPatrolPoint();
	}
}

