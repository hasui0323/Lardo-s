// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CheckpointGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MERUOZI_API UCheckpointGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    // 最後に通過したチェックポイントの位置
    UPROPERTY(BlueprintReadWrite, Category = "Checkpoint")
    FVector SavedCheckpointLocation = FVector::ZeroVector;

    // チェックポイントが保存されているか
    UPROPERTY(BlueprintReadWrite, Category = "Checkpoint")
    bool bHasCheckpoint = false;

    // 最後に通過したチェックポイントの回転
    UPROPERTY(BlueprintReadWrite, Category = "Checkpoint")
    FRotator SavedCheckpointRotation = FRotator::ZeroRotator;
};
