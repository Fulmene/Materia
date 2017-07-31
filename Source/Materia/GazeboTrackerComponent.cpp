// Fill out your copyright notice in the Description page of Project Settings.


#include "GazeboTrackerComponent.h"

#include "Gazebo.h"

// Sets default values for this component's properties
UGazeboTrackerComponent::UGazeboTrackerComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
    IPAddress = RosBridge::DefaultIpAddr;
    Port = RosBridge::DefaultPort;
    bUseUDP = true;
}


// Called when the game starts
void UGazeboTrackerComponent::BeginPlay() {
    Super::BeginPlay();

    // ...

    GazeboTracker = MakeShareable(new GazeboTrackerRunnable(IPAddress, Port, bUseUDP, ModelName, RelativeEntityName));
}


// Called every frame
void UGazeboTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...

    auto State = GazeboTracker->GetState();
    if (State.IsValid())
        GetOwner()->SetActorLocationAndRotation(
                State->Pose.Position,
                State->Pose.Orientation,
                false,
                nullptr,
                ETeleportType::TeleportPhysics
                );

}
