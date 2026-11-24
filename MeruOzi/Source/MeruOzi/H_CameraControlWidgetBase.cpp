
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

void UH_CameraControlWidgetBase::RefreshSpawnButtonVisibility()
{
    UpdateSpawnButtonVisibility();
}

void UH_CameraControlWidgetBase::UpdateSpawnButtonVisibility()
{
    if (!FixedCameraAreaRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("FixedCameraAreaRef is NULL"));
        return;
    }

    // Aボタン
    if (Button_A)
    {
        bool bVisible =
            FixedCameraAreaRef->HasSpawnOption(0) &&
            !FixedCameraAreaRef->IsSpawnOptionUsed(0);

        Button_A->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
    }

    // Bボタン
    if (Button_B)
    {
        bool bVisible =
            FixedCameraAreaRef->HasSpawnOption(1) &&
            !FixedCameraAreaRef->IsSpawnOptionUsed(1);

        Button_B->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
    }

    // Cボタン
    if (Button_C)
    {
        bool bVisible =
            FixedCameraAreaRef->HasSpawnOption(2) &&
            !FixedCameraAreaRef->IsSpawnOptionUsed(2);

        Button_C->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
    }

    // Dボタン
    if (Button_D)
    {
        bool bVisible =
            FixedCameraAreaRef->HasSpawnOption(3) &&
            !FixedCameraAreaRef->IsSpawnOptionUsed(3);

        Button_D->SetVisibility(
            bVisible ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
    }
}
