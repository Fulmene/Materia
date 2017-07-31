// Fill out your copyright notice in the Description page of Project Settings.


#include "RosImage.h"

#include "Utility.h"

bool RosImage::AdvertiseImageTopic(
        TSharedPtr<RosBridge> Bridge,
        const FString& BaseTopicName,
        RosImage::EImageTransport Transport
        ) {
    FString TopicName(AppendTopicName(BaseTopicName, Transport));
    FString MsgType;
    switch (Transport) {
        case JPEG:
        case PNG:
            MsgType = "sensor_msgs/CompressedImage";
            break;
        case Theora:
            MsgType = "theora_image_transport/Packet";
            break;
        default:
            MsgType = "sensor_msgs/Image";
    };
    return Bridge->Advertise(TopicName, MsgType);
}

bool RosImage::UnadvertiseImageTopic(
        TSharedPtr<RosBridge> Bridge,
        const FString& BaseTopicName,
        RosImage::EImageTransport Transport
        ) {
    FString TopicName(AppendTopicName(BaseTopicName, Transport));
    return Bridge->Unadvertise(TopicName);
}

bool RosImage::PublishRawImage(
        TSharedPtr<RosBridge> Bridge,
        const FString& BaseTopicName,
        const RosMsgType::SensorMsgs::Image& Image
        ) {
    return Bridge->Publish(
            BaseTopicName,
            RosMsgType::ToJson(Image)
            );
}

TSharedPtr<RosMsgType::SensorMsgs::Image> RosImage::CreateRawImage(
        const TArray<FColor>& Bitmap,
        uint32 ImageWidth,
        uint32 ImageHeight
        ) {

    TArray<uint8> RgbBitmap;
    for (FColor c : Bitmap) {
        RgbBitmap.Add(c.R);
        RgbBitmap.Add(c.G);
        RgbBitmap.Add(c.B);
    }

    return MakeShareable(
		new RosMsgType::SensorMsgs::Image(
			ImageHeight,
            ImageWidth,
            "rgb8",
            false,
            3*ImageWidth,
            RgbBitmap
		)
	);
}

bool RosImage::PublishCompressedImage(
        TSharedPtr<RosBridge> Bridge,
        const FString& BaseTopicName,
        const RosMsgType::SensorMsgs::CompressedImage& Image
        ) {
    FString TopicName(AppendTopicName(BaseTopicName, RosImage::EImageTransport::PNG));
    return Bridge->Publish(
            TopicName,
            RosMsgType::ToJson(Image)
            );
}

TSharedPtr<RosMsgType::SensorMsgs::CompressedImage> RosImage::CreateCompressedImage(
        const TArray<FColor>& Bitmap,
        uint32 ImageWidth,
        uint32 ImageHeight,
		RosImage::EImageTransport ImageTransport
        ) {
	ensure(ImageTransport == PNG || ImageTransport == JPEG);
    TArray<uint8> Data;
    Utility::CompressImageDataPng(ImageWidth, ImageHeight, Bitmap, Data);
    return MakeShareable(new RosMsgType::SensorMsgs::CompressedImage(ImageTransport == PNG ? "png" : "jpeg", Data));
}

FString RosImage::AppendTopicName(
        const FString& BaseTopicName,
        RosImage::EImageTransport Transport
        ) {
    FString TopicName(BaseTopicName);
    switch (Transport) {
        case JPEG:
        case PNG:
            TopicName += "/compressed";
            break;
        case Theora:
            TopicName += "/theora";
            break;
    }
    return TopicName;
}
