// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WindowsCriticalSection.h"

#include "RosBridge.h"
#include "RosMsgType.h"

/**
 * 
 */
class MATERIA_API ImageStreamerRunnable : public FRunnable {

public:
	ImageStreamerRunnable(const FString& InIpAddr, uint16 InPort, bool InUdp, const FString& InBaseTopicName, uint16 InFrameRate);
	~ImageStreamerRunnable();

    bool Init() override;
    uint32 Run() override;
    void Stop() override;

    void AddImage(TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> InImage);

private:
    FRunnableThread* Thread;

    TSharedPtr<RosBridge> Bridge;
    FString IpAddr;
    uint16 Port;
    bool bUdp;

    TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> Image;
    FString BaseTopicName;
    uint16 FrameRate;

    FCriticalSection ImageSynchronize;
    FThreadSafeBool bRunning;

};
