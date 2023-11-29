// Copyright 海蓝之心


#include "Player/AuraPlayerController.h"

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
