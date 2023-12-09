// 版权 海蓝之心

#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interface/EnemyInterface.h"

// 构造函数实现
AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true; // 在服务器上开启复制，确保多玩家游戏中的一致性
}

// PlayerTick 函数每帧调用
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime); // 调用基类实现
    CursorTrace(); // 调用光标追踪函数
}

// BeginPlay 函数在游戏开始时调用
void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay(); // 调用基类实现
    check(AuraContext); // 检查 AuraContext 是否为空，如果为空则崩溃（用于调试）

    // 获取增强输入本地玩家子系统
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(Subsystem); // 检查 Subsystem 是否为空，如果为空则崩溃（用于调试）

    Subsystem->AddMappingContext(AuraContext, 0); // 将 AuraContext 添加到输入子系统中，优先级为 0

    bShowMouseCursor = true; // 显示鼠标光标
    DefaultMouseCursor = EMouseCursor::Default; // 设置默认鼠标光标样式

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 设置鼠标锁定模式为不锁定
    InputModeData.SetHideCursorDuringCapture(false); // 设置鼠标隐藏模式为不隐藏
    SetInputMode(InputModeData); // 应用鼠标模式
}

// SetupInputComponent 函数用于设置输入绑定
void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent(); // 调用基类实现

    // 将 InputComponent 转换为 UEnhancedInputComponent 类型，如果转换失败则崩溃
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    // 将 MoveAction 绑定到 Move 函数
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

// Move 函数响应移动输入
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    // 从 InputActionValue 提取输入向量
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation(); // 获取控制器的旋转
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // 获取 Yaw 方向的旋转

    // 计算前进和右侧方向向量
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // 对受控制的 Pawn 应用移动输入
    if (APawn* ControllerPawn = GetPawn<APawn>())
    {
        ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

// CursorTrace 函数执行来自鼠标光标的射线追踪
void AAuraPlayerController::CursorTrace()
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

    // 根据射线追踪结果的不同情况处理
    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            // 情况 2：首次将光标悬停在此 actor 上
            ThisActor->HighlightActor();
        }
        // 情况 1：光标未悬停在敌人上
    }
    else
    {
        if (ThisActor == nullptr)
        {
            // 情况 3：光标从敌人上移开
            LastActor->UnHighlightActor();
        }
        else
        {
            if (LastActor == ThisActor)
            {
                // 情况 4：从一个敌人上移动到另一个敌人上
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
            // 情况 5：光标悬停在同一敌人上
        }
    }
}
