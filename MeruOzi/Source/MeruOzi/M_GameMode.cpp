// Fill out your copyright notice in the Description page of Project Settings.


#include "M_GameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"

AM_GameMode::AM_GameMode()
{
	// Tickを有効にする
	PrimaryActorTick.bCanEverTick = true;
	CurrentKillZ = -1000.f; // デフォルト値
}

void AM_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//エディタのWorld Settingsで設定されたKillZを取得
	if (GetWorld() && GetWorld()->GetWorldSettings())
	{
		CurrentKillZ = GetWorld()->GetWorldSettings()->KillZ;
	}
}

void AM_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// すべてのプレイヤーコントローラーをイテレート（巡回）
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		// コントローラーが有効で、かつ操作中のポーンが存在するか確認
		if (PC && PC->GetPawn())
		{
			APawn* Pawn = PC->GetPawn();

			// ポーンのZ座標がKillZを下回っているかチェック
			if (Pawn->GetActorLocation().Z < CurrentKillZ)
			{
				// プレイヤーの排除イベントとしてのログ出力
				UE_LOG(LogTemp, Warning, TEXT("Player %s fell below KillZ. Elimination and Respawn starting."), *PC->GetName());

				// リスポーンを実行
				// RestartPlayerは内部で既存のPawnの破棄と再生成を行ってくれます
				RestartPlayer(PC);
			}
		}
	}
}