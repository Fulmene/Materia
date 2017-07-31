// Fill out your copyright notice in the Description page of Project Settings.

#include "ImageReaderRunnable.h"

#include "Utility.h"

#include "RosImage.h"

ImageReaderRunnable::ImageReaderRunnable(uint32 InImageWidth, uint32 InImageHeight) {
    ImageWidth = InImageWidth;
    ImageHeight = InImageHeight;
	Thread = FRunnableThread::Create(this, TEXT("Image Reader Thread"));
}

ImageReaderRunnable::~ImageReaderRunnable() {
    Thread->Kill();
    delete Thread;
    Thread = nullptr;
}

bool ImageReaderRunnable::Init() {
    bRunning = true;
    return true;
}

uint32 ImageReaderRunnable::Run() {
    while (bRunning) {
        TArray<uint8> ImageData(ReadBitmap());
		TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> CompressedImage;
        if (ImageData.Num() > 0)
            CompressedImage = MakeShareable(new RosMsgType::SensorMsgs::CompressedImage("jpeg", ImageData));

		{
			FScopeLock Lock(&ImageSynchronize);
			Image = CompressedImage;
		}

        FPlatformProcess::Sleep(1.0 / 60);
    }
    return 0;
}

void ImageReaderRunnable::Stop() {
    bRunning = false;
}

void ImageReaderRunnable::SetBitmap(const TArray<FColor>& InBitmap) {
    {
        FScopeLock Lock(&BitmapSynchronize);
        Bitmap = InBitmap;
    }
}

void ImageReaderRunnable::SetBitmap(TArray<FColor>&& InBitmap) {
    {
        FScopeLock Lock(&BitmapSynchronize);
        Bitmap = InBitmap;
    }
}

TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> ImageReaderRunnable::GetImage() {
    {
        FScopeLock Lock(&ImageSynchronize);
        return Image;
    }
}

TArray<uint8> ImageReaderRunnable::ReadBitmap() {
    TArray<FColor> CopyBitmap;
    {
        FScopeLock Lock(&BitmapSynchronize);
        CopyBitmap = Bitmap;
    }
    TArray<uint8> ImageData;
    if (CopyBitmap.Num() > 0)
        Utility::CompressImageDataJpg(ImageWidth, ImageHeight, CopyBitmap, ImageData);
    return ImageData;
}
