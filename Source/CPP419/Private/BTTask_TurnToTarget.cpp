﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "BTTask_TurnToTarget.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// 노드 이름 설정
	NodeName = TEXT("Turn");
}

// Execute
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 현재 task를 실행하는 캐릭터 객체를 캐스팅하기
	auto ABCharacter = Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == ABCharacter)
	{
		return EBTNodeResult::Failed;
	}

	// 탐지한 플레이어 객체를 캐스팅하기
	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey));

	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	// 플레이어와 AI 캐릭터와의 거리 
	FVector LookVector = Target->GetActorLocation() - ABCharacter->GetActorLocation();
	// Z축은 무시
	LookVector.Z = 0.0f;
	// AI 캐릭터가 플레이어를 바라보도록 설정
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); // ?
	ABCharacter->SetActorRotation(FMath::RInterpTo(ABCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;


}



