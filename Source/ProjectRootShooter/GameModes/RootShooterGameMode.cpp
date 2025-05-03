// Fill out your copyright notice in the Description page of Project Settings.


#include "RootShooterGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectRootShooter/Characters/PlayerCharacter.h"

ARootShooterGameMode::ARootShooterGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerCharacter> PlayerPawnBPClass(TEXT("/Game/Character/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("DefaultPawnClass Set To: %s"), *DefaultPawnClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find BP_PlayerCharacter class!"));
	}
}

