// Fill out your copyright notice in the Description page of Project Settings.


#include "H_CameraControlWidgetBase.h"
#include "H_FixedCameraArea.h"

void UH_CameraControlWidgetBase::SetFixedCameraAreaRef(
    AH_FixedCameraArea* Area)
{
    FixedCameraAreaRef = Area;
}