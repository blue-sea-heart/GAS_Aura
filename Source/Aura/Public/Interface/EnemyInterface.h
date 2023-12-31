// Copyright 海蓝之心

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor()=0;//高亮鼠标所在的actor的纯虚函数
	virtual void UnHighlightActor()=0;//取消高亮鼠标所在的actor的纯虚函数

};
