#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext; 
class UInputAction;
class UUserWidget;

UCLASS()
class PROJECTROOTSHOOTER_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
    virtual void BeginPlay() override;

    // 입력 함수
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartAim();
    void StopAim();
    void UpdateCameraForAim(bool bIsAimingNow);
    void StartRun();
    void StopRun();

    void ToggleInventory();

    // 장비 변경 함수
	UFUNCTION(BlueprintCallable, Category = Equipment)
	void ChangeEquip1(int32 EquipIndex);

    // 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
	USkeletalMeshComponent* Equip1;

    // 입력 액션
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputAction* AimAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputAction* RunAction;

    // 인벤토리 토글 액션
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
    UInputAction* ToggleInventoryAction;

    // 카메라 위치 조정용
    FVector DefaultCameraOffset;
    FVector AimingCameraOffset;

    // 조준 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
    bool bIsAiming;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip")
    int Equip1_index;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TSoftObjectPtr<UDataTable> Equip1DataTable;

    UFUNCTION()
    void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    int32 OverlappingEquipIndex = -1;//임시

    // 캐릭터 스탯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
    float CurrentHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float MaxStamina = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
    float CurrentStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float MaxCarryWeight = 50.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
    float CurrentCarryWeight = 0.0f;

    // 걷기 / 달리기 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RunSpeed = 600.0f;

    // 상태 HUD 위젯
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> StatusWidgetClass;

    UPROPERTY()
    UUserWidget* StatusWidgetInstance;

    // 인벤토리 HUD 위젯
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    UPROPERTY()
    UUserWidget* InventoryWidgetInstance;

    bool bIsInventoryOpen = false;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;
    
};
