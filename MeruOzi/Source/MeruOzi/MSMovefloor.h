// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSMovefloor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class MERUOZI_API AMSMovefloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMSMovefloor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


    //床のメッシュ
    UPROPERTY(VisibleAnywhere, Category = "Platform")
    TObjectPtr<UStaticMeshComponent> PlatformMesh;

    //プレイヤーが上に乗っているか見る
    UPROPERTY(VisibleAnywhere, Category = "Platform")
    TObjectPtr<UBoxComponent> TriggerBox;

    // 移動方向 (アクターのローカル空間)。長さは自動で 1 に正規化される
    UPROPERTY(EditAnywhere, Category = "Platform")
    FVector MoveDirection = FVector(0.f, 0.f, 1.f);

    // 移動距離 (cm)
    UPROPERTY(EditAnywhere, Category = "Platform", meta = (ClampMin = "0.0"))
    float MoveDistance = 300.f;

    UPROPERTY(EditAnywhere, Category = "Platform", meta = (ClampMin = "1.0"))
    float Speed = 200.f;

    UPROPERTY(EditAnywhere, Category = "Platform")
    bool bPingPong = true;

    UPROPERTY(EditAnywhere, Category = "Platform", meta = (ClampMin = "0.0"))
    float WaitTime = 1.f;

    // プレイヤーが降りてから戻り始めるまでの猶予 (秒)
    UPROPERTY(EditAnywhere, Category = "Platform", meta = (ClampMin = "0.0"))
    float ReturnDelay = 0.5f;

private:
    static bool IsPlayerPawn(const AActor* Actor);
    bool IsAnyPlayerOnPlatform(const AActor* IgnoreActor = nullptr) const;
    void StartReturning();
    void StopAtStart();

    FVector StartLocation;//開始位置(ワールド座標)
    FVector EndLocation;//移動先(ワールド座標)

    bool bPlayerOnBoard = false;//プレイヤーが乗っているか
    bool bReturning = false;//床が戻っているか
    bool bMovingToEnd = true;//往復時の変更方向
    float WaitTimer = 0.f;//端に到達して停止している時間の残り
    float ReturnDelayTimer = 0.f;//戻り始めるまでの猶予の残り
};
