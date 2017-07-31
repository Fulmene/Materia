// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "WindowsPlatformProcess.h"
#include "Engine/TextureRenderTarget2D.h"

#include "RosBridge.h"
#include "RosMsgType.h"
#include "ImageReaderRunnable.h"
#include "ImageStreamerRunnable.h"

#include "ImageStreamerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MATERIA_API UImageStreamerComponent : public UActorComponent {

    GENERATED_BODY()


protected:
    // Forward declaration
    class StreamThread;

public:	
    // Sets default values for this component's properties
    UImageStreamerComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
    UTextureRenderTarget2D* StreamedImage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
    FString BaseTopicName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
    int32 FramePerSecond;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge")
    FString IPAddress;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge")
    int32 Port;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge")
    bool bUseUDP;

protected:
    TSharedPtr<ImageReaderRunnable> ImageReader;
    TSharedPtr<ImageStreamerRunnable> ImageStreamer;

    TArray<FColor> Bitmap;

};
