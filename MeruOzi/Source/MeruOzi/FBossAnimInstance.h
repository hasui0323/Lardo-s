// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MERUOZI_API UFBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// É_ÉEÉìèÛë‘Ç©Ç«Ç§Ç©
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	bool bIsDown = false;
};
