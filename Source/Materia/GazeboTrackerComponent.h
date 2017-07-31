// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "RosBridge.h"
#include "GazeboTrackerRunnable.h"

#include "GazeboTrackerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MATERIA_API UGazeboTrackerComponent : public UActorComponent {
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UGazeboTrackerComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge")
    FString IPAddress;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge")
    int32 Port;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge")
    bool bUseUDP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gazebo Model")
    FString ModelName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gazebo Model")
    FString RelativeEntityName;

protected:

    TSharedPtr<GazeboTrackerRunnable> GazeboTracker;
};
