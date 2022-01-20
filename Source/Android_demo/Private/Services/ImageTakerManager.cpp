#include "Services/ImageTakerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintPathsLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "Globals.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "IImageWrapper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"



extern FString GExternalFilePath;

//UImageTakerManager* UImageTakerManager::ImgTakerManagerObj = nullptr;

void UImageTakerManager::ReauestImageFromPlatform()
{
#if PLATFORM_ANDROID
	FOnImagePicked locDlg;
	locDlg.AddDynamic(this, &UImageTakerManager::ReqeustComplete);
	UAndroidGrabber* AndrGrab = NewObject<UAndroidGrabber>();

	if (AndrGrab)
	{
		AndrGrab->AndroidAPITemplate_GrabImage(locDlg);
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

void UImageTakerManager::ReqeustComplete(FString filePath)
{
	UTexture2D* LocImage = nullptr;
	FString filename = filePath;
	filename = filename.Replace(*Constants::Raw, *Constants::Empty);
	FPaths::NormalizeFilename(filename);

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(GetFileFormat(filePath));
	
	TArray<uint8> binData = TArray<uint8>();
	FString FinalePath = GetRootPath() + filename;
	if (FFileHelper::LoadFileToArray(binData, *FinalePath)) {

		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(binData.GetData(), binData.Num()))
		{
			//ImageWrapper->SetRaw(&binData[0], binData.Num() * sizeof(uint8), ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), ERGBFormat::BGRA, 8);
			TArray64<uint8> UncompressedBGRA;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{

				//UncompressedBGRA = ImageWrapper->GetCompressed(90);

				LocImage = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

				//Valid?
				if (!LocImage) return;
				//~~~~~~~~~~~~~~

				////Out!
				//Width = ImageWrapper->GetWidth();
				//Height = ImageWrapper->GetHeight();

				//Copy!
				void* TextureData = LocImage->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
				LocImage->PlatformData->Mips[0].BulkData.Unlock();

				//Update!
				LocImage->UpdateResource();
				image = LocImage;
			}
		}

		//image = UKismetRenderingLibrary::ImportBufferAsTexture2D(GetWorld(), binData);
		

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

EImageFormat UImageTakerManager::GetFileFormat(const FString& path)
{
	EImageFormat imageFormat = EImageFormat::Invalid;
	FString format = FPaths::GetExtension(path);
	
	if (format.Equals("jpg", ESearchCase::IgnoreCase) || format.Equals("jpeg", ESearchCase::IgnoreCase))
	{
		imageFormat = EImageFormat::JPEG;
	}
	else if (format.Equals("png", ESearchCase::IgnoreCase))
	{
		imageFormat = EImageFormat::PNG;
	}
	else if (format.Equals("bmp", ESearchCase::IgnoreCase))
	{
		imageFormat = EImageFormat::BMP;
	}
	return imageFormat;
}
