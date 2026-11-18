// Fill out your copyright notice in the Description page of Project Settings.


#include "FBossActor.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"
#include "FCharacter.h"

#include "Components/CapsuleComponent.h"

#include "FBossAnimInstance.h"

// Sets default values
AFBossActor::AFBossActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // ボス側で入力を受け取る
    AutoReceiveInput = EAutoReceiveInput::Player0;

    // ボスのモデル
    BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));
    RootComponent = BossMesh;

    //--------------------------------------------------------
    // ボス本体の当たり判定
    BossCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BossCollision"));
    BossCollision->SetupAttachment(RootComponent);

    // 当たり判定の大きさ
    BossCollision->SetCapsuleSize(50.0f, 100.0f);

    // プレイヤーと衝突させる
    BossCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BossCollision->SetCollisionObjectType(ECC_WorldStatic);
    BossCollision->SetCollisionResponseToAllChannels(ECR_Block);
    BossCollision->SetGenerateOverlapEvents(false);

    //--------------------------------------------------------
    // 接近判定
    AttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
    AttackRange->SetupAttachment(RootComponent);

    AttackRange->SetSphereRadius(200.0f);

    AttackRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    AttackRange->SetCollisionResponseToAllChannels(ECR_Ignore);
    AttackRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    AttackRange->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AFBossActor::BeginPlay()
{
	Super::BeginPlay();
	
    // 入力を有効にする
    EnableInput(GetWorld()->GetFirstPlayerController());

    // Fキーを押したとき
    InputComponent->BindKey(
        EKeys::F,
        IE_Pressed,
        this,
        &AFBossActor::InteractWithBoss
    );

    // 接近・離脱判定
    AttackRange->OnComponentBeginOverlap.AddDynamic(this, &AFBossActor::OnOverlapBegin);
    AttackRange->OnComponentEndOverlap.AddDynamic(this, &AFBossActor::OnOverlapEnd);

}

// プレイヤーが近づいたとき
void AFBossActor::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (Cast<AFCharacter>(OtherActor))
    {
        bCanInteract = true;
    }
}

// プレイヤーが離れたとき
void AFBossActor::OnOverlapEnd(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
)
{
    if (Cast<AFCharacter>(OtherActor))
    {
        bCanInteract = false;
    }
}

// Fキーを押したとき
void AFBossActor::Interact(AActor* Player)
{
    if (!bCanInteract || !AttackRange->IsOverlappingActor(Player))
    {
        return;
    }

    // 回数を増やす
    InteractionCount++;

    // ログに表示
    UE_LOG(LogTemp, Warning, TEXT("Boss: %d / %d"),
        InteractionCount, RequiredInteractions);

    // アニメーションインスタンスを取得
    UFBossAnimInstance* AnimInstance =
        Cast<UFBossAnimInstance>(BossMesh->GetAnimInstance());

    // ダウン状態にする
    if (AnimInstance)
    {
        AnimInstance->bIsDown = true;
    }

    // 3回で撃破
    if (InteractionCount >= RequiredInteractions)
    {
        UE_LOG(LogTemp, Warning, TEXT("Boss Defeated!"));
        Destroy();
    }

}


// Called every frame
void AFBossActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Fキーを押したときの処理
void AFBossActor::InteractWithBoss()
{
    if (bCanInteract)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();

        if (PC)
        {
            Interact(PC->GetPawn());
        }
    }
}

