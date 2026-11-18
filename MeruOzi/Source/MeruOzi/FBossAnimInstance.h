// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MERUOZI_API UFBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// ダウン状態かどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	bool bIsDown = false;

	// ダウンアニメーション再生中か
	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	bool bIsPlayingDown = false;

	// アニメーション更新処理
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
