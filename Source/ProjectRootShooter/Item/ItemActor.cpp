// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "ProjectRootShooter/Characters/PlayerCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::Interact_Implementation(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("ItemActor: Interacted!"));

    // 여기서 아이템 로직을 처리 (예: 캐릭터 인벤토리에 추가)
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor))
    {
        // 예시 로그
        UE_LOG(LogTemp, Log, TEXT("Item picked up by: %s"), *Player->GetName());

        // TODO: 인벤토리나 장비 시스템에 아이템 추가

        // 아이템 제거
        Destroy();
    }
}

