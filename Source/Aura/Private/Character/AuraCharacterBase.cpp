// Copyright 海蓝之心


#include "Character/AuraCharacterBase.h"


AAuraCharacterBase::AAuraCharacterBase()
{
 
	PrimaryActorTick.bCanEverTick = false;

	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");//初始化武器
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));//我们将在其中一只手上放置一个插槽Socket，并将我们的武器连接到该Socket上。
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);//武器不应有任何碰撞，设置武器武器碰撞为没有碰撞

	
}


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}



