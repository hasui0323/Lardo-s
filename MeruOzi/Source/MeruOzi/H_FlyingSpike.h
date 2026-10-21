#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "H_FlyingSpike.generated.h"

class UStaticMeshComponent;
class UStaticMesh;

UCLASS()
class MERUOZI_API AH_FlyingSpike : public AActor
{
    GENERATED_BODY()

public:
    // コンストラクタ
    AH_FlyingSpike();

    // 毎フレーム呼び出される処理
    virtual void Tick(float DeltaTime) override;

protected:
    // ゲーム開始時に呼び出される処理
    virtual void BeginPlay() override;

    // 発射装置、または飛翔するとげのメッシュ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spike")
    UStaticMeshComponent* SpikeMeshComponent;

    // 発射時に使用するとげのメッシュ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    UStaticMesh* SpikeMesh;

    // とげの発射方向（ワールド座標）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    FVector LaunchDirection = FVector(1.0f, 0.0f, 0.0f);

    // とげの移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike",
        meta = (ClampMin = "0.0"))
    float SpikeSpeed = 1500.0f;

    // 次の発射までの最短時間（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing",
        meta = (ClampMin = "0.1"))
    float MinFireInterval = 1.0f;

    // 次の発射までの最長時間（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing",
        meta = (ClampMin = "0.1"))
    float MaxFireInterval = 4.0f;

    // とげが消えるまでの時間（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike",
        meta = (ClampMin = "0.1"))
    float SpikeLifeTime = 5.0f;

    // とげの見た目の向きを調整する回転値
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    FRotator SpikeRotationOffset = FRotator::ZeroRotator;

    // 発射装置から発射位置までの相対位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    FVector SpawnOffset = FVector(100.0f, 0.0f, 0.0f);

private:
    // trueなら飛翔中のとげ、falseなら発射装置
    bool bIsProjectile = false;

    // 飛翔中のとげの移動方向
    FVector ProjectileDirection = FVector::ForwardVector;

    // 発射後の経過時間
    float ElapsedTime = 0.0f;

    // 発射タイマー
    FTimerHandle FireTimerHandle;

    // 次の発射時間をランダムに決定する
    void ScheduleNextShot();

    // とげを生成して発射する
    void FireSpike();

    // ゲーム開始時のプレイヤーの位置と回転
    FVector InitialPlayerLocation = FVector::ZeroVector;
    FRotator InitialPlayerRotation = FRotator::ZeroRotator;

    // 初期位置を保存できたか
    bool bInitialPlayerLocationSaved = false;

    // トゲがプレイヤーに当たったときの処理
    void HandlePlayerHit();

public:
    // 生成したとげに初期設定を行う
    void InitializeProjectile(
        UStaticMesh* InMesh,
        FVector InDirection,
        float InSpeed,
        float InLifeTime,
        FRotator InRotationOffset
    );
};