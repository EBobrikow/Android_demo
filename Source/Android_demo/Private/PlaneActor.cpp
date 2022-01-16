// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneActor.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
APlaneActor::APlaneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	StaticMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APlaneActor::ApplyImageOnPlane(UTexture2D* image)
{
	double sizeX = image->GetSizeX();
	double sizeY = image->GetSizeY();

	double scaleX = sizeX / sizeY;
	double scaleY = sizeY / sizeX;

	StaticMeshComp->SetRelativeScale3D(FVector(scaleX, scaleY, 0.01f));
	

	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
	

	DynMaterial->SetTextureParameterValue(FName("T2DParam"), image);
	DynMaterial->SetScalarParameterValue(FName("scaleX"), scaleX);
	DynMaterial->SetScalarParameterValue(FName("scaleY"), scaleY);

	

	if (StaticMeshComp)
	{
		StaticMeshComp->SetMaterial(0, DynMaterial);
		
		
	}

}

// Called when the game starts or when spawned
void APlaneActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlaneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

