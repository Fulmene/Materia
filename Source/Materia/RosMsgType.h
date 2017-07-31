// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Json.h"

/**
 *
 */
class MATERIA_API RosMsgType {
public:

    class StdMsgs {
    public:

        class Time {
        public:
            uint32 Secs;
            uint32 Nsecs;
            float ToFloat() const;
        };

        class Header {
        public:
            uint32 Seq;
            Time Stamp;
            FString FrameId;
        };

    };

    class GeometryMsgs {
    public:

        class Pose {
        public:
            FVector Position;
            FQuat Orientation;
        };

        class Twist {
        public:
            FVector Linear;
            FVector Angular;
        };

    };

    class SensorMsgs {
    public:

        class Image {
        public:
            StdMsgs::Header Header;

            uint32 Height;
            uint32 Width;

            FString Encoding;
            uint8 IsBigendian;
            uint32 Step;

            TArray<uint8> Data;

            Image(uint32 InHeight, uint32 InWidth, const FString& InEncoding, uint8 InBigendian, uint32 InStep, const TArray<uint8>& InData);
        };

        class CompressedImage {
        public:
            StdMsgs::Header Header;
            FString Format;
            TArray<uint8> Data;

            CompressedImage(const FString& InFormat, const TArray<uint8>& InData);
        };

    };

    class GazeboMsgs {
    public:
        class GetModelState {
        public:
            class Args {
            public:
                Args();
                Args(const FString& InModelName);
                Args(const FString& InModelName, const FString& InRelativeEntityName);
                FString ModelName;
                FString RelativeEntityName;
            };
            class Values {
            public:
                RosMsgType::StdMsgs::Header Header;
                RosMsgType::GeometryMsgs::Pose Pose;
                RosMsgType::GeometryMsgs::Twist Twist;
            };
        };
    };

    static bool FromJson(TSharedPtr<FJsonObject> Obj, StdMsgs::Time& Res);
    static bool FromJson(TSharedPtr<FJsonObject> Obj, StdMsgs::Header& Res);

    static bool LinearFromJson(TSharedPtr<FJsonObject> Obj, FVector& Res);
    static bool AngularFromJson(TSharedPtr<FJsonObject> Obj, FVector& Res);
    static bool OrientationFromJson(TSharedPtr<FJsonObject> Obj, FQuat& Res);
    static bool RotationFromJson(TSharedPtr<FJsonObject> Obj, FQuat& Res);

    static bool FromJson(TSharedPtr<FJsonObject> Obj, GeometryMsgs::Pose& Res);
    static bool FromJson(TSharedPtr<FJsonObject> Obj, GeometryMsgs::Twist& Res);

    static TSharedPtr<FJsonObject> ToJson(const SensorMsgs::Image& Res);
    static TSharedPtr<FJsonObject> ToJson(const SensorMsgs::CompressedImage& Res);

    static TSharedPtr<FJsonObject> ToJson(const GazeboMsgs::GetModelState::Args& Res);
    static bool FromJson(TSharedPtr<FJsonObject> Obj, GazeboMsgs::GetModelState::Values& Res);

};
