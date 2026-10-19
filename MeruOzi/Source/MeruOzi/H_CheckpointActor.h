// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_CheckpointActor.generated.h"

class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class MERUOZI_API AH_CheckpointActor : public AActor
{
	GENERATED_BODY()
	
public:
    AH_CheckpointActor();

protected:
    // ƒvƒŒƒCƒ„[‚ª“ü‚é”ÍˆÍ
    UPROPERTY(VisibleAnywhere, Category = "Checkpoint")
    TObjectPtr<UBoxComponent> TriggerBox;

    // ”ÍˆÍ‚É“ü‚Á‚½‚Æ‚«‚Ìˆ—
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
