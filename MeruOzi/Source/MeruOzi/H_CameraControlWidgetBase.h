// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "H_CameraControlWidgetBase.generated.h"

// 固定カメラエリアの前方宣言
class AH_FixedCameraArea;

/**
 * 
 */
UCLASS()
class MERUOZI_API UH_CameraControlWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

    // 固定カメラエリアの参照
    UPROPERTY(BlueprintReadOnly, Category = "Camera")
    AH_FixedCameraArea* FixedCameraAreaRef = nullptr;

    // 固定カメラエリアの参照を設定
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetFixedCameraAreaRef(AH_FixedCameraArea* Area);
};
