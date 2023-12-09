// 版权 海蓝之心

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

// AAuraPlayerController 类，继承自 APlayerController
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// 类构造函数
	AAuraPlayerController();

	// 重写的 PlayerTick 函数，每帧调用
	virtual void PlayerTick(float DeltaTime) override;

protected:
	// 重写的 BeginPlay 函数，游戏开始时调用
	virtual void BeginPlay() override;

	// 重写的 SetupInputComponent 函数，用于设置输入绑定
	virtual void SetupInputComponent() override;

private:
	// UPROPERTY 宏允许在 Unreal 编辑器中编辑此变量
	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputMappingContext> AuraContext; // 类型为 UInputMappingContext 的指针，命名为 AuraContext

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputAction> MoveAction; // 类型为 UInputAction 的指针，命名为 MoveAction

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

private:
	// 处理输入移动的函数
	void Move(const FInputActionValue& InputActionValue);

	// 光标追踪函数
	void CursorTrace(); 
};
