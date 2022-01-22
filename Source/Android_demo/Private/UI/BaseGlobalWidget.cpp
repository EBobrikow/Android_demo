// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseGlobalWidget.h"
#include "AndroidGrabber.h"
#include "Kismet/BlueprintPathsLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Actors/PlaneActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


extern FString GExternalFilePath;

void UBaseGlobalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OpenImageBtn)
	{
		OpenImageBtn->OnClicked.AddDynamic(this, &UBaseGlobalWidget::OnOpenBtnClicked);
	}

	ImgTakerManager = nullptr;
	APlayerController* PlayerControl = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerControl)
	{
		PlayerControl->bShowMouseCursor = true;
		PlayerControl->SetInputMode(FInputModeGameAndUI());
	}
	
	
}

void UBaseGlobalWidget::OnOpenBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Open button clicked"));
	
	if (!ImgTakerManager)
	{
		ImgTakerManager = NewObject<UImageTakerManager>(this); 
	}
	
	ImgTakerManager->ReauestImageFromPlatform();

}
