// Fill out your copyright notice in the Description page of Project Settings.


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

        // プレイヤーの操作を無効にする
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);

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

    if (PC && PreviousViewTarget)
    {
        PC->SetViewTargetWithBlend(
            PreviousViewTarget, 0.5f);

        // プレイヤーの操作を再び有効にする
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
    }

    PreviousViewTarget = nullptr;
    bCameraFixed = false;
}
