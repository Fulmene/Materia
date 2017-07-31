// Fill out your copyright notice in the Description page of Project Settings.


#include "MainActor.h"


// Sets default values
AMainActor::AMainActor(const FObjectInitializer& ObjectInitializer) {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Component"));

    if (RootComponent == nullptr)
        RootComponent = StaticMeshComponent;
    else
        StaticMeshComponent->SetupAttachment(RootComponent);

    GazeboTrackerComponent = CreateDefaultSubobject<UGazeboTrackerComponent>(TEXT("Gazebo Tracker Component"));

    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component"));
    SceneCaptureComponent->SetupAttachment(RootComponent);

    ImageStreamerComponent = CreateDefaultSubobject<UImageStreamerComponent>(TEXT("Image Streamer Component"));

}

// Called when the game starts or when spawned
void AMainActor::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void AMainActor::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}
