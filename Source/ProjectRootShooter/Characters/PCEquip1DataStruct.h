#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PCEquip1DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FPCEquip1DataStruct:public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* MeshAsset;
};