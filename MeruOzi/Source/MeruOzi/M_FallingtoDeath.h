// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_FallingtoDeath.generated.h"


class UBoxComponent;
UCLASS()
class MERUOZI_API AM_FallingtoDeath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AM_FallingtoDeath();

protected:

	//衝突用コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death")
	UBoxComponent* CollisionBox;

	// エディタのスポイトツール等でテレポート先アクタを指定します
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	AActor* CheckpointActor;

	// 与えるダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	float DamageAmount = 1.0f;

	// 重なり開始時の通知を受け取る関数
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		                bool bFromSweep, const FHitResult& SweepResult);
};
