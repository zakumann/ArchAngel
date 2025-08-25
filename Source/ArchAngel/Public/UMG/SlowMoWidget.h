// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlowMoWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class ARCHANGEL_API USlowMoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // ProgressBar reference (bind it in UMG)
    UPROPERTY(meta = (BindWidget))
    UProgressBar* Progress_SlowMo;

    UFUNCTION(BlueprintCallable, Category = "SlowMo")
    void UpdateSlowMoBar(float Percent);
};
