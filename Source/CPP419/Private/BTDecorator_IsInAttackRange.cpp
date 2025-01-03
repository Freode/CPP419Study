﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "BTDecorator_IsInAttackRange.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	// 노드 이름 설정
	NodeName = TEXT("CanAttack");
}

// 원하는 조건을 달성했는지 확인하는 함수
bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 현재 데코레이터를 실행하고 있는 객체의 캐릭터 폰을 가져옴
	// auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto ControllingPawn = Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	// 타겟 정보를 캐스팅해서 가져오기
	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	// 플레이어 캐릭터가 특정 거리 안에 존재하는지 확인
	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange());
	return bResult;
}
