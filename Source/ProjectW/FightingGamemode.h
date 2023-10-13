// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MeshPlayerCharacter.h"
#include "FightingGamemode.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class AFightingGamemode : public AGameMode
{
	GENERATED_BODY()

	AFightingGamemode();

public:
	
	AMeshPlayerCharacter* player1Reference;
	AMeshPlayerCharacter* player2Reference;
};
