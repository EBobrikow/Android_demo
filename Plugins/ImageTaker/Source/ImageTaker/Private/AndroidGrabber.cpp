// Fill out your copyright notice in the Description page of Project Settings.


#include "AndroidGrabber.h"
#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Async/Async.h"

static FString ImagePath;
static FOnImagePicked ImgPickedCallback;

#if PLATFORM_ANDROID

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include <jni.h>

#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true)) { \
	name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
	check(name != NULL); \
} else { \
	check(0); \
}

#define DECLARE_JAVA_METHOD(name) \
static jmethodID name = NULL;

DECLARE_JAVA_METHOD(AndroidThunkJava_AndroidAPI_GrabImage);		


void UAndroidGrabber::InitJavaFunctions()
{
	
	INIT_JAVA_METHOD(AndroidThunkJava_AndroidAPI_GrabImage, "()V");
	
}
#undef DECLARE_JAVA_METHOD
#undef INIT_JAVA_METHOD

#endif

void UAndroidGrabber::AndroidAPITemplate_GrabImage(FOnImagePicked ImgPickedDlg)
{
	ImgPickedCallback = ImgPickedDlg;
	
	
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		InitJavaFunctions();
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_AndroidAPI_GrabImage);
	}
#endif
}



#if PLATFORM_ANDROID
extern "C"
{
	JNIEXPORT void Java_com_epicgames_ue4_GameActivity_imagePicked(JNIEnv* jni, jclass clazz, jstring code)
	{
		
		const char* charsId = jni->GetStringUTFChars(code, 0);
		ImagePath = FString(UTF8_TO_TCHAR(charsId));
		jni->ReleaseStringUTFChars(code, charsId);
		
		UE_LOG(LogTemp, Log, TEXT("Java callback, path: %s"), *ImagePath);

		if (ImgPickedCallback.IsBound())
		{
			AsyncTask(ENamedThreads::GameThread, [&]()
				{
					ImgPickedCallback.Broadcast(ImagePath);
					ImgPickedCallback.Clear();
				}
			);

		}
	}


}
#endif
