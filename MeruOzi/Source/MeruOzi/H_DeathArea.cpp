
#include "H_DeathArea.h"

#include "FCharacter.h"
#include "CheckpointGameInstance.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// コンストラクタ
AH_DeathArea::AH_DeathArea()
{
    PrimaryActorTick.bCanEverTick = false;

    // ダメージ判定
    DamageBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("DamageBox")
    );

    RootComponent = DamageBox;

    DamageBox->SetBoxExtent(
        FVector(200.0f, 200.0f, 50.0f)
    );

    DamageBox->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly
    );

    DamageBox->SetCollisionResponseToAllChannels(
        ECR_Ignore
    );

    DamageBox->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap
    );

    DamageBox->SetGenerateOverlapEvents(true);
}

// ゲーム開始時
void AH_DeathArea::BeginPlay()
{
    Super::BeginPlay();

    // プレイヤーの初期位置を保存
    ACharacter* Player =
        UGameplayStatics::GetPlayerCharacter(this, 0);

    if (Player)
    {
        PlayerStartLocation = Player->GetActorLocation();
        PlayerStartRotation = Player->GetActorRotation();
    }

    // 接触判定を登録
    DamageBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AH_DeathArea::OnDamageOverlap
    );
}

// プレイヤーが触れたとき
void AH_DeathArea::OnDamageOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    // 重複処理防止
    if (bIsProcessing)
    {
        return;
    }

    // プレイヤーを取得
    AFCharacter* Player = Cast<AFCharacter>(OtherActor);

    if (!Player || Player->bIsDead)
    {
        return;
    }

    bIsProcessing = true;

    // 残機を減らす
    Player->DecreaseLife();

    // ゲームオーバーなら終了
    if (Player->bIsDead)
    {
        return;
    }

    // 初期位置をリスポーン先にする
    FVector RespawnLocation = PlayerStartLocation;
    FRotator RespawnRotation = PlayerStartRotation;

    // チェックポイント情報を取得
    UCheckpointGameInstance* GI =
        Cast<UCheckpointGameInstance>(
            UGameplayStatics::GetGameInstance(this)
        );

    if (GI && GI->bHasCheckpoint)
    {
        // チェックポイントを優先
        RespawnLocation = GI->SavedCheckpointLocation;
        RespawnRotation = GI->SavedCheckpointRotation;
    }

    // 移動を停止
    Player->GetCharacterMovement()->StopMovementImmediately();

    // リスポーン
    Player->SetActorLocation(
        RespawnLocation,
        false,
        nullptr,
        ETeleportType::TeleportPhysics
    );

    Player->SetActorRotation(RespawnRotation);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("マップ外に落下！残機: %d"),
        Player->Life
    );

    bIsProcessing = false;
}
