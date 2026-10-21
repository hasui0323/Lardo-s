#include "H_FlyingSpike.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CheckpointGameInstance.h"

AH_FlyingSpike::AH_FlyingSpike()
{
    PrimaryActorTick.bCanEverTick = true;

    //メッシュコンポーネントを作成
    SpikeMeshComponent =
        CreateDefaultSubobject<UStaticMeshComponent>(
            TEXT("SpikeMeshComponent")
        );

    //メッシュをActorのルートに設定する
    SetRootComponent(SpikeMeshComponent);

    //衝突判定を有効にする
    SpikeMeshComponent->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics
    );

    //動的オブジェクトと衝突する設定にする
    SpikeMeshComponent->SetCollisionProfileName(
        TEXT("BlockAllDynamic")
    );

    //初期状態では、とげのメッシュを未定にする
    SpikeMesh = nullptr;
}

void AH_FlyingSpike::BeginPlay()
{
    Super::BeginPlay();

    // 飛んでいるとげは発射処理を行わない
    if (!bIsProjectile)
    {
        APawn* PlayerPawn =
            UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        if (PlayerPawn)
        {
            InitialPlayerLocation = PlayerPawn->GetActorLocation();
            InitialPlayerRotation = PlayerPawn->GetActorRotation();
            bInitialPlayerLocationSaved = true;
        }
        //最初のとげを発射するタイマーを設定
        ScheduleNextShot();
    }
}

void AH_FlyingSpike::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 発射装置自身は移動させない
    if (!bIsProjectile)
    {
        return;
    }

    //飛んでいる時間を計測
    ElapsedTime += DeltaTime;

    //寿命に達したら、とげを削除する
    if (ElapsedTime >= SpikeLifeTime)
    {
        Destroy();
        return;
    }

    // とげを直線的に移動させる
    FHitResult Hit;

    //指定した方向へ、とげを直線的に移動させる
    //Sweepを有効にして、移動中の衝突判定を検出する
    AddActorWorldOffset(
        ProjectileDirection * SpikeSpeed * DeltaTime,
        true,
        &Hit
    );

    // 壁や床などに当たったら消滅
    if (Hit.bBlockingHit)
    {
        // 衝突した相手がプレイヤーか確認
        APawn* HitPawn = Cast<APawn>(Hit.GetActor());
        APawn* PlayerPawn =
            UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        if (HitPawn && HitPawn == PlayerPawn)
        {
            HandlePlayerHit();
        }

        //とげは衝突したら消す
        Destroy();
    }
}

//飛翔するとげの初期設定を行う
void AH_FlyingSpike::InitializeProjectile(
    UStaticMesh* InMesh,
    FVector InDirection,
    float InSpeed,
    float InLifeTime,
    FRotator InRotationOffset
)
{
    //このActorを飛翔用として設定する
    bIsProjectile = true;

    //飛ばすメッシュを設定する
    SpikeMeshComponent->SetStaticMesh(InMesh);

    //衝突判定を有効にする
    SpikeMeshComponent->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics
    );

    //動的オブジェクトと衝突する設定にする
    SpikeMeshComponent->SetCollisionProfileName(
        TEXT("BlockAllDynamic")
    );

    //発射方向を正規化する
    ProjectileDirection = InDirection.GetSafeNormal();

    //法王が未定ならX軸方向の正方向を使用する
    if (ProjectileDirection.IsNearlyZero())
    {
        ProjectileDirection = FVector::ForwardVector;
    }

    //速度と飛ぶ時間を設定する
    SpikeSpeed = InSpeed;
    SpikeLifeTime = InLifeTime;
    //経過時間をリセットする
    ElapsedTime = 0.0f;

    // Actorを飛ぶ方向に向ける
    SetActorRotation(ProjectileDirection.Rotation());

    // メッシュの向きだけ補正する
    SpikeMeshComponent->SetRelativeRotation(
        InRotationOffset
    );
}

//次の発車時間をランダムに決定する
void AH_FlyingSpike::ScheduleNextShot()
{
    //ワールドが存在しない倍は処理を終了する
    if (!GetWorld())
    {
        return;
    }

    //最大値と最小値を整理する
    const float MinInterval = FMath::Min(
        MinFireInterval,
        MaxFireInterval
    );

    const float MaxInterval = FMath::Max(
        MinFireInterval,
        MaxFireInterval
    );

    //ランダムな時間が経過したら発射処理を1買い実行する
    const float RandomInterval = FMath::FRandRange(
        MinInterval,
        MaxInterval
    );

    GetWorld()->GetTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AH_FlyingSpike::FireSpike,
        RandomInterval,
        false
    );
}

//とげを発射する
void AH_FlyingSpike::FireSpike()
{
    //ワールドが存在しない場合は処理を終了する
    if (!GetWorld())
    {
        return;
    }

    //とげのメッシュが設定されていない場合は発射しない
    if (!SpikeMesh)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("H_FlyingSpike: SpikeMesh is not set")
        );

        //メッシュ未定でも次の発射を予約する
        ScheduleNextShot();
        return;
    }

    //発射方向を正規化する
    FVector Direction = LaunchDirection.GetSafeNormal();

    //方向が未定ならActorの前方向を使用する
    if (Direction.IsNearlyZero())
    {
        Direction = GetActorForwardVector();
    }

    // 発射位置を計算
    const FVector SpawnLocation =
        GetActorLocation()
        + GetActorRotation().RotateVector(SpawnOffset);

    //とげを飛ぶ方向に向ける
    const FRotator SpawnRotation = Direction.Rotation();

    //生成するActorの位置と回転を設定する
    const FTransform SpawnTransform(
        SpawnRotation,
        SpawnLocation,
        FVector::OneVector
    );

    // 同じクラスのActorを、とげとして生成する
    AH_FlyingSpike* NewSpike =
        GetWorld()->SpawnActorDeferred<AH_FlyingSpike>(
            GetClass(),
            SpawnTransform,
            this,
            nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

    //Actorがっ正常に生成された場合
    if (NewSpike)
    {
        // BeginPlayより前に飛翔用として設定する
        NewSpike->InitializeProjectile(
            SpikeMesh,
            Direction,
            SpikeSpeed,
            SpikeLifeTime,
            SpikeRotationOffset
        );

        //初期設定を完了してActorを有効にする
        NewSpike->FinishSpawning(SpawnTransform);
    }

    // 次の発射を予約する
    ScheduleNextShot();
}

void AH_FlyingSpike::HandlePlayerHit()
{
    APawn* PlayerPawn =
        UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (!PlayerPawn)
    {
        return;
    }

    // 初期地点を戻り先にする
    FVector RespawnLocation = InitialPlayerLocation;
    FRotator RespawnRotation = InitialPlayerRotation;

    // チェックポイント情報を取得
    UCheckpointGameInstance* GameInstance =
        Cast<UCheckpointGameInstance>(
            UGameplayStatics::GetGameInstance(this)
        );

    // チェックポイントを通過済みなら、その位置と回転を使う
    if (GameInstance && GameInstance->bHasCheckpoint)
    {
        RespawnLocation = GameInstance->SavedCheckpointLocation;
        RespawnRotation = GameInstance->SavedCheckpointRotation;
    }

    // 移動を停止してプレイヤーを戻す
    if (PlayerPawn->GetMovementComponent())
    {
        PlayerPawn->GetMovementComponent()->StopMovementImmediately();
    }

    PlayerPawn->SetActorLocation(
        RespawnLocation,
        false,
        nullptr,
        ETeleportType::TeleportPhysics
    );

    PlayerPawn->SetActorRotation(RespawnRotation);
}