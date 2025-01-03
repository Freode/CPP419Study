﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "BTTask_FindPatrolPos.h"
#include "ABAIcontroller.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// 노드 이름 설정
	NodeName = TEXT("FindPatrolPos");
}

// Execute Task
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 현재 task가 실행중인 캐릭터 객체 가져오기
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 캐릭터 객체가 없는 경우, Failed를 반환
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 네비 메시 볼륨 찾기
	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	// 출발지 위치 정보를 블랙보드로부터 가져오기
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);

	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		// NPC가 이동할 다음 위치가 범위 안에 있을 경우
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}



