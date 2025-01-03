﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

// 생성자
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// Tick
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 현재 Service가 진행하고 있는 객체의 캐릭터 폰을 가져오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	// World 객체
	UWorld* World = ControllingPawn->GetWorld();
	// 현재 캐릭터 위치
	FVector Center = ControllingPawn->GetActorLocation();
	// 탐지 반경
	float DetectRadius = 600.0f;

	if (nullptr == World)
	{
		return;
	}

	// 탐지 반경 내에 있는 모든 ABCharacter 객체 탐지
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool Result = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParam);

	if (Result)
	{
		// 모든 ABCharacter 객체에 대한 검사
		for (auto const& OverlapResult : OverlapResults)
		{
			// ABCharacter 객체로 형변환
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());

			// ABCharacter 객체가 유효하고 플레이어가 조종하는 객체인지 확인
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				// 추적 대상을 플레이어 객체로 설정
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);

				// 탐지 반경을 디버그 모드로 출력
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 플레이어까지의 탐지 거리를 디버그 모드로 출력
				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
			}
		}
	}

	// 플레이어가 탐지되지 않았을 경우에는 빨간색 원으로 출력
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}



