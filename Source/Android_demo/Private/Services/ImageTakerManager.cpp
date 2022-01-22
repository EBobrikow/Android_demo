#include "Services/ImageTakerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintPathsLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Globals.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Core/Public/Misc/Paths.h"



extern FString GExternalFilePath;

void UImageTakerManager::ReauestImageFromPlatform()
{
#if PLATFORM_ANDROID
	FOnImagePicked locDlg;
	locDlg.AddDynamic(this, &UImageTakerManager::ReqeustComplete);
	UAndroidGrabber* AndrGrab = NewObject<UAndroidGrabber>();

	if (AndrGrab)
	{
		AndrGrab->GrabImage(locDlg);
	}

#elif PLATFORM_WINDOWS
	
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	TArray<FString> outFileNames;

	if (DesktopPlatform->OpenFileDialog(0, Constants::DlgTitle, Constants::DfltPath, Constants::Empty, Constants::ImgFileFormat, EFileDialogFlags::None, outFileNames))
	{
		if (outFileNames.Num() > 0)
		{
			ReqeustComplete(outFileNames[0]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You didn't choose the image"));
	}

#endif

}


void UImageTakerManager::ReqeustComplete(const FString& filePath)
{
	UTexture2D* LocImage = nullptr;
	FString filename = filePath;
	filename = filename.Replace(*Constants::Raw, *Constants::Empty);
	FPaths::NormalizeFilename(filename);

	
	TArray<uint8> binData = TArray<uint8>();
	FString FinalePath = GetRootPath() + filename;
	if (FFileHelper::LoadFileToArray(binData, *FinalePath)) {

		image = UKismetRenderingLibrary::ImportBufferAsTexture2D(GetWorld(), binData);

		if (!PlaneActor)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlaneActor::StaticClass(), FName(Constants::ActorTag), FoundActors);
			if (FoundActors.Num() > 0)
			{
				PlaneActor = Cast<APlaneActor>(FoundActors[0]);	

			}
		}
		
		if (PlaneActor)
			PlaneActor->ApplyImageOnPlane(image);


	}
	else
	{
		if (!FPaths::FileExists(FinalePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("File wasn't found"));
		}
	}
}

FString UImageTakerManager::GetRootPath()
{
	FString PathStr = "";
#if PLATFORM_ANDROID
	TArray<FString> Folders;
	GExternalFilePath.ParseIntoArray(Folders, TEXT("/"));
	for (FString Folder : Folders)
	{
		PathStr += FString("/..");
	}

#endif
	return PathStr;
}
