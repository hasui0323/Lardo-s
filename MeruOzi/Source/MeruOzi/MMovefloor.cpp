// Fill out your copyright notice in the Description page of Project Settings.


#include "MMovefloor.h"

// Sets default values
AMMovefloor::AMMovefloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

}

// Called when the game starts or when spawned
void AMMovefloor::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	MoveDirection.Normalize();
}

// Called every frame
void AMMovefloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePlatform(DeltaTime);
}

void AMMovefloor::StartReturning()
{
	bReturning = true;
	WaitTimer = 0.f; // 端で待機中でもすぐ戻り始める
}

void AMMovefloor::MovePlatform(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	//移動ベクトルを計算して現在の位置に追加
	CurrentLocation += MoveDirection * MoveSpeed * DeltaTime;

	//bSweepをtrueにすることで、上に乗っているプレイヤーを押し出したり運んだりする
	FHitResult Hit;
	SetActorLocation(CurrentLocation, true, &Hit);

	//動き出し開始地点からの距離をチェック
	float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);

	if (DistanceMoved >= MoveDistance)
	{
		//誤差を排除(Eliminate)するために、移動方向を反転し、開始地点を更新して位置を補正する
		MoveDirection = -MoveDirection;
		StartLocation = GetActorLocation();
	}
}