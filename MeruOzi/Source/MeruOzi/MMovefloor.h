// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"GameFramework/RotatingMovementComponent.h"
#include"Kismet/kismetSystemLibrary.h"
#include "MMovefloor.generated.h"

UCLASS()
class MERUOZI_API AMMovefloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMovefloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//スタティックメッシュ用の変数
	UPROPERTY(EditAnywhere,Category = "Mesh", BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	//RotatingMovementComponent用の変数
	UPROPERTY(EditAnywhere, Category = "RotationMovement", BlueprintReadWrite)
	URotatingMovementComponent* Rotating;

	//回転する各度量の変数
	UPROPERTY(EditAnywhere,Category = "RotationValue",BlueprintReadWrite)
	FRotator MeshRotationValue;


};
