// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleActor.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"

ATitleActor::ATitleActor()
{
    //毎フレーム処理を実行する
    PrimaryActorTick.bCanEverTick = true;
}

void ATitleActor::BeginPlay()
{
    Super::BeginPlay();

    ////プレイヤーコントローラーを取得
    //APlayerController* PlayerController =
    //    UGameplayStatics::GetPlayerController(this, 0);

    //if (PlayerController)
    //{
    //    //マウスカーソルを非表示する
    //    PlayerController->bShowMouseCursor = false;

    //    //ゲーム画面の入力を受け取れるようにする
    //    FInputModeGameAndUI InputMode;
    //    PlayerController->SetInputMode(InputMode);

    //    //左クリックを検知できるようにする
    //    EnableInput(PlayerController);
    //}
}

void ATitleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //プレイヤーコントローラーを取得する
    APlayerController* PlayerController =
        UGameplayStatics::GetPlayerController(this, 0);

    //プレイヤーコントローラーが存在するか確認する
    if (PlayerController)
    {
        //左クリックされた瞬間か確認する
        if (PlayerController->WasInputKeyJustPressed(
            EKeys::LeftMouseButton))
        {
            //次のレベルへ移動する
            OnLeftClick();
        }
    }

    //ESCキーが押された瞬間か確認する
    if (PlayerController->WasInputKeyJustPressed(
        EKeys::Escape))
    {
        //ゲームを終了する
        UKismetSystemLibrary::QuitGame(
            this,                   
            PlayerController,       
            EQuitPreference::Quit,   //ゲームを終了する
            false                 
        );
    }
}

void ATitleActor::OnLeftClick()
{
    //指定したレベルを開く
    UGameplayStatics::OpenLevel(this, NextLevelName);
}
