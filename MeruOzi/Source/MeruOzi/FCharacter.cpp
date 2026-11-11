// Fill out your copyright notice in the Description page of Project Settings.


#include "FCharacter.h"

#include "Blueprint/UserWidget.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AFCharacter::AFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // カメラアーム
    //キャラクターとカメラの距離を保ってカメラを追従させる
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 350.0f;               //キャラクターとカメラの間の距離を350に設定する
    CameraBoom->bUsePawnControlRotation = true;

    //実際に画面を映すカメラ
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(
        TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(
        CameraBoom, USpringArmComponent::SocketName);     //カメラをカメラアームの先端に取り付ける
    FollowCamera->bUsePawnControlRotation = false;

    // キャラクターの向き
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // 移動方向にキャラクターを向ける
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate =
        FRotator(0.0f, 500.0f, 0.0f);

    // 移動・ジャンプ設定
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;      //最大移動速度
    GetCharacterMovement()->JumpZVelocity = 500.0f;     //ジャンプの初速（大きくすれば高くジャンプする）
    GetCharacterMovement()->AirControl = 0.35f;         //空中で向きを変える操作の強さ
}

// Called when the game starts or when spawned
void AFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    //現在のコントローラーをプレイヤーコントローラーとして取得する
    APlayerController* PC = Cast<APlayerController>(Controller);


    if (PC)
    {
        ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

            // マウスカーソルを非表示にする
            PC->bShowMouseCursor = false;

            // ゲーム操作に入力を戻す
            PC->SetInputMode(FInputModeGameOnly());

        if (LocalPlayer)
        {
            UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<
                UEnhancedInputLocalPlayerSubsystem>();

            if (Subsystem && DefaultMappingContext)
            {
                Subsystem->AddMappingContext(
                    DefaultMappingContext, 0);
            }
        }
    }
}

// Called every frame
void AFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput =
        Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (!EnhancedInput)
    {
        return;
    }

    if (MoveAction)
    {
        EnhancedInput->BindAction(
            MoveAction, ETriggerEvent::Triggered,
            this, &AFCharacter::Move);
    }

    if (LookAction)
    {
        EnhancedInput->BindAction(
            LookAction, ETriggerEvent::Triggered,
            this, &AFCharacter::Look);
    }

    if (JumpAction)
    {
        EnhancedInput->BindAction(
            JumpAction, ETriggerEvent::Started,
            this, &AFCharacter::StartJump);

        EnhancedInput->BindAction(
            JumpAction, ETriggerEvent::Completed,
            this, &AFCharacter::StopJump);

        EnhancedInput->BindAction(
            JumpAction, ETriggerEvent::Canceled,
            this, &AFCharacter::StopJump);
    }
}

//WASDなどの入力を受け取ってキャラクターを移動させる処理
void AFCharacter::Move(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();

    if (Controller)
    {
        FRotator Rotation = Controller->GetControlRotation();
        FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

        FVector Forward =
            FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        FVector Right =
            FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, Input.Y);
        AddMovementInput(Right, Input.X);
    }
}

//マウスなどの入力によって視点を操作する処理
void AFCharacter::Look(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();

    AddControllerYawInput(Input.X);
    AddControllerPitchInput(-Input.Y);
}

//ジャンプを開始する処理
void AFCharacter::StartJump()
{
    // ジャンプできる状態か確認
    if (!CanJump())
    {
        return;
    }

    Jump();

    // ジャンプ音
    PlaySE(JumpSound);
}

//ジャンプを終了する処理
void AFCharacter::StopJump()
{
    StopJumping();
}

//アイテムのゲージ
void AFCharacter::AddGauge(float Amount)
{
    GaugePercent = FMath::Clamp(
        GaugePercent + Amount,
        0.0f,
        1.0f
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Gauge: %f"),
        GaugePercent * 100.0f
    );

    // アイテム入手音
    PlaySE(ItemGetSound);
}
//-----------------------------------------

//プレイヤーの残機を増やす
void AFCharacter::AddLife(int32 Amount)
{
    Life += Amount;

    UE_LOG(LogTemp, Warning, TEXT("残機: %d"), Life);

    // アイテム入手音
    PlaySE(LifeItemGetSound);
}

//プレイヤーの残機を減らす
void AFCharacter::DecreaseLife()
{
    // ダメージ音
    PlaySE(DamageSound);

    // ゲームオーバー後は処理しない
    if (bIsDead)
    {
        return;
    }

    Life = FMath::Max(0, Life - 1);

    UE_LOG(LogTemp, Warning, TEXT("残機: %d"), Life);

    // 残機が0になったらゲームオーバー
    if (Life <= 0)
    {
        GameOver();
    }
}

//-----------------------------------------

// ゲージを消費する処理
bool AFCharacter::TryUseGauge(float Amount)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("消費前のゲージ: %f"),
        GaugePercent
    );

    // ゲージが足りない場合は消費しない
    if (GaugePercent < Amount)
    {
        UE_LOG(LogTemp, Warning, TEXT("ゲージ不足！"));
        return false;
    }

    // ゲージを消費
    GaugePercent -= Amount;

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("消費後のゲージ: %f"),
        GaugePercent
    );

    return true;
}

//------------------------------------------------------------------
// 効果音を再生する
void AFCharacter::PlaySE(USoundBase* Sound)
{
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(this, Sound);
    }
}
//bool AFCharacter::TryUseGauge(float Amount)
//{
//    UE_LOG(
//        LogTemp,
//        Warning,
//        TEXT("TryUseGauge 呼び出し！ 消費前: %f"),
//        GaugePercent
//    );
//
//    if (GaugePercent < Amount)
//    {
//        UE_LOG(
//            LogTemp,
//            Warning,
//            TEXT("ゲージ不足！必要量: %f"),
//            Amount
//        );
//
//        return false;
//    }
//
//    GaugePercent -= Amount;
//
//    UE_LOG(
//        LogTemp,
//        Warning,
//        TEXT("TryUseGauge 成功！ 消費後: %f"),
//        GaugePercent
//    );
//
//    return true;
//}

//ゲームオーバー処理
void AFCharacter::GameOver()
{
    if (bIsDead)
    {
        return;
    }

    bIsDead = true;

    // 移動停止
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();

    // ジャンプ停止
    StopJumping();

    APlayerController* PC =
        Cast<APlayerController>(Controller);

    if (PC)
    {
        // プレイヤー操作停止
        DisableInput(PC);
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);

        // マウスカーソル表示
        PC->bShowMouseCursor = true;

        // ゲームオーバーWidgetを生成
        if (GameOverWidgetClass)
        {
            GameOverWidget = CreateWidget<UUserWidget>(
                PC,
                GameOverWidgetClass
            );

            if (GameOverWidget)
            {
                GameOverWidget->AddToViewport();

                // UI操作に切り替える
                FInputModeUIOnly InputMode;
                PC->SetInputMode(InputMode);
            }
        }
        else
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT("GameOverWidgetClass is not set")
            );
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
}