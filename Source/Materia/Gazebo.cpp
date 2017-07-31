// Fill out your copyright notice in the Description page of Project Settings.


#include "Gazebo.h"
#include "Json.h"

bool Gazebo::Service::GetModelState(
        const TSharedPtr<RosBridge> Bridge,
        const FString& ModelName,
        const FString& RelativeEntityName,
        RosMsgType::GazeboMsgs::GetModelState::Values& Values
        ) {
    TSharedPtr<FJsonObject> ModelState = MakeShareable(new FJsonObject());
    return
        Bridge->CallService(
                "/gazebo/get_model_state",
                RosMsgType::ToJson(
                    RosMsgType::GazeboMsgs::GetModelState::Args(
                        ModelName, RelativeEntityName
                        )
                    ),
                ModelState
                ) &&
        ModelState.IsValid() &&
        RosMsgType::FromJson(
                ModelState,
                Values
                );
}
