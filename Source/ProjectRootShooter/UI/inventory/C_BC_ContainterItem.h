// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ORReplicatedObject.h"
#include "Math/IntPoint.h"
#include "C_BC_ContainterItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTROOTSHOOTER_API UC_BC_ContainterItem : public UORReplicatedObject
{
	GENERATED_BODY()
private:

    // 크기 정보 (2D IntPoint)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContainerItem", meta = (AllowPrivateAccess = "true"))
    FIntPoint Demention;

    // 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContainerItem", meta = (AllowPrivateAccess = "true"))
    FName Name;

    // 썸네일 머티리얼
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContainerItem", meta = (AllowPrivateAccess = "true"))
    UMaterialInterface* Thumbnail;

    // 회전된 썸네일 머티리얼
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContainerItem", meta = (AllowPrivateAccess = "true"))
    UMaterialInterface* ThumbnailRotated;

    // 컨테이너 행 배열
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContainerItem", meta = (AllowPrivateAccess = "true"))
    //TArray<FSContainerRow> ContainerRows;
	
};
