﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Attack.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// 생성자
	UBTTaskNode_Attack();

	// Execute
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	// Tick
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	// 현재 공격중인지 확인
	bool IsAttacking = false;
	
};
