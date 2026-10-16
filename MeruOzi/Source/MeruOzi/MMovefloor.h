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

    FVector StartLocation;

    void MovePlatform(float DeltaTime);  //
};
