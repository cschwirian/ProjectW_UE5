// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hitbox.h"
#include "GameFramework/Character.h"
#include "MeshPlayerCharacter.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FInputTuple
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString movementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString buttonComponent;

	bool operator==(const FInputTuple& otherInput) const
	{
		if (movementComponent.Compare(otherInput.movementComponent) == 0 && buttonComponent.Compare(otherInput.buttonComponent) == 0)
		{
			return true;
		}
		else if (movementComponent.Compare("*") == 0 && buttonComponent.Compare(otherInput.buttonComponent) == 0)
		{
			return true;
		}
		else if (otherInput.movementComponent.Compare("*") == 0 && buttonComponent.Compare(otherInput.buttonComponent) == 0)
		{
			return true;
		}
		else if (buttonComponent.Compare("*") == 0 && movementComponent.Compare(otherInput.movementComponent) == 0)
		{
			return true;
		}
		else if (otherInput.buttonComponent.Compare("*") == 0 && movementComponent.Compare(otherInput.movementComponent) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

USTRUCT(BlueprintType)
struct FInputInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FInputTuple input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float timeStamp;
};

USTRUCT(BlueprintType)
struct FCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputTuple> motionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FInputTuple buttonComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool hasBeenUsed;
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	STANDING,
	JUMPING,
	LANDING_LAG,
	ATTACKING,
	AIR_ATTACKING,
	DASHING,
	AIR_DASHING,
	BLOCKING,
	AIR_BLOCKING,
	STUNNED,
	AIR_STUNNED,
	CROUCHING,
	CROUCH_BLOCKING,
	DEAD

};

UENUM(BlueprintType)
enum class EDirectionalInput : uint8
{
	DEFAULT,
	HOLDING_LEFT,
	HOLDING_RIGHT

};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	MONK

};

UENUM(BlueprintType)
enum class EAttacks : uint8
{
	ATTACK1,
	CROUCH_ATTACK1,
	AIR_ATTACK1,
	ATTACK2,
	CROUCH_ATTACK2,
	AIR_ATTACK2,
	ATTACK3,
	CROUCH_ATTACK3,
	AIR_ATTACK3,
	ATTACK4,
	CROUCH_ATTACK4,
	AIR_ATTACK4

};


UCLASS()
class AMeshPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	AMeshPlayerCharacter();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float secondsPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float maxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float currentHitstunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float currentBlockstunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Reference")
	AMeshPlayerCharacter* otherPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterState characterState;

	UPROPERTY(BlueprintReadOnly, Category = "Commands")
	TArray<FCommand> commands;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EDirectionalInput directionalInput;

	void MoveLeft(float value);
	void MoveRight(float value);

	void Jump() override;
	void StopJumping() override;
	void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void StartCrouching();

	UFUNCTION(BlueprintCallable)
	void StopCrouching();

	UFUNCTION(BlueprintCallable)
	void StartBlocking();

	UFUNCTION(BlueprintCallable)
	void StopBlocking();

	UFUNCTION(BlueprintCallable)
	void CollidedWithProximityHitbox();

	UFUNCTION(BlueprintCallable)
	void PerformPushback(float pushbackAmount, float launchAmount, bool hasBlocked);

	UFUNCTION(BlueprintCallable)
	void BeginStun();

	UFUNCTION(BlueprintCallable)
	void EndStun();

	UFUNCTION(BlueprintCallable)
	void AddInputToBuffer(FInputInfo inputInfo);

	UFUNCTION(BlueprintCallable)
	void CheckInputBufferForCommand();

	UFUNCTION(BlueprintCallable)
	void StartCommand(FString commandName);

	float removeInputFromBufferTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputInfo> inputBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FQuat rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float currentDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float dashModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float dashDuration;

	float enemyLocationX;
	float characterLocationX;

	FCommand tempCommand;

	UFUNCTION(BlueprintCallable)
	float GetCurrentDistance();

	UPROPERTY(BlueprintReadOnly, Category = "Hurtbox")
	FTransform standingHurtbox;

	UPROPERTY(BlueprintReadOnly, Category = "Hurtbox")
	FTransform crouchingHurtbox;
};
