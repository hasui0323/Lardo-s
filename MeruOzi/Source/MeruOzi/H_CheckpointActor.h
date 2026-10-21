// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_CheckpointActor.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class MERUOZI_API AH_CheckpointActor : public AActor
{
	GENERATED_BODY()
	
public:
    AH_CheckpointActor();

protected:
    virtual void BeginPlay() override;

    // プレイヤーが入る範囲
    UPROPERTY(VisibleAnywhere, Category = "Checkpoint")
    TObjectPtr<UBoxComponent> TriggerBox;

    // 範囲に入ったときの処理
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
    // 触れる前のNiagara
    UPROPERTY(VisibleAnywhere, Category = "Checkpoint")
    TObjectPtr<UNiagaraComponent> NormalEffect;

    // 触れた後のNiagara
    UPROPERTY(VisibleAnywhere, Category = "Checkpoint")
    TObjectPtr<UNiagaraComponent> ActivatedEffect;

    // 触れる前のNiagara System
    UPROPERTY(EditAnywhere, Category = "Checkpoint")
    TObjectPtr<UNiagaraSystem> NormalNiagara;

    // 触れた後のNiagara System
    UPROPERTY(EditAnywhere, Category = "Checkpoint")
    TObjectPtr<UNiagaraSystem> ActivatedNiagara;

    // チェックポイントが有効になったか
    bool bIsActivated = false;

    // リスポーン地点のずらす量（チェックポイントを基準にする）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoint")
    FVector RespawnOffset = FVector(100.0f, 0.0f, 50.0f);
};
