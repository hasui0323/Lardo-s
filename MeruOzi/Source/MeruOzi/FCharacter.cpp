// Fill out your copyright notice in the Description page of Project Settings.


#include "FCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

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
    Jump();
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
}

//プレイヤーの残機
void AFCharacter::AddLife(int32 Amount)
{
    Life += Amount;

    UE_LOG(LogTemp, Warning, TEXT("残機: %d"), Life);
}