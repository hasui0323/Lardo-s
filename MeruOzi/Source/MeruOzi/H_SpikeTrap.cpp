// Fill out your copyright notice in the Description page of Project Settings.


#include "H_SpikeTrap.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

// Sets default values
AH_SpikeTrap::AH_SpikeTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // 床を作成
    FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("FloorMesh"));
    SetRootComponent(FloorMesh);

    // とげを作成
    SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("SpikeMesh"));
    SpikeMesh->SetupAttachment(FloorMesh);

    // ダメージ判定用の箱
    DamageBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("DamageBox"));
    DamageBox->SetupAttachment(FloorMesh);

    DamageBox->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);
    DamageBox->SetCollisionResponseToAllChannels(
        ECR_Ignore);
    DamageBox->SetCollisionResponseToChannel(
        ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AH_SpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	
    // とげの初期位置を保存
    HiddenLocation = SpikeMesh->GetRelativeLocation();

    // 飛び出した位置
    ExtendedLocation = HiddenLocation
        + FVector(0.0f, 0.0f, SpikeHeight);

    // 最初は隠す
    SpikeMesh->SetRelativeLocation(HiddenLocation);

    // ダメージ判定を無効化
    DamageBox->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    // 一定時間ごとにとげを出す
    GetWorldTimerManager().SetTimer(
        SpikeTimerHandle,
        this,
        &AH_SpikeTrap::ExtendSpikes,
        WaitTime,
        false);

    // プレイヤーを取得
    ACharacter* Player =
        UGameplayStatics::GetPlayerCharacter(this, 0);

    if (Player)
    {
        // ゲーム開始時の位置と向きを保存
        PlayerStartLocation = Player->GetActorLocation();
        PlayerStartRotation = Player->GetActorRotation();
    }
}

void AH_SpikeTrap::ExtendSpikes()
{
    // とげを飛び出させる
    SpikeMesh->SetRelativeLocation(ExtendedLocation);

    // ダメージ判定を有効化
    DamageBox->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);

    bSpikeActive = true;

    // 一定間隔でダメージ判定を繰り返す
    GetWorldTimerManager().SetTimer(
        DamageTimerHandle,
        this,
        &AH_SpikeTrap::CheckDamage,
        DamageInterval,
        true);

    // 一定時間後に引っ込める
    GetWorldTimerManager().SetTimer(
        SpikeTimerHandle,
        this,
        &AH_SpikeTrap::RetractSpikes,
        ActiveTime,
        false);
}

void AH_SpikeTrap::RetractSpikes()
{
    // ダメージ判定のタイマーを停止
    GetWorldTimerManager().ClearTimer(DamageTimerHandle);

    // とげを隠す
    SpikeMesh->SetRelativeLocation(HiddenLocation);

    // ダメージ判定を無効化
    DamageBox->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    bSpikeActive = false;

    // 次に飛び出すまで待つ
    GetWorldTimerManager().SetTimer(
        SpikeTimerHandle,
        this,
        &AH_SpikeTrap::ExtendSpikes,
        WaitTime,
        false);
}

void AH_SpikeTrap::CheckDamage()
{
    if (!bSpikeActive)
    {
        return;
    }

    TArray<AActor*> OverlappingActors;

    DamageBox->GetOverlappingActors(
        OverlappingActors,
        ACharacter::StaticClass());

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor)
        {
            // プレイヤーを初期位置に戻す
            Actor->SetActorLocation(
                PlayerStartLocation);

            // プレイヤーの向きも初期状態に戻す
            Actor->SetActorRotation(
                PlayerStartRotation);

            // 同じ判定で何度も処理しないように終了
            return;
        }
    }
}
