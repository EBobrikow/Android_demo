// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "BaseGlobalWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANDROID_DEMO_API UBaseGlobalWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* OpenImageBtn;

	UFUNCTION(BlueprintCallable)
	void OnOpenBtnClicked();

	UFUNCTION()
	void OnImagePickerFinished(FString path);
	
private:

	FString GetRootPath();

};
