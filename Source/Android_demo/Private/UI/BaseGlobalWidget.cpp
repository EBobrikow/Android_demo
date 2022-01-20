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
	/*UE_LOG(LogTemp, Warning, TEXT("Open button clicked"));
	FOnImagePicked locDlg;
	locDlg.AddDynamic(this, &UBaseGlobalWidget::OnImagePickerFinished);
	UAndroidGrabber* AndrGrab = NewObject<UAndroidGrabber>();

	if (AndrGrab)
	{
		AndrGrab->AndroidAPITemplate_GrabImage(locDlg);
	}*/
	if (!ImgTakerManager)
	{
		ImgTakerManager = NewObject<UImageTakerManager>(this); // UImageTakerManager::GetManager(this);
		//ImgTakerManager->AddToRoot();
	}
	
	ImgTakerManager->ReauestImageFromPlatform();

}
//
//void UBaseGlobalWidget::OnImagePickerFinished(FString path)
//{
//	UTexture2D* image;
//	FString filename = path;
//	filename = filename.Replace(TEXT("/raw/"), TEXT(""));
//	FPaths::NormalizeFilename(filename);
//	
//
//	TArray<uint8> binData = TArray<uint8>();
//	FString FinalePath = GetRootPath() + filename;
//	if (FFileHelper::LoadFileToArray(binData, *FinalePath, EFileRead::FILEREAD_AllowWrite)) {
//		image = UKismetRenderingLibrary::ImportBufferAsTexture2D(GetWorld(), binData);
//		image->AddToRoot();
//
//		TArray<AActor*> FoundActors;
//		APlaneActor* PlaneActor = nullptr;
//		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlaneActor::StaticClass(), FoundActors);
//		if (FoundActors.Num() > 0)
//		{
//			PlaneActor = Cast<APlaneActor>(FoundActors[0]);
//		}
//
//		if (PlaneActor)
//		{
//			PlaneActor->ApplyImageOnPlane(image);
//		}
//		
//	}
//	else
//	{
//		if (!FPaths::FileExists(FinalePath))
//		{
//			UE_LOG(LogTemp, Warning, TEXT("File wasn't found"));
//		}
//	}
//	
//}
//
//FString UBaseGlobalWidget::GetRootPath()
//{
//	FString PathStr = "";
//#if PLATFORM_ANDROID
//	TArray<FString> Folders;
//	GExternalFilePath.ParseIntoArray(Folders, TEXT("/"));
//	for (FString Folder : Folders)
//	{
//		PathStr += FString("/..");
//	}
//
//#endif
//	return PathStr;
//}
