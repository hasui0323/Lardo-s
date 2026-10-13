// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleActor.generated.h"

UCLASS()
class MERUOZI_API ATitleActor : public AActor
{
    GENERATED_BODY()

public:
    //コンストラクタ
    ATitleActor();

protected:
    //ゲーム開始時に呼び出される
    virtual void BeginPlay() override;

public:
    //毎フレーム呼び出される
    virtual void Tick(float DeltaTime) override;

    //左クリックされたときに呼び出す
    void OnLeftClick();

    //移動先のレベル名を設定する変数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title")
    FName NextLevelName = TEXT("Operation");

};
