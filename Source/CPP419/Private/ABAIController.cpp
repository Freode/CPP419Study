﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

// ===== Blackboard Key Name =====
const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AABAIController::TargetKey(TEXT("Target"));

// 생성자
AABAIController::AABAIController()
{
	//// 타이머 간격 설정
	//RepeatInterval = 3.0f;

	// 블랙보드 설정
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Book/AI/BB_ABCharacter.BB_ABCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// 비헤이비어 트리 설정
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Book/AI/BT_ABCharacter.BT_ABCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

// 컨트롤러가 해당 캐릭터를 조종할 때, 함수
void AABAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	//// 블랙 보드로 BBAsset을 사용하고 있는지 확인
	//if (UseBlackboard(BBAsset, Blackboard))
	//{
	//	// 비헤이비어 트리가 작동하고 있는지 확인
	//	if (!RunBehaviorTree(BTAsset))
	//	{
	//		ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
	//	}
	//}
	//
	//// AI 캐릭터 처음 위치를 설정
	//Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

	////// 타이머 실행
	////GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);
}

// 컨트롤러가 해당 캐릭터를 조종하지 않을 때, 함수
void AABAIController::UnPossess()
{
	Super::UnPossess();
	
	//// 타이머 해제
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

// 3초마다 폰에게 목적지로 이동하는 명령 함수
//void AABAIController::OnRepeatTimer()
//{
//	// 현재 객체의 폰 얻기
//	auto CurrentPawn = GetPawn();
//
//	ABCHECK(nullptr != CurrentPawn);
//
//	// 네비 매시 볼륨 가져오기
//	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(GetWorld());
//	if (nullptr == NavSystem) return;
//
//	// NPC가 이동할 다음 위치가 범위 안에 있을 경우
//	FNavLocation NextLocation;
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		// NPC 이동
//		UNavigationSystem::SimpleMoveToLocation(this, NextLocation.Location);
//		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
//	}
//}

// AI 실행
void AABAIController::RunAI()
{
	// 블랙 보드로 BBAsset을 사용하고 있는지 확인
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		// 비헤이비어 트리가 작동하고 있는지 확인
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

// AI 중지
void AABAIController::StopAI()
{
	// Behavior Tree 캐스팅
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		// 멈추기
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}