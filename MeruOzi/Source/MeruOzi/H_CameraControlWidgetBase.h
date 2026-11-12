// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "H_CameraControlWidgetBase.generated.h"

class AH_FixedCameraArea;
class UButton;

UCLASS()
class MERUOZI_API UH_CameraControlWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly, Category = "Camera")
    AH_FixedCameraArea* FixedCameraAreaRef = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetFixedCameraAreaRef(AH_FixedCameraArea* Area);

protected:

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI")
    UButton* Button_A;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI")
    UButton* Button_B;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI")
    UButton* Button_C;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI")
    UButton* Button_D;

    void UpdateSpawnButtonVisibility();
};
