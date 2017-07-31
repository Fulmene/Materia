// Fill out your copyright notice in the Description page of Project Settings.


#include "RosBridge.h"
#include "Utility.h"

const FString RosBridge::DefaultIpAddr = "127.0.0.1";
const uint16 RosBridge::DefaultPort = 9090;
const int32 RosBridge::BufferSize = 2*1024*1024;

RosBridge::RosBridge() : RosBridge(DefaultIpAddr, DefaultPort) {
}

RosBridge::RosBridge(const FString& IpAddr, uint16 Port) : RosBridge(IpAddr, Port, true) {
}

RosBridge::RosBridge(const FString& IpAddr, const uint16 Port, bool IsUdp) {
    TSharedPtr<FInternetAddr> RosBridgeAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool IpAddrValid;
    RosBridgeAddr->SetIp(*IpAddr, IpAddrValid);
    RosBridgeAddr->SetPort(Port);

    if (IsUdp) {
        Socket = FUdpSocketBuilder("ROSBridge").
            WithSendBufferSize(BufferSize).
            WithReceiveBufferSize(BufferSize)
            ;
    }
    else {
        Socket = FTcpSocketBuilder("ROSBridge").
            WithSendBufferSize(BufferSize).
            WithReceiveBufferSize(BufferSize)
            ;
    }
    Socket->Connect(*RosBridgeAddr);
}

RosBridge::~RosBridge() {
    if (Socket == nullptr) {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
    }
}

bool RosBridge::Advertise(
        const FString& Topic,
        const FString& Type
        ) const {
    TSharedRef<FJsonObject> AdvertiseObj = MakeShareable(new FJsonObject());
    AdvertiseObj->SetStringField("op", "advertise");
    AdvertiseObj->SetStringField("topic", Topic);
    AdvertiseObj->SetStringField("type", Type);
    return Send(AdvertiseObj);
}

bool RosBridge::Unadvertise(
        const FString& Topic
        ) const {
    TSharedRef<FJsonObject> UnadvertiseObj = MakeShareable(new FJsonObject());
    UnadvertiseObj->SetStringField("op", "unadvertise");
    UnadvertiseObj->SetStringField("topic", Topic);
    return Send(UnadvertiseObj);
}

bool RosBridge::Publish(
        const FString& Topic,
        TSharedPtr<FJsonObject> Msg
        ) const {
    TSharedRef<FJsonObject> PublishObj = MakeShareable(new FJsonObject());
    PublishObj->SetStringField("op", "publish");
    PublishObj->SetStringField("topic", Topic);
    PublishObj->SetObjectField("msg", Msg);
    return Send(PublishObj);
}

bool RosBridge::Subscribe(
	const FString& Topic,
	int32 ThrottleRate
	) const {
	TSharedRef<FJsonObject> SubscribeObj = MakeShareable(new FJsonObject());
	SubscribeObj->SetStringField("op", "subscribe");
	SubscribeObj->SetStringField("topic", Topic);
	SubscribeObj->SetNumberField("throttle_rate", ThrottleRate);
	return Send(SubscribeObj);
}

bool RosBridge::Unsubscribe(
		const FString& Topic
		) const {
	TSharedRef<FJsonObject> UnsubscribeObj = MakeShareable(new FJsonObject());
	UnsubscribeObj->SetStringField("op", "unsubscribe");
	UnsubscribeObj->SetStringField("topic", Topic);
	return Send(UnsubscribeObj);
}

bool RosBridge::CallService(
        const FString& Service
        ) const {
    return CallService(Service, nullptr);
}

bool RosBridge::CallService(
        const FString& Service,
        const TSharedPtr<FJsonObject>& Args
        ) const {
    // Send service call
    TSharedRef<FJsonObject> CallServiceObj = MakeShareable(new FJsonObject());
    CallServiceObj->SetStringField("op", "call_service");
    CallServiceObj->SetStringField("service", Service);
    if (Args.IsValid())
        CallServiceObj->SetObjectField("args", Args);
    return Send(CallServiceObj);
}

bool RosBridge::CallService(
        const FString& Service,
        TSharedPtr<FJsonObject>& Values
        ) const {
    return CallService(Service, nullptr, Values);
}

bool RosBridge::CallService(
        const FString& Service,
        const TSharedPtr<FJsonObject>& Args,
        TSharedPtr<FJsonObject>& Values
        ) const {

    // Receive response
    TSharedPtr<FJsonObject> ServiceResponseObj;
    return
        CallService(Service, Args) &&
        Recv(ServiceResponseObj) &&
        ServiceResponseObj.IsValid() &&
        ServiceResponseObj->HasTypedField<EJson::String>("op") && ServiceResponseObj->GetStringField("op") == "service_response" &&
        ServiceResponseObj->HasTypedField<EJson::String>("service") && ServiceResponseObj->GetStringField("service") == Service &&
        ServiceResponseObj->HasTypedField<EJson::Boolean>("result") && ServiceResponseObj->GetBoolField("result") &&
        Utility::TryGetObjectField(ServiceResponseObj, "values", Values);

}

bool RosBridge::Send(TSharedRef<FJsonObject> SentObj) const {
    return Utility::Send(Socket, SentObj);
}

bool RosBridge::Recv(TSharedPtr<FJsonObject>& RecvObj) const {
    return Utility::Recv(Socket, RecvObj);
}
