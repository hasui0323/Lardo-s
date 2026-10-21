// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/Engine.h"

#include "H_FixedCameraArea.h"

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

#include "Blueprint/UserWidget.h"

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
            CameraControlWidget =
                CreateWidget<UUserWidget>(
                    PC, CameraControlWidgetClass);

            if (CameraControlWidget)
            {
                CameraControlWidget->AddToViewport();
            }
        }

        bCameraFixed = true;
    }
    else
    {
        RestoreCamera();
    }
}

void AH_FixedCameraArea::RestoreCamera()
{
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

        // マウスカーソルと入力モードを戻す
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());

        // キャラクターの移動を再び有効にする
        APawn* PlayerPawn =
            UGameplayStatics::GetPlayerPawn(this, 0);

        ACharacter* PlayerCharacter =
            Cast<ACharacter>(PlayerPawn);

        if (PlayerCharacter)
        {
            UCharacterMovementComponent* Movement =
                PlayerCharacter->GetCharacterMovement();

            if (Movement &&
                Movement->MovementMode == MOVE_None)
            {
                Movement->SetMovementMode(MOVE_Walking);
            }
        }
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
    UE_LOG(LogTemp, Warning,
        TEXT("Spawn Index=%d, Location=%s, Class=%s"),
        Index,
        *Option.SpawnLocation.ToString(),
        *GetNameSafe(Option.ObjectClass.Get()));
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