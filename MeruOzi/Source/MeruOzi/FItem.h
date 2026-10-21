// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

// アイテムの種類
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Gauge,  // ゲージ回復
	Life    // 残機追加
};

UCLASS()
class MERUOZI_API AFItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFItem();


	// アイテムの種類を設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType = EItemType::Gauge;


	// アイテム取得時に増える残機
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 LifeAmount = 1;

protected:
	// アイテムの見た目
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision;

	// プレイヤーが触れたとき
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
