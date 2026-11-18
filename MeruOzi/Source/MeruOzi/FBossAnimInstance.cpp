// Fill out your copyright notice in the Description page of Project Settings.


#include "FBossAnimInstance.h"

void UFBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // ダウンアニメーションが終了したら
    if (bIsPlayingDown)
    {
        // Downステートの再生が終了したかは
        // ABP側から通知を受け取って判定する
    }
}