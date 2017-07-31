// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WindowsCriticalSection.h"

#include "RosMsgType.h"

/**
 * 
 */
class MATERIA_API ImageReaderRunnable : public FRunnable {

public:

	ImageReaderRunnable(uint32 ImageWidth, uint32 ImageHeight);
	~ImageReaderRunnable();

    bool Init() override;
    uint32 Run() override;
    void Stop() override;

    void SetBitmap(const TArray<FColor>& InBitmap);
    void SetBitmap(TArray<FColor>&& InBitmap);
    TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> GetImage();

private:

    FRunnableThread* Thread;

    TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> Image;

    TArray<FColor> Bitmap;
    uint32 ImageWidth;
    uint32 ImageHeight;

    FThreadSafeBool bRunning;
    FCriticalSection BitmapSynchronize;
    FCriticalSection ImageSynchronize;

    TArray<uint8> ReadBitmap();

};

