// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMovefloor.generated.h"

UCLASS()
class MERUOZI_API AMMovefloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMovefloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
    virtual void Tick(float DeltaTime) override; //
private:
    // 表示用のメッシュ
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* PlatformMesh;

    // 移動速度 (cm/s)
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 100.f;

    // 移動する最大距離
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveDistance = 500.f;

    // 移動方向 (正規化されたベクトル)
    UPROPERTY(EditAnywhere, Category = "Movement")
  FVector MoveDirection = FVector(1, 0, 0);
    //端についてから折り返し時間
    UPROPERTY(EditAnywhere, Category = "Platform", meta = (ClampMin = "0.0"))
    float WaitTime = 1.f;

  

    FVector StartLocation;

    bool bReturning = false;//床が戻っているか
    bool bMovingToEnd = true;//往復時の変更方向

    float WaitTimer = 0.f;//端に到達して停止している時間の残り

    void MovePlatform(float DeltaTime);  //
    void StartReturning();
};
