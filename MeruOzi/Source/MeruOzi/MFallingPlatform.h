//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "MFallingPlatform.generated.h"
//
//UCLASS()
//class MERUOZI_API AMFallingPlatform : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	// Sets default values for this actor's properties
//	AMFallingPlatform();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//	////足場のメッシュ
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
//	class UStaticMeshComponent* PlatformMesh;
//
//	//乗ってから落下(物理有効化)までの時間
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
//	float FallDelay = 0.5f;
//
//	//物理シミュレーションを開始する関数
//	UFUNCTION()
//	void StartFalling();
//
//
//	//衝突時のコールバック  Overlap用の関数に変更
//	UFUNCTION()
//	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//private:
//	FTimerHandle FallTimerHandle;
//};
