
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_GoalActor.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class MERUOZI_API AH_GoalActor : public AActor
{
    GENERATED_BODY()

public:
    AH_GoalActor();

protected:
    virtual void BeginPlay() override;

    // ê⁄êGîªíË
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    // GOALï\é¶ópWidget
    UPROPERTY(EditAnywhere, Category = "Goal")
    TSubclassOf<UUserWidget> GoalWidgetClass;

    // ìÒèdé¿çsñhé~
    bool bIsGoal = false;

    UFUNCTION()
    void OnGoalOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void StartGoal();
};
