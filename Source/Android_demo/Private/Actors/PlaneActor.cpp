// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlaneActor.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
APlaneActor::APlaneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	StaticMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APlaneActor::ApplyImageOnPlane(UTexture2D* image)
{
	double sizeX = image->GetSizeX();
	double sizeY = image->GetSizeY();

	

	double scaleX = sizeX / sizeY;
	double scaleY = sizeY / sizeX;

	
	FVector CurScale = MeshScale;
	if (sizeX > sizeY)
		CurScale.X *= scaleX;
	else
		CurScale.Y *= scaleY;
	


	if (StaticMeshComp)
		StaticMeshComp->SetRelativeScale3D(CurScale);

	
	if (DynMaterial)
		DynMaterial->SetTextureParameterValue(FName("T2DParam"), image);

}


void APlaneActor::BeginPlay()
{
	Super::BeginPlay();

	MeshScale = StaticMeshComp->GetRelativeScale3D();
	DynMaterial = UMaterialInstanceDynamic::Create(Material, this);

	if (StaticMeshComp)
	{
		StaticMeshComp->SetMaterial(0, DynMaterial);

	}
}


void APlaneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

