// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AndroidGrabber.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImagePicked, FString, ImagePath);

UCLASS()
class IMAGETAKER_API UAndroidGrabber : public UObject
{
	GENERATED_BODY()
	
public:

	
	FOnImagePicked OnImagePickedDelegate;


#if PLATFORM_ANDROID
	static bool InitJavaFunctions();
#endif


	UFUNCTION()
	static void AndroidAPITemplate_GrabImage(FOnImagePicked ImgPickedDlg);


};
