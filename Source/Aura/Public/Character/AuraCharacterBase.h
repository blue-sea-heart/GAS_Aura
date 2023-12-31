// Copyright 海蓝之心

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();

protected:

	virtual void BeginPlay() override;

	//一个骨骼网格体组件（USkeletalMeshComponent）类型的武器（Weapon）的指针
	UPROPERTY(EditAnywhere,Category=Combat)
	TObjectPtr<USkeletalMeshComponent>Weapon;//TObjectPtr 是一种指向 UObject 的指针类型,新的标准推荐使用这种指针代替原始指针
};
