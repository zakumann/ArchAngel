// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SlowMoWidget.h"
#include "Components/ProgressBar.h"

void USlowMoWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void USlowMoWidget::UpdateSlowMoBar(float Percent)
{
    if (Progress_SlowMo)
    {
        Progress_SlowMo->SetPercent(Percent);
    }
}
