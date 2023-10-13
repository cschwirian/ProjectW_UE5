// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hitbox.generated.h"

UENUM(BlueprintType)
enum class EHitboxType : uint8
{
	PROXIMITY,
	STRIKE,
	THROW,
	COMMAND_THROW,
	HURTBOX
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	LOW,
	MID,
	HIGH,
	OVERHEAD

};

UCLASS()
class AHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitbox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	EHitboxType hitboxType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	EAttackType attackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	float hitboxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	float hitboxHitstunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	float hitboxBlockstunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	FVector hitboxOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true, InstanceEditable = true), Category = "Hitbox")
	FVector hitboxScale;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
