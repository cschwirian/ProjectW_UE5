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

	TArray<FInputTuple> commandInputs = { FInputTuple{"Down", "*"}, FInputTuple{"DownRight", "*"}, FInputTuple{"Right", "*"} };

	tempCommand = FCommand();
	tempCommand.name = "HADOUKEN";
	tempCommand.motionComponent = commandInputs;
	tempCommand.buttonComponent = FInputTuple{ "*", "A1" };
	tempCommand.hasBeenUsed = false;

	commands.SetNum(2);

	commands[0] = tempCommand;

	commands[1].name = "SHORYUKEN";
	commands[1].motionComponent.Add(FInputTuple{ "Right", "" });
	commands[1].motionComponent.Add(FInputTuple{ "Down", "" });
	commands[1].motionComponent.Add(FInputTuple{ "DownRight", "*" });
	commands[1].buttonComponent = FInputTuple{ "*", "A1" };
	commands[1].hasBeenUsed = false;

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
	int correctSequenceCounter = 0;
	FInputTuple previousInput = FInputTuple{};
	FInputTuple noButton = FInputTuple{ "*", "" };
	bool motionCompleted = false;

	for (int commandIndex = 0; commandIndex < commands.Num(); commandIndex++)
	{
		for (int commandInput = 0; commandInput < commands[commandIndex].motionComponent.Num(); commandInput++)
		{
			for (int input = 0; input < inputBuffer.Num(); input++)
			{
				if (input + correctSequenceCounter < inputBuffer.Num())
				{
					if (motionCompleted)
					{
						if (inputBuffer[input + correctSequenceCounter].input == commands[commandIndex].buttonComponent)
						{
							StartCommand(commands[commandIndex].name);
							commands[commandIndex].hasBeenUsed = true;
							return;
						}
						else if (inputBuffer[input + correctSequenceCounter].input == noButton)
						{
							continue;
						}
						else
						{
							motionCompleted = false;
							correctSequenceCounter = 0;
							previousInput = FInputTuple{};
						}
					}
					else if (inputBuffer[input + correctSequenceCounter].input == commands[commandIndex].motionComponent[commandInput])
					{
						previousInput = inputBuffer[input + correctSequenceCounter].input;

						//UE_LOG(LogTemp, Warning, TEXT("yujhhhhh"));
						correctSequenceCounter += 1;

						if (correctSequenceCounter == commands[commandIndex].motionComponent.Num())
						{
							UE_LOG(LogTemp, Warning, TEXT("hmmmm"));
							motionCompleted = true;
							//StartCommand(tempCommand.name);
						}

						// Check to see if the last part of the motion also contains the button.
						if (motionCompleted)
						{
							if (inputBuffer[input + correctSequenceCounter - 1].input == commands[commandIndex].buttonComponent)
							{
								StartCommand(commands[commandIndex].name);
								commands[commandIndex].hasBeenUsed = true;
								return;
							}
							else if (inputBuffer[input + correctSequenceCounter - 1].input == noButton)
							{
								continue;
							}
							else
							{
								motionCompleted = false;
								correctSequenceCounter = 0;
								previousInput = FInputTuple{};
								continue;
							}
						}

						break;
					}
					else if (inputBuffer[input + correctSequenceCounter].input == previousInput)
					{
						continue;
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("naaahhh"));
						motionCompleted = false;
						correctSequenceCounter = 0;
						previousInput = FInputTuple{};
					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("not yet"));
					motionCompleted = false;
					correctSequenceCounter = 0;
					previousInput = FInputTuple{};
				}
			}
		}
	}
}

void AMeshPlayerCharacter::StartCommand(FString commandName)
{
	if (commandName == "HADOUKEN")
	{
		UE_LOG(LogTemp, Warning, TEXT("HADOUKEN!!!"));
	}
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