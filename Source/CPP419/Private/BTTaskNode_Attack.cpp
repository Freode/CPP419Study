﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "BTTaskNode_Attack.h"
#include "ABAIController.h"
#include "ABCharacter.h"

// 생성자
UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	// Tick 활성화 - FinishLatentTask 호출하기 위함
	bNotifyTick = true;
	// 현재 공격하지 않은 상태로 설정
	IsAttacking = false;
}

// Execute
EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 캐릭터 객체 캐스팅
	auto ABCharacter = Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ABCharacter)
	{
		return EBTNodeResult::Failed;
	}

	// AI NPC 캐릭터가 공격하도록 설정
	ABCharacter->Attack();
	IsAttacking = true;

	// 공격 상태가 종료되면, 공격하지 않은 상태로 변경
	ABCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

// Tick
void UBTTaskNode_Attack::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 공격이 끝날 때까지, 기다리기
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


