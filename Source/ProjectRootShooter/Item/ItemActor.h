// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ORReplicatedObject.h"
#include "GameFramework/Actor.h"
#include "ProjectRootShooter/Utilities/InteractableInterface.h"
#include "ItemActor.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Equipment,
    Weapon,
    Consumable,
    // 등등
};

UCLASS(BlueprintType)
class UItemBase : public  UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint SizeInInventory; // 테트리스 용 크기
};

UCLASS()
class PROJECTROOTSHOOTER_API AItemActor : public AActor
, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMeshComponent* ItemMesh;

	 // 상호작용 인터페이스 구현
     virtual void Interact_Implementation(AActor* Interactor) override;

};
