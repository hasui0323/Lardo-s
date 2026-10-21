// Fill out your copyright notice in the Description page of Project Settings.


#include "H_CheckpointActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CheckpointGameInstance.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
AH_CheckpointActor::AH_CheckpointActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 範囲判定用のBoxを作成
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("TriggerBox")
    );

    SetRootComponent(TriggerBox);

    // 範囲を大きくする（X・Y・Z）
    TriggerBox->SetBoxExtent(
        FVector(150.0f, 150.0f, 150.0f));

    // プレイヤーを検知する設定
    TriggerBox->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly
    );

    TriggerBox->SetCollisionResponseToAllChannels(
        ECR_Ignore
    );

    TriggerBox->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap
    );

    TriggerBox->SetGenerateOverlapEvents(true);

    // 範囲に入ったときのイベントを登録
    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AH_CheckpointActor::OnOverlapBegin
    );

    NormalEffect = CreateDefaultSubobject<UNiagaraComponent>(
        TEXT("NormalEffect"));

    NormalEffect->SetupAttachment(TriggerBox);
    NormalEffect->SetRelativeLocation(FVector(0, 0, 100));
    NormalEffect->SetAutoActivate(false);

    ActivatedEffect = CreateDefaultSubobject<UNiagaraComponent>(
        TEXT("ActivatedEffect"));

    ActivatedEffect->SetupAttachment(TriggerBox);
    ActivatedEffect->SetRelativeLocation(FVector(0, 0, 100));
    ActivatedEffect->SetAutoActivate(false);
}

void AH_CheckpointActor::BeginPlay()
{
    Super::BeginPlay();

    // 触れる前のNiagaraを表示
    if (NormalEffect)
    {
        NormalEffect->SetVisibility(true, true);
        NormalEffect->Activate(true);
    }

    // 触れた後のNiagaraを非表示
    if (ActivatedEffect)
    {
        ActivatedEffect->Deactivate();
        ActivatedEffect->SetVisibility(false, true);
    }
}


void AH_CheckpointActor::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    // プレイヤーPawn以外は無視する
    if (!Cast<APawn>(OtherActor))
    {
        return;
    }

    // 一度起動したら繰り返さない
    if (bIsActivated)
    {
        return;
    }

    // チェックポイントを起動済みにする
    bIsActivated = true;

    // GameInstanceを取得
    UCheckpointGameInstance* GameInstance =
        Cast<UCheckpointGameInstance>(
            UGameplayStatics::GetGameInstance(this)
        );

    // リスポーン地点を保存
    if (GameInstance)
    {
        // チェックポイントの向きを基準にオフセットを適用
        GameInstance->SavedCheckpointLocation =
            GetActorLocation()
            + GetActorRotation().RotateVector(RespawnOffset);

        // チェックポイントの回転も保存
        GameInstance->SavedCheckpointRotation = GetActorRotation();

        GameInstance->bHasCheckpoint = true;

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Checkpoint saved: %s"),
            *GameInstance->SavedCheckpointLocation.ToString()
        );
    }

    // 触れる前のNiagaraを消す
    if (NormalEffect)
    {
        NormalEffect->Deactivate();
        NormalEffect->SetVisibility(false, true);
    }

    // 触れた後のNiagaraを表示
    if (ActivatedEffect)
    {
        ActivatedEffect->SetVisibility(true, true);
        ActivatedEffect->Activate(true);
    }
}
