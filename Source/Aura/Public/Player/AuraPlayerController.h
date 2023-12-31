// Copyright 海蓝之心

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputMappingContext>AuraContext;//用新模版定义一个名为AuraContext，类型为UInputMappingContext的指针

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction>MoveAction;//用新模版定义一个名为MoveAction，类型为UInputAction的指针

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
private:
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();//跟踪鼠标
};
