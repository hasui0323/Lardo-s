// Fill out your copyright notice in the Description page of Project Settings.


#include "MMovefloor.h"

// Sets default values
AMMovefloor::AMMovefloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMeshComponentの作成
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	// コンポーネントのアタッチ
	Mesh->SetupAttachment(RootComponent);

	// ObjectとしてStaticMeshオブジェクトをロード
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));

	// キューブアセットのロードに成功した場合
	if (CubeVisualAsset.Succeeded())
	{
		// モデルのセットと位置のセット
		Mesh->SetStaticMesh(CubeVisualAsset.Object);
	}

	// メッシュの大きさを指定
	Mesh->SetRelativeScale3D(FVector(5, 5, 1.5f));

	// RotatingMovementComponentのコンポーネント設定
	Rotating = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));

	// 回転する軸の位置を指定
	Rotating->PivotTranslation = FVector(250, 250, 250);

	// 回転量を設定
	MeshRotationValue = FRotator(100, 200, 0);
}

// Called when the game starts or when spawned
void AMMovefloor::BeginPlay()
{
	Super::BeginPlay();

	// 回転する軸に回転量を設定
	Rotating->RotationRate = MeshRotationValue;
	
}

// Called every frame
void AMMovefloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

