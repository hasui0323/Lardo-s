
#include "H_CameraControlWidgetBase.h"
#include "H_FixedCameraArea.h"
#include "Components/Button.h"

void UH_CameraControlWidgetBase::SetFixedCameraAreaRef(
    AH_FixedCameraArea* Area)
{
    FixedCameraAreaRef = Area;

    // 固定カメラエリアの参照を受け取ったら表示を更新
    UpdateSpawnButtonVisibility();
}

void UH_CameraControlWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // ウィジェット表示時にも再確認
    UpdateSpawnButtonVisibility();
}

void UH_CameraControlWidgetBase::UpdateSpawnButtonVisibility()
{
    if (!FixedCameraAreaRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("FixedCameraAreaRef is NULL"));
        return;
    }

    // A
    if (Button_A)
    {
        bool bVisible = FixedCameraAreaRef->HasSpawnOption(0);

        Button_A->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);

        UE_LOG(LogTemp, Warning, TEXT("Button_A Visible: %d"), bVisible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Button_A is NULL"));
    }

    // B
    if (Button_B)
    {
        bool bVisible = FixedCameraAreaRef->HasSpawnOption(1);

        Button_B->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);

        UE_LOG(LogTemp, Warning, TEXT("Button_B Visible: %d"), bVisible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Button_B is NULL"));
    }

    // C
    if (Button_C)
    {
        bool bVisible = FixedCameraAreaRef->HasSpawnOption(2);

        Button_C->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);

        UE_LOG(LogTemp, Warning, TEXT("Button_C Visible: %d"), bVisible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Button_C is NULL"));
    }

    // D
    if (Button_D)
    {
        bool bVisible = FixedCameraAreaRef->HasSpawnOption(3);

        Button_D->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);

        UE_LOG(LogTemp, Warning, TEXT("Button_D Visible: %d"), bVisible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Button_D is NULL"));
    }
}
