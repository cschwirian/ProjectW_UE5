// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshPlayerCharacter.h"
#include "FightingGamemode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMeshPlayerCharacter::AMeshPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	otherPlayer = nullptr;
	characterState = ECharacterState::STANDING;
	rotation = FQuat(0.0f, 0.0f, 0.0f, 0.0f);

	characterLocationX = 0.0f;
	enemyLocationX = 0.0f;

	currentDistance = 0.0f;
	maxDistance = 800.0f;

	health = 1000.0f;
	maxHealth = 1000.0f;
	currentHitstunFrames = 0.0f;
	currentBlockstunFrames = 0.0f;
	removeInputFromBufferTime = 2.0f;

	dashDuration = 0.2f;
	dashModifier = 3000.0f;

	directionalInput = EDirectionalInput::DEFAULT;

	secondsPerTick = 0.01666666666;
	
	TArray<FInputTuple> facingLeft;
	TArray<FInputTuple> facingRight;

	commands.SetNum(4);

	commands[0].name = "HADOUKEN";
	facingLeft.Add(FInputTuple{ "Down", "*" });
	facingLeft.Add(FInputTuple{ "DownLeft", "*" });
	facingLeft.Add(FInputTuple{ "Left", "*" });
	facingLeft.Add(FInputTuple{ "*", "A1" });
	//commands[0].facingLeft.Add(FInputTuple{ "Down", "*" });
	//commands[0].facingLeft.Add(FInputTuple{ "DownLeft", "*" });
	//commands[0].facingLeft.Add(FInputTuple{ "Left", "*" });
	//commands[0].facingLeft.Add(FInputTuple{ "*", "A1" });
	commands[0].facingLeft = facingLeft;
	UE_LOG(LogTemp, Warning, TEXT("Left: %d"), commands[0].facingLeft.Num());
	

	commands[0].facingRight.Add(FInputTuple{ "Down", "*" });
	commands[0].facingRight.Add(FInputTuple{ "DownRight", "*" });
	commands[0].facingRight.Add(FInputTuple{ "Right", "*" });
	commands[0].facingRight.Add(FInputTuple{ "*", "A1" });
	commands[0].hasBeenUsed = false;


	commands[1].name = "SHORYUKEN";
	commands[1].facingLeft.Add(FInputTuple{ "Left", "" });
	commands[1].facingLeft.Add(FInputTuple{ "*", "*" });
	commands[1].facingLeft.Add(FInputTuple{ "Down", "" });
	commands[1].facingLeft.Add(FInputTuple{ "DownLeft", "" });
	commands[1].facingLeft.Add(FInputTuple{ "*", "A1" });

	commands[1].facingRight.Add(FInputTuple{"Right", ""});
	commands[1].facingRight.Add(FInputTuple{ "*", "*" });
	commands[1].facingRight.Add(FInputTuple{ "Down", "" });
	commands[1].facingRight.Add(FInputTuple{ "DownRight", "" });
	commands[1].facingRight.Add(FInputTuple{ "*", "A1" });
	commands[1].hasBeenUsed = false;


	commands[2].name = "DASH_BACKWARD";
	commands[2].facingLeft.Add(FInputTuple{ "Right", "*" });
	commands[2].facingLeft.Add(FInputTuple{ "N", "*" });
	commands[2].facingLeft.Add(FInputTuple{ "Right", "*" });

	commands[2].facingRight.Add(FInputTuple{ "Left", "*" });
	commands[2].facingRight.Add(FInputTuple{ "N", "*" });
	commands[2].facingRight.Add(FInputTuple{ "Left", "*" });
	commands[2].hasBeenUsed = false;


	commands[3].name = "DASH_FORWARD";

	commands[3].facingLeft.Add(FInputTuple{ "Left", "*" });
	commands[3].facingLeft.Add(FInputTuple{ "N", "*" });
	commands[3].facingLeft.Add(FInputTuple{ "Left", "*" });

	commands[3].facingRight.Add(FInputTuple{ "Right", "*" });
	commands[3].facingRight.Add(FInputTuple{ "N", "*" });
	commands[3].facingRight.Add(FInputTuple{ "Right", "*" });
	commands[3].hasBeenUsed = false;

}

// Called when the game starts or when spawned
void AMeshPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMeshPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (otherPlayer)
	//{
	//	enemyLocationX = otherPlayer->GetActorLocation().X;
	//	characterLocationX = this->GetActorLocation().X;
	//	if (enemyLocationX > characterLocationX)
	//	{
	//		if (GEngine)
	//		{
	//			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	//		}
	//		rotation.Z = 0.0;
	//		this->SetActorRotation(rotation);
	//	}
	//	else
	//	{
	//		rotation.Z = 180.0;
	//		this->SetActorRotation(rotation);
	//	}
	//	currentDistance = abs(enemyLocationX - characterLocationX);
	//}

}

// Called to bind functionality to input
//void AMeshPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
	//PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMeshPlayerCharacter::MoveLeft);

//}

void AMeshPlayerCharacter::MoveLeft(float value)
{

}

void AMeshPlayerCharacter::MoveRight(float value)
{

}

void AMeshPlayerCharacter::Jump()
{
	Super::Jump();
}

void AMeshPlayerCharacter::StopJumping()
{

}
void AMeshPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void AMeshPlayerCharacter::StartCrouching()
{

}

void AMeshPlayerCharacter::StopCrouching()
{

}

void AMeshPlayerCharacter::StartBlocking()
{

}

void AMeshPlayerCharacter::StopBlocking()
{

}

void AMeshPlayerCharacter::CollidedWithProximityHitbox()
{
	FVector forwardVector = GetActorForwardVector();

	if (characterState != ECharacterState::DEAD)
	{
		if (characterState == ECharacterState::CROUCHING)
		{
			if (forwardVector.X > 0 and directionalInput == EDirectionalInput::HOLDING_LEFT)
			{
				characterState = ECharacterState::CROUCH_BLOCKING;
			}
			else if (forwardVector.X < 0 and directionalInput == EDirectionalInput::HOLDING_RIGHT)
			{
				characterState = ECharacterState::CROUCH_BLOCKING;
			}
		}
		else if (characterState == ECharacterState::STANDING)
		{
			if (forwardVector.X > 0 and directionalInput == EDirectionalInput::HOLDING_LEFT)
			{
				characterState = ECharacterState::BLOCKING;
			}
			else if (forwardVector.X < 0 and directionalInput == EDirectionalInput::HOLDING_RIGHT)
			{
				characterState = ECharacterState::BLOCKING;
			}
		}
	}
}

void AMeshPlayerCharacter::PerformPushback(float pushbackAmount, float launchAmount, bool hasBlocked)
{

}

void AMeshPlayerCharacter::BeginStun()
{

}

void AMeshPlayerCharacter::EndStun()
{

}

void AMeshPlayerCharacter::AddInputToBuffer(FInputInfo input)
{
	inputBuffer.Add(input);
}

void AMeshPlayerCharacter::CheckInputBufferForCommand()
{
	try {

		int correctSequenceCounter = 0;
		FInputTuple previousInput = FInputTuple{};
		TArray<FInputTuple> currentCommand;
		float forwardVector = GetActorForwardVector().X;

		for (int commandIndex = 0; commandIndex < commands.Num(); commandIndex++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Commands Size: %d"), commands.Num());
			if (commands[commandIndex].hasBeenUsed)
			{
				continue;
			}
			UE_LOG(LogTemp, Warning, TEXT("Index: %d"), commandIndex);
			UE_LOG(LogTemp, Warning, TEXT("Left: %d"), commands[commandIndex].facingLeft.Num());
			UE_LOG(LogTemp, Warning, TEXT("Right: %d"), commands[commandIndex].facingRight.Num());

			currentCommand = commands[commandIndex].facingLeft;

			if (forwardVector > 0)
			{
				currentCommand = commands[commandIndex].facingRight;
			}

			//UE_LOG(LogTemp, Warning, TEXT("%s"), *currentCommand.name)
			for (int input = 0; input < inputBuffer.Num(); input++)
			{
				inputBuffer[input].wasEvaluated = false;
			}
			UE_LOG(LogTemp, Warning, TEXT("hmm"));

			correctSequenceCounter = 0;
			previousInput = FInputTuple{ "Not", "Not" };
			return;

			for (int input = 0; input < inputBuffer.Num(); input++)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d"), currentCommand.Num());
				return;
				if (!inputBuffer[input].wasEvaluated && inputBuffer[input].input == previousInput)
				{
					inputBuffer[input].wasEvaluated = true;
					continue;
				}
				else if (!inputBuffer[input].wasEvaluated && inputBuffer[input].input == currentCommand[correctSequenceCounter])
				{
					previousInput = inputBuffer[input].input;

					//UE_LOG(LogTemp, Warning, TEXT("%s"), *(previousInput.toString() + " " + commands[commandIndex].name));
					inputBuffer[input].wasEvaluated = true;
					correctSequenceCounter += 1;

					if (correctSequenceCounter == currentCommand.Num())
					{
						StartCommand(commands[commandIndex].name);
						commands[commandIndex].hasBeenUsed = true;
						correctSequenceCounter = 0;
						return;
					}
				}
				else
				{

					//UE_LOG(LogTemp, Warning, TEXT("naaahhh"));
					correctSequenceCounter = 0;
					previousInput = FInputTuple{ "Not", "Not" };
					inputBuffer[input].wasEvaluated = true;
				}
			}
		}
	}
	catch (...)
	{
		
	}
}

void AMeshPlayerCharacter::StartCommand(FString commandName)
{
	UE_LOG(LogTemp, Warning, TEXT("executing command: %s"), *commandName);
}

float AMeshPlayerCharacter::GetCurrentDistance()
{
	if (otherPlayer)
	{
		if (auto characterMovement = GetCharacterMovement())
		{
			if (auto enemyMovement = otherPlayer->GetCharacterMovement())
			{
				characterLocationX = characterMovement->GetActorLocation().X;
				enemyLocationX = enemyMovement->GetActorLocation().X;

				return abs(enemyLocationX - characterLocationX);
			}
		}

	}

	return 0.0f;
}