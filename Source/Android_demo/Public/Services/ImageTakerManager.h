#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakObjectPtr.h"
#include "AndroidGrabber.h"
#include "Actors/PlaneActor.h"
#include "ImageTakerManager.generated.h"

/**
 * 
 */
UCLASS()
class ANDROID_DEMO_API UImageTakerManager : public UObject
{
	GENERATED_BODY()
	

public:
	
	void ReauestImageFromPlatform();


private:

	UFUNCTION()
	void ReqeustComplete(const FString& filePath);

	FString GetRootPath();

	UPROPERTY()
	APlaneActor* PlaneActor = nullptr;

	UPROPERTY()
	UTexture2D* image = nullptr;

	
	
	
	
};
