// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_SpikeTrap.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTimelineComponent;

UCLASS()
class MERUOZI_API AH_SpikeTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AH_SpikeTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // 床
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* FloorMesh;

    // とげのメッシュ
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* SpikeMesh;

    // ダメージ判定
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* DamageBox;

    // とげが飛び出すまでの時間
    UPROPERTY(EditAnywhere, Category = "Spike")
    float WaitTime = 2.0f;

    // とげが出ている時間
    UPROPERTY(EditAnywhere, Category = "Spike")
    float ActiveTime = 1.5f;

    // とげが上下する距離
    UPROPERTY(EditAnywhere, Category = "Spike")
    float SpikeHeight = 2.0f;

    // ダメージ量
    UPROPERTY(EditAnywhere, Category = "Spike")
    float Damage = 1.0f;

    FVector HiddenLocation;
    FVector ExtendedLocation;

    bool bSpikeActive = false;

    FTimerHandle SpikeTimerHandle;

    void ExtendSpikes();
    void RetractSpikes();
    void CheckDamage();

    // ダメージ判定を繰り返すタイマー
    FTimerHandle DamageTimerHandle;

    // ダメージ判定の間隔
    UPROPERTY(EditAnywhere, Category = "Spike")
    float DamageInterval = 0.1f;

    // プレイヤーの初期位置
    FVector PlayerStartLocation;

    // プレイヤーの初期回転
    FRotator PlayerStartRotation;
};
