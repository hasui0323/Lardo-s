// Fill out your copyright notice in the Description page of Project Settings.


#include "M_FallingtoDeath.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AM_FallingtoDeath::AM_FallingtoDeath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("CollisionBox"));

    // ボックスをルートコンポーネントにする
    SetRootComponent(CollisionBox);

    // オーバーラップイベントを有効化
    CollisionBox->SetGenerateOverlapEvents(true);

    // 衝突判定だけを行い、物理的にはブロックしない
    CollisionBox->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);

    // Pawnとのオーバーラップを許可
    CollisionBox->SetCollisionResponseToChannel(
        ECC_Pawn, ECR_Overlap);

    // イベントを登録
    CollisionBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AM_FallingtoDeath::OnOverlapBegin);

	////コンポーネントの初期化
	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

 //   RootComponent = CollisionBox;

	////動的デリゲートのバインド
	//CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AM_FallingtoDeath::OnOverlapBegin);
}

void AM_FallingtoDeath::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning,
        TEXT("FallingToDeath: Overlap detected!"));

    UE_LOG(LogTemp, Warning,
        TEXT("OtherActor: %s"),
        *GetNameSafe(OtherActor));

    if (OtherActor && (OtherActor != this))
    {
        // 他のアクタがCharacterクラスであるか確認
        ACharacter* Victim = Cast<ACharacter>(OtherActor);

        if (Victim)
        {

            // 1. ダメージを与える (Character側のAnyDamageイベント等が発火します)
            UGameplayStatics::ApplyDamage(Victim, DamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());

            // 2. チェックポイントが設定されていればテレポート実行
            if (CheckpointActor)
            {

                FVector TargetLoc = CheckpointActor->GetActorLocation();
                FRotator TargetRot = CheckpointActor->GetActorRotation();

                // TeleportPhysicsを使用することで、落下速度などを「eliminate」し、安全に移動させます
                Victim->SetActorLocationAndRotation(TargetLoc, TargetRot, false, nullptr, ETeleportType::TeleportPhysics);

            }
            else {
                UE_LOG(LogTemp, Error,
                    TEXT("CheckpointActor is not set!"));
            }
        }
    }
}