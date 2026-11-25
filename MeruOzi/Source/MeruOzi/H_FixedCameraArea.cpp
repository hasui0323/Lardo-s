// Fill out your copyright notice in the Description page of Project Settings.

#include "H_FixedCameraArea.h"

#include "Engine/Engine.h"

#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"

#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Blueprint/UserWidget.h"

#include "H_CameraControlWidgetBase.h"

// Sets default values
AH_FixedCameraArea::AH_FixedCameraArea()
{
    PrimaryActorTick.bCanEverTick = false;

    // 範囲判定用のBox
    Trigger = CreateDefaultSubobject<UBoxComponent>(
        TEXT("Trigger"));

    SetRootComponent(Trigger);

    // 判定範囲を現在の2倍にする
    Trigger->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));

    Trigger->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);

    Trigger->SetCollisionResponseToAllChannels(
        ECR_Ignore);

    Trigger->SetCollisionResponseToChannel(
        ECC_Pawn, ECR_Overlap);

    // 固定カメラ
    FixedCamera = CreateDefaultSubobject<UCameraComponent>(
        TEXT("FixedCamera"));

    FixedCamera->SetupAttachment(RootComponent);

    // 光の柱
    LightPillar = CreateDefaultSubobject<UNiagaraComponent>(
        TEXT("LightPillar"));

    LightPillar->SetupAttachment(RootComponent);
    LightPillar->SetAutoActivate(true);
}

// Called when the game starts or when spawned
void AH_FixedCameraArea::BeginPlay()
{
    Super::BeginPlay();

    Trigger->OnComponentBeginOverlap.AddDynamic(
        this, &AH_FixedCameraArea::OnBeginOverlap);

    Trigger->OnComponentEndOverlap.AddDynamic(
        this, &AH_FixedCameraArea::OnEndOverlap);

    // Niagaraエフェクトを設定
    if (LightPillarEffect)
    {
        LightPillar->SetAsset(LightPillarEffect);
        LightPillar->Activate(true);
    }
}

void AH_FixedCameraArea::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    APawn* PlayerPawn =
        UGameplayStatics::GetPlayerPawn(this, 0);

    // プレイヤー以外は無視
    if (OtherActor != PlayerPawn || bPlayerInRange)
    {
        return;
    }

    bPlayerInRange = true;

    // Fキーの案内を表示
    ShowPrompt();

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (PC)
    {
        // 範囲内でFキーを受け取れるようにする
        EnableInput(PC);

        // 入力を重複登録しない
        if (InputComponent && !bInputBound)
        {
            InputComponent->BindKey(
                EKeys::F,
                IE_Pressed,
                this,
                &AH_FixedCameraArea::SwitchCamera);

            bInputBound = true;
        }
    }
}

void AH_FixedCameraArea::OnEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    APawn* PlayerPawn =
        UGameplayStatics::GetPlayerPawn(this, 0);

    if (OtherActor != PlayerPawn)
    {
        return;
    }

    bPlayerInRange = false;

    // 案内文字を必ず消す
    HidePrompt();

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (PC)
    {
        DisableInput(PC);
    }

    // 範囲外に出たら通常視点に戻す
    if (bCameraFixed)
    {
        RestoreCamera();
    }
}

void AH_FixedCameraArea::SwitchCamera()
{
    if (!bPlayerInRange)
    {
        return;
    }

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    APawn* PlayerPawn =
        UGameplayStatics::GetPlayerPawn(this, 0);

    if (!PC)
    {
        return;
    }

    if (!bCameraFixed)
    {
        // 現在の視点を保存
        PreviousViewTarget = PC->GetViewTarget();

        // 固定カメラへ切り替える
        PC->SetViewTargetWithBlend(this, 0.5f);

        // プレイヤーの移動・視点操作を無効化
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);

        // プレイヤーの入力を停止
        if (PlayerPawn)
        {
            PlayerPawn->DisableInput(PC);
        }

        // 既存の案内を消す
        HidePrompt();

        // マウスカーソルを表示してUIを操作可能にする
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);

        // 操作ボタンを表示
        if (CameraControlWidgetClass &&
            !CameraControlWidget)
        {
            // 操作ボタンのウィジェットを生成
            CameraControlWidget =
                CreateWidget<UUserWidget>(
                    PC, CameraControlWidgetClass);

            if (CameraControlWidget)
            {
                // 専用ウィジェットクラスに変換
                UH_CameraControlWidgetBase* ControlWidget =
                    Cast<UH_CameraControlWidgetBase>(CameraControlWidget);

                if (ControlWidget)
                {
                    UE_LOG(LogTemp, Warning,
                        TEXT("ControlWidget Cast SUCCESS"));

                    ControlWidget->SetFixedCameraAreaRef(this);
                }
                else
                {
                    UE_LOG(LogTemp, Error,
                        TEXT("ControlWidget Cast FAILED"));
                }

                // ウィジェットを画面に表示
                CameraControlWidget->AddToViewport();
            }
        }

        bCameraFixed = true;

        // プレビューを生成
        CreateSpawnPreviews();
    }
    else
    {
        RestoreCamera();
    }
}

void AH_FixedCameraArea::RestoreCamera()
{
    // プレビューを削除
    ClearSpawnPreviews();

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    // 操作ボタンを削除
    if (CameraControlWidget)
    {
        CameraControlWidget->RemoveFromParent();
        CameraControlWidget = nullptr;
    }

    if (PC)
    {
        if (PreviousViewTarget)
        {
            PC->SetViewTargetWithBlend(
                PreviousViewTarget, 0.5f);
        }

        // プレイヤー操作を再び有効にする
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);

        // プレイヤーの入力を再び有効にする
        APawn* PlayerPawn =
            UGameplayStatics::GetPlayerPawn(this, 0);

        if (PlayerPawn)
        {
            PlayerPawn->EnableInput(PC);
        }

        // マウスカーソルと入力モードを戻す
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }

    PreviousViewTarget = nullptr;
    bCameraFixed = false;
}

// Fキーの案内を表示
void AH_FixedCameraArea::ShowPrompt()
{
    if (PromptWidget || !PromptWidgetClass)
    {
        return;
    }

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (!PC)
    {
        return;
    }

    PromptWidget = CreateWidget<UUserWidget>(
        PC, PromptWidgetClass);

    if (PromptWidget)
    {
        PromptWidget->AddToViewport();
        bPromptVisible = true;
    }
}

//Fキーの案内を非表示
void AH_FixedCameraArea::HidePrompt()
{
    if (PromptWidget)
    {
        PromptWidget->RemoveFromParent();
        PromptWidget = nullptr;
    }

    bPromptVisible = false;
}

void AH_FixedCameraArea::SpawnObjectByIndex(int32 Index)
{
    UE_LOG(LogTemp, Warning,
        TEXT("SpawnObjectByIndex: Index=%d, AlreadyUsed=%d"),
        Index,
        SpawnedOptionIndices.Contains(Index) ? 1 : 0);

    // 配列の範囲外なら終了
    if (!SpawnOptions.IsValidIndex(Index))
    {
        return;
    }

    // このボタンですでに生成していたら終了
    if (SpawnedOptionIndices.Contains(Index))
    {
        return;
    }

    const FCameraSpawnOption& Option = SpawnOptions[Index];

    if (!Option.ObjectClass)
    {
        return;
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    // 生成前に使用済みとして登録
    SpawnedOptionIndices.Add(Index);

    AActor* SpawnedObject = World->SpawnActor<AActor>(
        Option.ObjectClass,
        Option.SpawnLocation,
        Option.SpawnRotation
    );

    // 生成に失敗した場合は、もう一度押せるようにする
    if (!SpawnedObject)
    {
        SpawnedOptionIndices.Remove(Index);
    }
    else
    {
        // 押したボタンに対応するプレビューだけ削除
        if (PreviewActors.Contains(Index))
        {
            if (IsValid(PreviewActors[Index]))
            {
                PreviewActors[Index]->Destroy();
            }

            PreviewActors.Remove(Index);
        }

        UE_LOG(LogTemp, Warning,
            TEXT("Preview Removed: Index=%d"),
            Index);

        // すべてのプレビューがなくなった場合のみタイマー停止
        if (PreviewActors.Num() == 0)
        {
            GetWorldTimerManager().ClearTimer(
                PreviewBlinkTimer
            );
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Spawn Index=%d, Location=%s, Class=%s"),
        Index,
        *Option.SpawnLocation.ToString(),
        *GetNameSafe(Option.ObjectClass.Get()));

    // スポーン後にボタンの表示を更新
    if (CameraControlWidget)
    {
        UH_CameraControlWidgetBase* ControlWidget =
            Cast<UH_CameraControlWidgetBase>(CameraControlWidget);

        if (ControlWidget)
        {
            ControlWidget->RefreshSpawnButtonVisibility();
        }
    }
}

bool AH_FixedCameraArea::IsSpawnOptionUsed(int32 Index) const
{
    const bool bUsed = SpawnedOptionIndices.Contains(Index);

    UE_LOG(LogTemp, Warning,
        TEXT("IsSpawnOptionUsed: Index=%d, Used=%d"),
        Index,
        bUsed ? 1 : 0);

    return bUsed;
}

// スポーン設定があるか確認
bool AH_FixedCameraArea::HasSpawnOption(int32 Index) const
{
    if (!SpawnOptions.IsValidIndex(Index))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Index %d : Invalid"), Index);

        return false;
    }

    bool bHasClass =
        SpawnOptions[Index].ObjectClass != nullptr;

    UE_LOG(LogTemp, Warning,
        TEXT("Index %d : HasClass = %d, Class = %s"),
        Index,
        bHasClass ? 1 : 0,
        *GetNameSafe(SpawnOptions[Index].ObjectClass.Get()));

    return bHasClass;
}

// プレビュー生成
void AH_FixedCameraArea::CreateSpawnPreviews()
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    // 以前のプレビューを削除
    ClearSpawnPreviews();

    for (int32 i = 0; i < SpawnOptions.Num(); i++)
    {
        // すでにスポーン済みなら無視
        if (SpawnedOptionIndices.Contains(i))
        {
            continue;
        }

        const FCameraSpawnOption& Option = SpawnOptions[i];

        if (!Option.ObjectClass)
        {
            continue;
        }

        FActorSpawnParameters Params;

        // 他のオブジェクトと重なっていても生成する
        Params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        // オブジェクトを生成
        AActor* PreviewActor = World->SpawnActor<AActor>(
            Option.ObjectClass,
            Option.SpawnLocation,
            Option.SpawnRotation,
            Params
        );

        if (!PreviewActor)
        {
            continue;
        }

        // プレビューはゲーム処理に影響させない
        PreviewActor->SetActorEnableCollision(false);
        PreviewActor->SetActorTickEnabled(false);

        TArray<UPrimitiveComponent*> Components;

        PreviewActor->GetComponents<UPrimitiveComponent>(Components);

        for (UPrimitiveComponent* Primitive : Components)
        {
            if (Primitive)
            {
                Primitive->SetCollisionEnabled(
                    ECollisionEnabled::NoCollision);

                Primitive->SetGenerateOverlapEvents(false);
                Primitive->SetComponentTickEnabled(false);
            }
        }

        // 半透明化を試みる
        ApplyPreviewOpacity(PreviewActor);

        // 配列番号とプレビューを紐付ける
        PreviewActors.Add(i, PreviewActor);
    }

    // 点滅開始
    bPreviewVisible = true;

    GetWorldTimerManager().SetTimer(
        PreviewBlinkTimer,
        this,
        &AH_FixedCameraArea::UpdatePreviewVisibility,
        0.5f,
        true
    );
}

//半透明処理
void AH_FixedCameraArea::ApplyPreviewOpacity(
    AActor* PreviewActor)
{
    if (!PreviewActor)
    {
        return;
    }

    // PrimitiveComponentを取得
    TArray<UPrimitiveComponent*> Components;

    PreviewActor->GetComponents<UPrimitiveComponent>(Components);

    for (UPrimitiveComponent* Primitive : Components)
    {
        if (!Primitive)
        {
            continue;
        }

        int32 MaterialCount = Primitive->GetNumMaterials();

        for (int32 i = 0; i < MaterialCount; i++)
        {
            UMaterialInstanceDynamic* DynamicMaterial =
                Primitive->CreateAndSetMaterialInstanceDynamic(i);

            if (DynamicMaterial)
            {
                DynamicMaterial->SetScalarParameterValue(
                    FName("Opacity"),
                    0.3f
                );
            }
        }
    }
}

//点滅処理
void AH_FixedCameraArea::UpdatePreviewVisibility()
{
    // プレビューが残っていなければタイマー停止
    if (PreviewActors.Num() == 0)
    {
        GetWorldTimerManager().ClearTimer(
            PreviewBlinkTimer
        );
        return;
    }

    bPreviewVisible = !bPreviewVisible;

    for (auto It = PreviewActors.CreateIterator(); It; ++It)
    {
        AActor* PreviewActor = It.Value();

        // 無効なActorを削除
        if (!IsValid(PreviewActor))
        {
            It.RemoveCurrent();
            continue;
        }

        // 有効なプレビューだけ点滅
        PreviewActor->SetActorHiddenInGame(
            !bPreviewVisible
        );
    }

    // 全部なくなったら停止
    if (PreviewActors.Num() == 0)
    {
        GetWorldTimerManager().ClearTimer(
            PreviewBlinkTimer
        );
    }
}

//プレビュー削除処理
void AH_FixedCameraArea::ClearSpawnPreviews()
{
    // タイマー停止
    GetWorldTimerManager().ClearTimer(
        PreviewBlinkTimer
    );

    // プレビューを削除
    for (auto& Pair : PreviewActors)
    {
        if (IsValid(Pair.Value))
        {
            Pair.Value->Destroy();
        }
    }

    PreviewActors.Empty();
}