//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "MFallingPlatform.h"
//#include "Components/StaticMeshComponent.h"
//#include "GameFramework/Character.h"
//#include "TimerManager.h"
//
//AMFallingPlatform::AMFallingPlatform()
//{
//    PrimaryActorTick.bCanEverTick = false;
//
//    // ルートコンポーネントとしてスタティックメッシュを作成
//    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
//    RootComponent = PlatformMesh;
//
//    // 初期状態では物理シミュレーションを無効にする
//    PlatformMesh->SetSimulatePhysics(false);
//    // 衝突イベントを通知するように設定
//    PlatformMesh->SetNotifyRigidBodyCollision(true);
//}
//
//void AMFallingPlatform::BeginPlay()
//{
//    Super::BeginPlay();
//    // Hitイベントをバインド
//    PlatformMesh->OnComponentHit.AddDynamic(this, &AFallingPlatform::StartFalling());
//}
//
//void AMFallingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Overlap Detected with Player!"));
//
//        if (!GetWorldTimerManager().IsTimerActive(FallTimerHandle))
//        {
//            GetWorldTimerManager().SetTimer(FallTimerHandle, this, &AMFallingPlatform::StartFalling, FallDelay, false);
//        }
//    }
//}
//
//void AMFallingPlatform::StartFalling()
//{
//    if (PlatformMesh)
//    {
//        // 物理開始前に、再度可動性をチェックしてログに出す
//        if (PlatformMesh->Mobility != EComponentMobility::Movable)
//        {
//            UE_LOG(LogTemp, Error, TEXT("FAILED: Platform Mobility is NOT Movable!"));
//            return;
//        }
//
//        // 物理演算を開始する前にコリジョンをBlockに戻す（足場から抜けないように）
//        PlatformMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
//        PlatformMesh->SetSimulatePhysics(true);
//
//        UE_LOG(LogTemp, Warning, TEXT("Platform is falling now!"));
//
//        // 不要な足場の除去（Elimination）
//        SetLifeSpan(5.0f);
//    }
//}
//
////
////// Sets default values
////AMFallingPlatform::AMFallingPlatform()
////{
//// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
////	PrimaryActorTick.bCanEverTick = false;
////
////	//ルートコンポーネントとしてスタティックメッシュを作成
////	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>
////		(TEXT("PlatformMesh"));
////	RootComponent = PlatformMesh;
////
////	//初期状態では物理シミュレーションを無効にする
////	PlatformMesh->SetSimulatePhysics(false);
////	//衝突イベントを通知するように設定
////	PlatformMesh->SetNotifyRigidBodyCollision(true);
////
////}
////
////// Called when the game starts or when spawned
////void AMFallingPlatform::BeginPlay()
////{
////	Super::BeginPlay();
////
////	//Hitイベントをバインド
////	PlatformMesh->OnComponentHit.AddDynamic(this, &AMFallingPlatform::OnPlatformHit);
////	
////}
////
////void AMFallingPlatform::OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
////{
////	//衝突したActorがプレイヤー(ACharactor)かどうかを確認
////	if (OtherActor && OtherActor -> IsA(ACharacter::StaticClass()))
////	{
////		//すでに落下タイマーが動いてないことを確認
////		if (!GetWorldTimerManager().IsTimerActive(FallTimerHandle))
////		{
////			//指定秒数語にStartFallingを実行
////
////			GetWorldTimerManager().SetTimer(FallTimerHandle, this, &AMFallingPlatform::StartFalling, FallDelay, false);
////		}
////	}
////}
////
////void AMFallingPlatform::StartFalling()
////{
////	if (PlatformMesh)
////	{
////		//物理趣味レーションを有効化して重力で落下させる
////		PlatformMesh->SetSimulatePhysics(true);
////
////		//落下後,3秒後にActorを消去(メモリの最適化)
////		SetLifeSpan(3.0f);
////	}
////}
//
