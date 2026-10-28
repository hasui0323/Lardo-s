// Fill out your copyright notice in the Description page of Project Settings.


#include "MSMovefloor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"


// Sets default values
AMSMovefloor::AMSMovefloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false; // プレイヤーが乗るまでは Tick しない

    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    PlatformMesh->SetMobility(EComponentMobility::Movable);
    RootComponent = PlatformMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 20.f));
    TriggerBox->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AMSMovefloor::BeginPlay()
{
	Super::BeginPlay();
	
    StartLocation = GetActorLocation();

    // 方向を正規化 (長さ 1 にする)
    const FVector LocalDirection = MoveDirection.GetSafeNormal();
    if (LocalDirection.IsNearlyZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: MoveDirection is zero. Platform will not move."), *GetName());
    }

    // ローカル方向をワールド方向に変換 (スケールは無視)
    const FVector WorldDirection = GetActorTransform().TransformVectorNoScale(LocalDirection);

    EndLocation = StartLocation + WorldDirection * MoveDistance;

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMSMovefloor::OnTriggerBeginOverlap);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMSMovefloor::OnTriggerEndOverlap);
}

bool AMSMovefloor::IsPlayerPawn(const AActor* Actor)
{
    const APawn* Pawn = Cast<APawn>(Actor);
    return Pawn && Pawn->IsPlayerControlled();
}

bool AMSMovefloor::IsAnyPlayerOnPlatform(const AActor* IgnoreActor) const
{
    TArray<AActor*> Overlapping;
    TriggerBox->GetOverlappingActors(Overlapping, APawn::StaticClass());

    for (const AActor* Actor : Overlapping)
    {
        if (Actor != IgnoreActor && IsPlayerPawn(Actor))
        {
            return true;
        }
    }
    return false;
}

void AMSMovefloor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
    if (!IsPlayerPawn(OtherActor))
    {
        return;
    }

    bPlayerOnBoard = true;

    // 戻っている途中で再び乗ったら、移動先へ向かい直す
    if (bReturning)
    {
        bReturning = false;
        bMovingToEnd = true;
        WaitTimer = 0.f;
    }

    SetActorTickEnabled(true);
}

void AMSMovefloor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!IsPlayerPawn(OtherActor))
    {
        return;
    }

    // ほかのプレイヤーがまだ乗っていれば何もしない
    if (IsAnyPlayerOnPlatform(OtherActor))
    {
        return;
    }

    bPlayerOnBoard = false;
    ReturnDelayTimer = ReturnDelay;
    SetActorTickEnabled(true); // 片道モードで停止中でも、戻る処理のために Tick を再開
}

void AMSMovefloor::StartReturning()
{
    bReturning = true;
    WaitTimer = 0.f; // 端で待機中でもすぐ戻り始める
}

void AMSMovefloor::StopAtStart()
{
    SetActorLocation(StartLocation);
    bReturning = false;
    bMovingToEnd = true;
    WaitTimer = 0.f;
    SetActorTickEnabled(false);
}

// Called every frame
void AMSMovefloor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 降りた後の猶予をカウントダウン
    if (!bPlayerOnBoard && !bReturning)
    {
        ReturnDelayTimer -= DeltaTime;
        if (ReturnDelayTimer <= 0.f)
        {
            StartReturning();
        }
    }

    if (WaitTimer > 0.f)
    {
        WaitTimer -= DeltaTime;
        return;
    }

    const FVector Goal = (bReturning || !bMovingToEnd) ? StartLocation : EndLocation;
    const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), Goal, DeltaTime, Speed);
    SetActorLocation(NewLocation);

    if (!NewLocation.Equals(Goal, 0.1f))
    {
        return;
    }

    // 開始位置に戻りきったら停止
    if (bReturning)
    {
        StopAtStart();
        return;
    }

    if (bPingPong)
    {
        bMovingToEnd = !bMovingToEnd;
        WaitTimer = WaitTime;
    }
    else if (bMovingToEnd && bPlayerOnBoard)
    {
        // 片道モード: 移動先に着いたら、降りるまでその場で停止
        SetActorTickEnabled(false);
    }
}

