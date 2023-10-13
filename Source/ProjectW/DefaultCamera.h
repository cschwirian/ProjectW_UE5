// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FightingGamemode.h"
#include "DefaultCamera.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class ADefaultCamera : public ACameraActor
{
	GENERATED_BODY()

	// Sets default values for this character's properties
	ADefaultCamera();

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
