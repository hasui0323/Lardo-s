// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "M_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MERUOZI_API AM_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AM_GameMode();

	// 毎フレームの監視処理を有効にする
	virtual void Tick(float DeltaTime) override;

protected:
	// ゲーム開始時に呼ばれる
	virtual void BeginPlay() override;

private:
	// 現在のレベルで設定されているKillZの値
	float CurrentKillZ;
	
};
