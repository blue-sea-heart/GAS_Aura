// Copyright 海蓝之心


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	//以下是topdown游戏视角基本操作
	GetCharacterMovement()->bOrientRotationToMovement=true;//将角色向加速方向旋转
	GetCharacterMovement()->RotationRate=FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane=true;//运动将被限制在一个平面内
	GetCharacterMovement()->bSnapToPlaneAtStart=true;//如果为true并且平面约束被启用，那么更新后的组件将在第一次附加时被固定到平面上。

	bUseControllerRotationPitch=false;
	bUseControllerRotationRoll=false;
	bUseControllerRotationYaw=false;
}
