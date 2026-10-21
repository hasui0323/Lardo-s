// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "H_FixedCameraArea.generated.h"

class UBoxComponent;
class UCameraComponent;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FCameraSpawnOption
{
    GENERATED_BODY()

    // 出現させるオブジェクト
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ObjectClass;

    // 出現位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector SpawnLocation = FVector::ZeroVector;

    // 出現時の向き
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator SpawnRotation = FRotator::ZeroRotator;
};

UCLASS()
class MERUOZI_API AH_FixedCameraArea : public AActor
{
    GENERATED_BODY()

public:
    AH_FixedCameraArea();

protected:
    virtual void BeginPlay() override;

    // プレイヤーが入る範囲
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UBoxComponent* Trigger;

    // 固定視点用カメラ
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* FixedCamera;

    // 光の柱
    UPROPERTY(VisibleAnywhere, Category = "Effect")
    UNiagaraComponent* LightPillar;

    // 光の柱のエフェクト
    UPROPERTY(EditAnywhere, Category = "Effect")
    UNiagaraSystem* LightPillarEffect;

    // カメラ切り替え後に表示する操作ウィジェット
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> CameraControlWidgetClass;

    UPROPERTY()
    UUserWidget* CameraControlWidget = nullptr;

    // ボタンごとの出現設定
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<FCameraSpawnOption> SpawnOptions;

    // ボタンから呼び出す出現処理
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void SpawnObjectByIndex(int32 Index);

    // 指定したボタンが使用済みか確認する
    UFUNCTION(BlueprintPure, Category = "Spawn")
    bool IsSpawnOptionUsed(int32 Index) const;

    // 範囲内にいるか
    bool bPlayerInRange = false;

    // カメラ固定中か
    bool bCameraFixed = false;

    // 入力登録済みか
    bool bInputBound = false;

    // 切り替え前の視点
    AActor* PreviousViewTarget = nullptr;

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    void SwitchCamera();
    void RestoreCamera();

    // 視点変更の案内ウィジェット
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> PromptWidgetClass;

    // 作成したウィジェットの実体
    UPROPERTY()
    UUserWidget* PromptWidget;

    // 表示中かどうか
    bool bPromptVisible = false;

    // 表示・非表示の処理
    void ShowPrompt();
    void HidePrompt();

    // 生成済みのボタン番号
    TSet<int32> SpawnedOptionIndices;
};
