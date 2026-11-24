// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FBossActor.generated.h"

class USphereComponent;
class USkeletalMeshComponent;

class UCapsuleComponent;


UCLASS()
class MERUOZI_API AFBossActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFBossActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // ボスのモデル
    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* BossMesh;

    // ボス本体の当たり判定
    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* BossCollision;

    // 接近判定
    UPROPERTY(VisibleAnywhere)
    USphereComponent* AttackRange;

    // ボスのインタラクト回数
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
    int32 InteractionCount = 0;

    // 撃破に必要な回数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
    int32 RequiredInteractions = 1;

    // インタラクトできるか
    bool bCanInteract = false;

    // 接近したとき
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    // 離れたとき
    UFUNCTION()
    void OnOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

public:	
    // Fキーを押したときに呼ぶ
    void Interact(AActor* Player);

    // Fキーを押したときの処理
    void InteractWithBoss();

    // ダウンアニメーション終了後にボスを消す
    UFUNCTION(BlueprintCallable)
    void FinishDownAnimation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
