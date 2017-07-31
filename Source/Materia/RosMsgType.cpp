#include "RosMsgType.h"
#include "Utility.h"

float RosMsgType::StdMsgs::Time::ToFloat() const {
    return Secs + Nsecs*1e-9;
}

RosMsgType::SensorMsgs::Image::Image(
        uint32 InHeight,
        uint32 InWidth,
        const FString& InEncoding,
        uint8 InBigendian,
        uint32 InStep,
        const TArray<uint8>& InData) 
    : Height(InHeight), Width(InWidth),
    Encoding(InEncoding), IsBigendian(InBigendian), Step(InStep),
    Data(InData) {
}

RosMsgType::SensorMsgs::CompressedImage::CompressedImage(
        const FString& InFormat,
        const TArray<uint8>& InData
        ) : Format(InFormat), Data(InData) {
}

bool RosMsgType::FromJson(
        TSharedPtr<FJsonObject> Obj,
        RosMsgType::StdMsgs::Time& Res
        ) {
    return
        Obj->TryGetNumberField("secs", Res.Secs) &&
        Obj->TryGetNumberField("nsecs", Res.Nsecs);
}

bool RosMsgType::FromJson(
        TSharedPtr<FJsonObject> Obj,
        RosMsgType::StdMsgs::Header& Res
        ) {
    TSharedPtr<FJsonObject> StampPtr;
    return
        Utility::TryGetObjectField(Obj, "stamp", StampPtr) &&
        FromJson(StampPtr, Res.Stamp) &&
        Obj->TryGetNumberField("seq", Res.Seq) &&
        Obj->TryGetStringField("frame_id", Res.FrameId);
}

bool RosMsgType::LinearFromJson(
        TSharedPtr<FJsonObject> Obj,
        FVector& Res
        ) {
    // Also convert ROS coordinate to UE4 coordinate
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    bool Valid =
        Obj->TryGetNumberField("x", x) &&
        Obj->TryGetNumberField("y", y) &&
        Obj->TryGetNumberField("z", z);
    // ROS length unit is metre
    // UE4 length unit is centimetre
    Res.X = 100*y;
    Res.Y = 100*x;
    Res.Z = 100*z;
    return Valid;
}

bool RosMsgType::AngularFromJson(
        TSharedPtr<FJsonObject> Obj,
        FVector& Res
        ) {
    // Also convert ROS coordinate to UE4 coordinate
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    bool Valid =
        Obj->TryGetNumberField("x", x) &&
        Obj->TryGetNumberField("y", y) &&
        Obj->TryGetNumberField("z", z);
    Res.X = -y;
    Res.Y = -x;
    Res.Z = -z;
    return Valid;
}

bool RosMsgType::OrientationFromJson(
        TSharedPtr<FJsonObject> Obj,
        FQuat& Res
        ) {
    // Also convert ROS coordinate to UE4 coordinate
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 0.0;
    bool Valid =
        Obj->TryGetNumberField("x", x) &&
        Obj->TryGetNumberField("y", y) &&
        Obj->TryGetNumberField("z", z) &&
        Obj->TryGetNumberField("w", w);
    Res.X = -y;
    Res.Y = -x;
    Res.Z = -z;
    Res.W = w;
    return Valid;
}

bool RosMsgType::RotationFromJson(
        TSharedPtr<FJsonObject> Obj,
        FQuat& Res
        ) {
    // Rotation quaternion doesn't change from ROS to UE4
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 0.0;
    bool Valid =
        Obj->TryGetNumberField("x", x) &&
        Obj->TryGetNumberField("y", y) &&
        Obj->TryGetNumberField("z", z) &&
        Obj->TryGetNumberField("w", w);
    Res.X = x;
    Res.Y = y;
    Res.Z = z;
    Res.W = w;
    return Valid;
}

bool RosMsgType::FromJson(
        TSharedPtr<FJsonObject> Obj,
        RosMsgType::GeometryMsgs::Pose& Res
        ) {
    TSharedPtr<FJsonObject> PositionPtr;
    TSharedPtr<FJsonObject> OrientationPtr;
    return
        Utility::TryGetObjectField(Obj, "position", PositionPtr) &&
        LinearFromJson(PositionPtr, Res.Position) &&
        Utility::TryGetObjectField(Obj, "orientation", OrientationPtr) &&
        OrientationFromJson(OrientationPtr, Res.Orientation);
}

bool RosMsgType::FromJson(
        TSharedPtr<FJsonObject> Obj,
        RosMsgType::GeometryMsgs::Twist& Res
        ) {
    TSharedPtr<FJsonObject> LinearPtr;
    TSharedPtr<FJsonObject> AngularPtr;
    return
        Utility::TryGetObjectField(Obj, "linear", LinearPtr) &&
        LinearFromJson(LinearPtr, Res.Linear) &&
        Utility::TryGetObjectField(Obj, "angular", AngularPtr) &&
        AngularFromJson(AngularPtr, Res.Angular);
}

TSharedPtr<FJsonObject> RosMsgType::ToJson(const RosMsgType::SensorMsgs::Image& Res) {
    TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
    Obj->SetNumberField("height", Res.Height);
    Obj->SetNumberField("width", Res.Width);
    Obj->SetStringField("encoding", Res.Encoding);
    Obj->SetNumberField("is_bigendian", Res.IsBigendian);
    Obj->SetNumberField("step", Res.Step);
    TArray<TSharedPtr<FJsonValue>> DataStream;
    for (uint8 d : Res.Data)
        DataStream.Add(MakeShareable(new FJsonValueNumber(d)));
    Obj->SetArrayField("data", DataStream);
    return Obj;
}

TSharedPtr<FJsonObject> RosMsgType::ToJson(const RosMsgType::SensorMsgs::CompressedImage& Res) {
    TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
    Obj->SetStringField("format", Res.Format);
    TArray<TSharedPtr<FJsonValue>> DataStream;
    for (uint8 d : Res.Data)
        DataStream.Add(MakeShareable(new FJsonValueNumber(d)));
    Obj->SetArrayField("data", DataStream);
    return Obj;
}

RosMsgType::GazeboMsgs::GetModelState::Args::Args()
    : Args("") {
}

RosMsgType::GazeboMsgs::GetModelState::Args::Args(const FString& InModelName)
    : Args(InModelName, "") {
}

RosMsgType::GazeboMsgs::GetModelState::Args::Args(const FString& InModelName, const FString& InRelativeEntityName)
    : ModelName(InModelName), RelativeEntityName(InRelativeEntityName) {
}

TSharedPtr<FJsonObject> RosMsgType::ToJson(
        const RosMsgType::GazeboMsgs::GetModelState::Args& Res
        ) {
    TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
    Obj->SetStringField("model_name", Res.ModelName);
    Obj->SetStringField("relative_entity_name", Res.RelativeEntityName);
    return Obj;
}

bool RosMsgType::FromJson(
        TSharedPtr<FJsonObject> Obj,
        RosMsgType::GazeboMsgs::GetModelState::Values& Res
        ) {
    TSharedPtr<FJsonObject> HeaderPtr;
    TSharedPtr<FJsonObject> PosePtr;
    TSharedPtr<FJsonObject> TwistPtr;
    return
        Utility::TryGetObjectField(Obj, "header", HeaderPtr) &&
        RosMsgType::FromJson(HeaderPtr, Res.Header) &&
        Utility::TryGetObjectField(Obj, "pose", PosePtr) &&
        RosMsgType::FromJson(PosePtr, Res.Pose) &&
        Utility::TryGetObjectField(Obj, "twist", TwistPtr) &&
        RosMsgType::FromJson(TwistPtr, Res.Twist);
}
