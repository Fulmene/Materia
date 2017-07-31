// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeboTrackerRunnable.h"

#include "Gazebo.h"

GazeboTrackerRunnable::GazeboTrackerRunnable(const FString& InIpAddr, uint16 InPort, bool InUdp, const FString& InModelName, const FString& InRelativeEntityName) {
    IpAddr = InIpAddr;
    Port = InPort;
    bUdp = InUdp;
    ModelName = InModelName;
    RelativeEntityName = InRelativeEntityName;
    Thread = FRunnableThread::Create(this, TEXT("Gazebo Model State Thread"));
}

GazeboTrackerRunnable::~GazeboTrackerRunnable() {
    Thread->Kill();
    delete Thread;
    Thread = nullptr;
}

bool GazeboTrackerRunnable::Init() {
    Bridge = MakeShareable(new RosBridge(IpAddr, Port, true));
    bRunning = true;
    return true;
}

uint32 GazeboTrackerRunnable::Run() {
    while (bRunning) {
        TSharedPtr<RosMsgType::GazeboMsgs::GetModelState::Values> CurrentState = MakeShareable(new RosMsgType::GazeboMsgs::GetModelState::Values());
        if (Gazebo::Service::GetModelState(Bridge, ModelName, RelativeEntityName, *CurrentState)) {
            FScopeLock Lock(&StateSynchronize);
            State = CurrentState;
        }
        FPlatformProcess::Sleep(1.0 / 60);
    }
    return 0;
}

void GazeboTrackerRunnable::Stop() {
    bRunning = false;
}

TSharedPtr<RosMsgType::GazeboMsgs::GetModelState::Values> GazeboTrackerRunnable::GetState() {
    FScopeLock Lock(&StateSynchronize);
    return State;
}
