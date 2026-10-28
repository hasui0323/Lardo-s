// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class MERUOZI_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    //--------------------------------------------------
     // カメラ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;//カメラを後ろに離して設置する

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;//実際に画面を映すカメラ

    // 入力設定
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;//キー操作の組み合わせを管理する

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;//移動入力

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;//マウスによる視点操作

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;//ジャンプ入力

    // 移動処理
    void Move(const FInputActionValue& Value);

    // 視点操作
    void Look(const FInputActionValue& Value);

    // ジャンプ処理
    void StartJump();
    void StopJump();

public:
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;    //--------------------------------------------------

    //--------------------------------------------------

    //--------------------------------------------------
    // アイテム取得ゲージ（0.0～1.0）
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemGauge")
    float GaugePercent = 0.0f;

    // ゲージを増やす関数
    UFUNCTION(BlueprintCallable, Category = "ItemGauge")
    void AddGauge(float Amount);

    // ゲージを消費する。消費できたら true を返す
    UFUNCTION(BlueprintCallable, Category = "ItemGauge")
    bool TryUseGauge(float Amount);

    //--------------------------------------------------

    //--------------------------------------------------
    // 残機（初期値は3）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    int32 Life = 3;

    // 残機を増やす関数
    UFUNCTION(BlueprintCallable, Category = "Player")
    void AddLife(int32 Amount);

    //--------------------------------------------------

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
