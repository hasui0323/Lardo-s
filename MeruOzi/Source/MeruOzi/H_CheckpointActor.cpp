// Fill out your copyright notice in the Description page of Project Settings.


#include "H_CheckpointActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CheckpointGameInstance.h"
#include "GameFramework/Pawn.h"

// Sets default values
AH_CheckpointActor::AH_CheckpointActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 範囲判定用のBoxを作成
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("TriggerBox")
    );

    SetRootComponent(TriggerBox);

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

    // GameInstanceを取得
    UCheckpointGameInstance* GameInstance =
        Cast<UCheckpointGameInstance>(
            UGameplayStatics::GetGameInstance(this)
        );

    if (!GameInstance)
    {
        return;
    }

    // このチェックポイントの位置を保存
    GameInstance->SavedCheckpointLocation =
        GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);

    GameInstance->bHasCheckpoint = true;

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Checkpoint saved: %s"),
        *GameInstance->SavedCheckpointLocation.ToString()
    );
}