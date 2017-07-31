// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RosMsgType.h"
#include "RosBridge.h"

/**
 * 
 */
class MATERIA_API Gazebo {

public:

    class Service {

    public:

        static bool GetModelState(
                const TSharedPtr<RosBridge> Bridge,
                const FString& ModelName,
                const FString& RelativeEntityName,
                RosMsgType::GazeboMsgs::GetModelState::Values& Values);

    };

};
