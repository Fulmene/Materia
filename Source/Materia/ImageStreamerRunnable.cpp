// Fill out your copyright notice in the Description page of Project Settings.

#include "ImageStreamerRunnable.h"

#include "RosImage.h"

ImageStreamerRunnable::ImageStreamerRunnable(const FString& InIpAddr, uint16 InPort, bool InUdp, const FString& InBaseTopicName, uint16 InFrameRate)  {
    IpAddr = InIpAddr;
    Port = InPort;
    bUdp = InUdp;
    BaseTopicName = InBaseTopicName;
    FrameRate = InFrameRate;
    Thread = FRunnableThread::Create(this, TEXT("Image Streamer Thread"));
}

ImageStreamerRunnable::~ImageStreamerRunnable() {
    Thread->Kill();
    delete Thread;
    Thread = nullptr;
}

bool ImageStreamerRunnable::Init() {
    Bridge = MakeShareable(new RosBridge(IpAddr, Port, bUdp));
    bRunning = true;
    return true;
}

uint32 ImageStreamerRunnable::Run() {
    RosImage::AdvertiseImageTopic(Bridge, BaseTopicName, RosImage::EImageTransport::JPEG);
    while (bRunning) {
        TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> CopyImage;
        {
            FScopeLock Lock(&ImageSynchronize);
            CopyImage = Image;
        }
        if (CopyImage.IsValid())
            RosImage::PublishCompressedImage(Bridge, BaseTopicName, *CopyImage);
		FPlatformProcess::Sleep(1.0/FrameRate);
    }
    RosImage::UnadvertiseImageTopic(Bridge, BaseTopicName, RosImage::EImageTransport::JPEG);
    return 0;
}

void ImageStreamerRunnable::Stop() {
    bRunning = false;
}

void ImageStreamerRunnable::AddImage(TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> InImage) {
    {
        FScopeLock Lock(&ImageSynchronize);
        Image = InImage;
    }
}
