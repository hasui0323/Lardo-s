
#include "H_GoalActor.h"

#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "FCharacter.h"

AH_GoalActor::AH_GoalActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 150.0f));

    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AH_GoalActor::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AH_GoalActor::OnGoalOverlap
    );
}

void AH_GoalActor::OnGoalOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bIsGoal)
    {
        return;
    }

    AFCharacter* Player = Cast<AFCharacter>(OtherActor);

    if (Player)
    {
        StartGoal();
    }
}

void AH_GoalActor::StartGoal()
{
    bIsGoal = true;

    // 接触判定を無効化
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // GOALウィジェットを表示
    if (GoalWidgetClass)
    {
        UUserWidget* GoalWidget =
            CreateWidget<UUserWidget>(GetWorld(), GoalWidgetClass);

        if (GoalWidget)
        {
            GoalWidget->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GOAL Widget Create Failed"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GoalWidgetClass is NULL"));
    }

    // プレイヤー操作を停止
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    AFCharacter* Player =
        Cast<AFCharacter>(
            UGameplayStatics::GetPlayerCharacter(this, 0)
        );

    if (PC && Player)
    {
        Player->DisableInput(PC);
    }
}