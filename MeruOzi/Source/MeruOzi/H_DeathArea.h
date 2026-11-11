
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_DeathArea.generated.h"

class UBoxComponent;

UCLASS()
class MERUOZI_API AH_DeathArea : public AActor
{
    GENERATED_BODY()

public:
    AH_DeathArea();

protected:
    virtual void BeginPlay() override;

    // ダメージ判定
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UBoxComponent> DamageBox;

    // ステージ開始地点
    FVector PlayerStartLocation;
    FRotator PlayerStartRotation;

    // 処理中の重複防止
    bool bIsProcessing = false;

    // 接触時の処理
    UFUNCTION()
    void OnDamageOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
