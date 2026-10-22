// Fill out your copyright notice in the Description page of Project Settings.

//AMFallingPlatform.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFallingPlatform.generated.h"

UCLASS()
class MERUOZI_API AMFallingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMFallingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	////足場のメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* PlatformMesh;

	//乗ってから落下(物理有効化)までの時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	float FallDelay = 0.5f;

	// 物理開始から非表示にするまでの時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	float HideDelay = 2.0f;

	// 非表示から復活するまでの待ち時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	float RespawnDelay = 3.0f;

	//物理シミュレーションを開始する関数
	UFUNCTION()
	void StartFalling();

	// 物理状態をリセットして見えなくする処理
	void DeactivatePlatform();

	// 復活処理
	void RespawnPlatform();

	//衝突時のコールバック  Overlap用の関数に変更
	UFUNCTION()
	
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// 初期位置と回転を保存する変数
	FVector InitialLocation;
	FRotator InitialRotation;

	FTimerHandle FallTimerHandle;
	FTimerHandle RespawnTimerHandle; // 復活・非表示管理用
};
