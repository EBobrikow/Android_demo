#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakObjectPtr.h"
#include "AndroidGrabber.h"
#include "Actors/PlaneActor.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "ImageTakerManager.generated.h"

/**
 * 
 */
UCLASS()
class ANDROID_DEMO_API UImageTakerManager : public UObject
{
	GENERATED_BODY()
	

public:
	
	/*static UImageTakerManager * ImgTakerManagerObj;

	static UImageTakerManager * GetManager(UObject* WorldContext)
	{
		
		if (!ImgTakerManagerObj)
		{
			ImgTakerManagerObj = NewObject<UImageTakerManager>(WorldContext);
			ImgTakerManagerObj->Context = WorldContext;
		}

		return ImgTakerManagerObj;
	}*/
	
	
	void ReauestImageFromPlatform();

private:

	UFUNCTION()
	void ReqeustComplete(FString filePath);

	FString GetRootPath();

	UPROPERTY()
	UObject* Context;

	UPROPERTY()
	APlaneActor* PlaneActor = nullptr;

	UPROPERTY()
	UTexture2D* image = nullptr;

	
	EImageFormat GetFileFormat(const FString& path);
};
