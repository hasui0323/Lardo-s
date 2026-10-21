// Fill out your copyright notice in the Description page of Project Settings.


#include "FItem.h"
#include "FCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AFItem::AFItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // 見た目
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("ItemMesh")
    );
    SetRootComponent(ItemMesh);

    // 当たり判定
    Collision = CreateDefaultSubobject<USphereComponent>(
        TEXT("Collision")
    );
    Collision->SetupAttachment(ItemMesh);

    Collision->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly
    );
    Collision->SetCollisionResponseToAllChannels(
        ECR_Overlap
    );
    Collision->SetGenerateOverlapEvents(true);

    // 接触時の処理を登録
    Collision->OnComponentBeginOverlap.AddDynamic(
        this, &AFItem::OnOverlapBegin
    );
}

void AFItem::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    // 接触した相手がプレイヤーか確認
    AFCharacter* Player = Cast<AFCharacter>(OtherActor);

    if (Player)
    {

            ////ゲージを20%増やす
            //Player->AddGauge(0.2f);

        // アイテムの種類によって処理を分ける
        switch (ItemType)
        {
        case EItemType::Gauge:
            //ゲージを20%増やす
            Player->AddGauge(0.2f);
            break;

        case EItemType::Life:
            //残機を1増やす
            Player->AddLife(1);
            break;
        }

        //アイテムを消す
        Destroy();
    }
}

//{
//    UE_LOG(LogTemp, Warning, TEXT("Item Overlap!"));
//
//    if (OtherActor)
//    {
//        UE_LOG(
//            LogTemp,
//            Warning,
//            TEXT("Touched Actor: %s"),
//            *OtherActor->GetName()
//        );
//
//        if (OtherActor != this)
//        {
//            Destroy();
//        }
//    }
//}