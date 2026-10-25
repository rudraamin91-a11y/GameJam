// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGhostAIController();

	void UpdateChase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus stimulus);
	virtual void BeginPlay() override;

	FTimerHandle ChaseTimerHandle;
};
