// Fill out your copyright notice in the Description page of Project Settings.


#include "ImageStreamerComponent.h"

#include "RosImage.h"


// Sets default values for this component's properties
UImageStreamerComponent::UImageStreamerComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
    IPAddress = RosBridge::DefaultIpAddr;
    Port = RosBridge::DefaultPort;
    bUseUDP = false;
    FramePerSecond = 60;
}


// Called when the game starts
void UImageStreamerComponent::BeginPlay() {
    Super::BeginPlay();

    // ...
    ImageReader = MakeShareable(new ImageReaderRunnable(StreamedImage->GetSurfaceWidth(), StreamedImage->GetSurfaceHeight()));
    ImageStreamer = MakeShareable(new ImageStreamerRunnable(IPAddress, Port, bUseUDP, BaseTopicName, FramePerSecond));
}

// Called every frame
void UImageStreamerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
    TArray<FColor> Bitmap;
    Bitmap.AddUninitialized(StreamedImage->GetSurfaceWidth() * StreamedImage->GetSurfaceHeight());
    FTextureRenderTargetResource* ImageResource = StreamedImage->GameThread_GetRenderTargetResource();
    FReadSurfaceDataFlags ReadFlags(ERangeCompressionMode::RCM_UNorm);
    ImageResource->ReadPixels(Bitmap, ReadFlags);
    ImageReader->SetBitmap(MoveTemp(Bitmap));

    auto Image = ImageReader->GetImage();
    if (Image.IsValid())
        ImageStreamer->AddImage(Image);
}
