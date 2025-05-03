#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTROOTSHOOTER_API IInteractableInterface
{
	GENERATED_BODY()

public:
	// 인터페이스 함수는 pure virtual이 아니어야 하며, _Implementation을 오버라이드할 수 있어야 함
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* Interactor);
};