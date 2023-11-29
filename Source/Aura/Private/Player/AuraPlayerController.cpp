// Copyright 海蓝之心


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates=true;//在服务器上更新数据并将其发送到客户端,确保多玩家游戏中的一致性
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);//检查AuraContext是否是空指针,如果空指针则崩溃,这是一种调试时的错误检查手段。

	UEnhancedInputLocalPlayerSubsystem* Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext,0);//将AuraContext添加到输入子系统中，优先级为0

	bShowMouseCursor=true;//显示鼠标指针
	DefaultMouseCursor=EMouseCursor::Default;//设置默认鼠标光标样式

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);//设置鼠标锁定模式为：不锁定
	InputModeData.SetHideCursorDuringCapture(false);//设置鼠标隐藏模式为不隐藏
	SetInputMode(InputModeData);//应用鼠标模式
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//把InputComponent转换为UEnhancedInputComponent类型，转换失败则崩溃
	UEnhancedInputComponent* EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(InputComponent);

	//绑定到AAuraPlayerController::Move函数，ETriggerEvent::Triggered: 这是一个枚举，指定当动作被触发时执行绑定的函数。
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//InputActionValue.Get<FVector2D>(): 这个函数从InputActionValue对象中获取类型为FVector2D的数据。InputActionValue是一个封装了输入数据的对象，可以是按钮按下、摇杆移动等。
	const FVector2D InputAxisVector=InputActionValue.Get<FVector2D>();
	const FRotator Rotation=GetControlRotation();//获取控制器的旋转Rotation
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);//获取yaw方向的旋转

	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//通过矩阵运算获得向前的方向向量
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//通过矩阵运算获得向右的方向向量

	if (APawn* ControllerPawn=GetPawn<APawn>())//获取控制器当前控制的Pawn实例,并且判断是否是空指针
	{
		ControllerPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection,InputAxisVector.X);

	}
}
