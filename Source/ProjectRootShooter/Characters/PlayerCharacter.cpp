#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "PCEquip1DataStruct.h"
#include "ProjectRootShooter/Utilities/InteractableInterface.h"


APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);

    Equip1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Equip1"));
    Equip1->SetupAttachment(GetMesh());

    // Movement 설정
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    // 카메라 오프셋 기본값
    DefaultCameraOffset = FVector(40.0f, 0.0f, 80.0f);
    AimingCameraOffset = FVector(120.0f, 80.0f, 30.0f); // 오른쪽으로 오프셋    

    // 스탯 초기값
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    MaxStamina = 100.0f;
    CurrentStamina = MaxStamina;

    MaxCarryWeight = 50.0f;
    CurrentCarryWeight = 0.0f;

    WalkSpeed = 300.0f;
    RunSpeed = 600.0f;

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 상태 UI 위젯 생성 및 화면에 표시
    if (StatusWidgetClass)
    {
        StatusWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), StatusWidgetClass);
        if (StatusWidgetInstance)
        {
            StatusWidgetInstance->AddToViewport();
        }
    }

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    Equip1DataTable.LoadSynchronous();
    ChangeEquip1(1);

    OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAim);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAim);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInput->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::StartRun);
        EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopRun);
        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
    }
}

void APlayerCharacter::StartRun()
{
    GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::StopRun()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D Input = Value.Get<FVector2D>();

    if (Controller && (Input != FVector2D::ZeroVector))
    {
        const FRotator YawRot(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, Input.Y);
        AddMovementInput(Right, Input.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookInput = Value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartAim()
{
    bIsAiming = true;
    UpdateCameraForAim(bIsAiming);

    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APlayerCharacter::StopAim()
{
    bIsAiming = false;
    UpdateCameraForAim(bIsAiming);

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter::UpdateCameraForAim(bool bIsAimingNow)
{
    FVector NewOffset = bIsAimingNow ? AimingCameraOffset : DefaultCameraOffset;
    CameraComp->SetRelativeLocation(NewOffset);
}

void APlayerCharacter::ChangeEquip1(int32 EquipIndex)
{
    Equip1_index = EquipIndex;

    if (!Equip1DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("Equip1DataTable이 비어있음"));
        return;
    }

    static const FString ContextString(TEXT("Equip1Context"));
    TArray<FName> RowNames = Equip1DataTable->GetRowNames();

    if (!RowNames.IsValidIndex(EquipIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("Equip1DataTable에 유효하지 않은 인덱스: %d"), EquipIndex);
        return;
    }

    FName RowName = RowNames[EquipIndex];
    FPCEquip1DataStruct* EquipData = Equip1DataTable->FindRow<FPCEquip1DataStruct>(RowName, ContextString);

    if (EquipData && EquipData->MeshAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("장비 변경 성공: %s"), *EquipData->name.ToString());

        Equip1->SetSkeletalMesh(EquipData->MeshAsset);
        Equip1->SetLeaderPoseComponent(GetMesh());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("장비 데이터가 없거나 MeshAsset이 null임"));
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 달리는 중이면 스태미나 감소
    if (GetCharacterMovement()->MaxWalkSpeed == RunSpeed && !FMath::IsNearlyZero(CurrentStamina))
    {
        const float StaminaDrainRate = 15.0f; // 초당 감소량 (원하는 값으로 조절)
        CurrentStamina = FMath::Clamp(CurrentStamina - (StaminaDrainRate * DeltaTime), 0.0f, MaxStamina);

        // 스태미나가 바닥나면 자동으로 걷기 속도로 전환
        if (FMath::IsNearlyZero(CurrentStamina))
        {
            StopRun();
        }
    }
    // 달리지 않는 경우에는 스태미나 회복 (선택사항)
    else if (CurrentStamina < MaxStamina)
    {
        const float StaminaRegenRate = 10.0f;
        CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.0f, MaxStamina);
    }
}

void APlayerCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
    {
        IInteractableInterface::Execute_Interact(OtherActor, this);
        ChangeEquip1(1);
    }
}

void APlayerCharacter::ToggleInventory()
{
    APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC) return;

    if (bIsInventoryOpen)
    {
        // 닫기
        if (InventoryWidgetInstance)
        {
            InventoryWidgetInstance->RemoveFromParent();
            InventoryWidgetInstance = nullptr;
        }

        PC->bShowMouseCursor = false;
        PC->SetIgnoreLookInput(false);
        PC->SetInputMode(FInputModeGameOnly());

        bIsInventoryOpen = false;

        // 상태 UI 다시 켜기
        if (StatusWidgetInstance)
        {
            StatusWidgetInstance->AddToViewport();
        }
    }
    else
    {
        // 열기
        if (InventoryWidgetClass)
        {
            InventoryWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
            if (InventoryWidgetInstance)
            {
                InventoryWidgetInstance->AddToViewport();
            }
        }

        PC->bShowMouseCursor = true;
        PC->SetIgnoreLookInput(true);

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        InputMode.SetWidgetToFocus(InventoryWidgetInstance->TakeWidget());

        PC->SetInputMode(InputMode);

        bIsInventoryOpen = true;

        // 상태 UI 끄기
        if (StatusWidgetInstance)
        {
            StatusWidgetInstance->RemoveFromParent();
        }
    }
}