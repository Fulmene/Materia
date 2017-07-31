// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "GazeboTrackerComponent.h"
#include "ImageStreamerComponent.h"

#include "MainActor.generated.h"

UCLASS()
class MATERIA_API AMainActor : public AActor {

    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AMainActor(const FObjectInitializer& ObjectInitializer);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UGazeboTrackerComponent* GazeboTrackerComponent;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    USceneCaptureComponent2D* SceneCaptureComponent;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UImageStreamerComponent* ImageStreamerComponent;

};
