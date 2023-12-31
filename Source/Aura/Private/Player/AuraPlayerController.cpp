// Copyright 海蓝之心


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interface/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates=true;//在服务器上更新数据并将其发送到客户端,确保多玩家游戏中的一致性
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit)return;

	LastActor=ThisActor;
	ThisActor=Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * 从鼠标光标的射线追踪有以下几种情况：
	 * 1、LastActor is null && ThisActor is null
	 *    -说明鼠标光标没有在敌人身上
	 *    -不做任何操作
	 * 2、LastActor is null && ThisActor is valid
	 *    -意味着我们第一次将鼠标悬停在这个actor上
	 *    -高亮 ThisActor
	 * 3、LastActor is valid && ThisActor is valid
	 *    -说明上一帧鼠标光标在敌人上，但是这一帧不在
	 *    -取消高亮LastActor
	 * 4、Both actor are valid,but LastActor!=ThisActor
	 *    -意味着我们盘旋在一个敌人上方，然后突然我们开始盘旋在另一个敌人上方
	 *    -取消高亮LastActor,高亮 ThisActor
	 * 5、Both actor are valid,and they are same actor
	 *    -意味着我们已经突出显示了它
	 *    -不做任何操作
	 */
	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
		{
			//case2
			ThisActor->HighlightActor();
		}
		else
		{
			//case1
		}
	}
	else
	{
		if (ThisActor==nullptr)
		{
			//case3
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor==ThisActor)
			{
				//case4
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//case5
			}
		}
	}
}
