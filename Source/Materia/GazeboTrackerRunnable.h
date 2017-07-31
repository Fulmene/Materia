// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WindowsCriticalSection.h"

#include "RosBridge.h"
#include "RosMsgType.h"

/**
 * 
 */
class MATERIA_API GazeboTrackerRunnable : public FRunnable {

public:
    GazeboTrackerRunnable(const FString& InIpAddr, uint16 InPort, bool InUdp, const FString& InModelName, const FString& InRelativeEntityName);
    ~GazeboTrackerRunnable();

    bool Init() override;
    uint32 Run() override;
    void Stop() override;

    TSharedPtr<RosMsgType::GazeboMsgs::GetModelState::Values> GetState();

private:
    FRunnableThread* Thread;
    FCriticalSection StateSynchronize;
    FThreadSafeBool bRunning;

    TSharedPtr<RosBridge> Bridge;
    FString IpAddr;
    uint16 Port;
    bool bUdp;

    FString ModelName;
    FString RelativeEntityName;

    TSharedPtr<RosMsgType::GazeboMsgs::GetModelState::Values> State;

};
