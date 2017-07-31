// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RosBridge.h"
#include "RosMsgType.h"

/**
 * 
 */
class MATERIA_API RosImage {

public:

    enum EImageTransport { Raw, JPEG, PNG, Theora };

    static bool AdvertiseImageTopic(
            TSharedPtr<RosBridge> Bridge,
            const FString& BaseTopicName,
            EImageTransport Transport
            );

    static bool UnadvertiseImageTopic(
            TSharedPtr<RosBridge> Bridge,
            const FString& BaseTopicName,
            EImageTransport Transport
            );

    static bool PublishRawImage(
            TSharedPtr<RosBridge> Bridge,
            const FString& BaseTopicName,
            const RosMsgType::SensorMsgs::Image& Image
            );

    static TSharedPtr<RosMsgType::SensorMsgs::Image> CreateRawImage(
            const TArray<FColor>& Bitmap,
            uint32 ImageWidth,
            uint32 ImageHeight
            );

    static bool PublishCompressedImage(
            TSharedPtr<RosBridge> Bridge,
            const FString& BaseTopicName,
            const RosMsgType::SensorMsgs::CompressedImage& Image
            );

    static TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> CreateCompressedImage(
            const TArray<FColor>& Bitmap,
            uint32 ImageWidth,
            uint32 ImageHeight,
			RosImage::EImageTransport ImageTransport
            );

private:
    static FString AppendTopicName(
            const FString& BaseTopicName,
            EImageTransport Transport
            );

};
