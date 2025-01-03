﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	// 생성자
	AABAIController();
	
	// 컨트롤러가 해당 캐릭터를 조종할 때, 함수
	virtual void Possess(APawn* InPawn) override;

	// 컨트롤러가 해당 캐릭터를 조종하지 않을 때, 함수
	virtual void UnPossess() override;

	// ===== Blackboard Key Name =====

	// 출발지
	static const FName HomePosKey;
	// 도착지
	static const FName PatrolPosKey;
	// 플레이어
	static const FName TargetKey;
	
	// AI 실행
	void RunAI();
	// AI 중지
	void StopAI();

private:

	// 비헤이비어 트리
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	// 블랙보드
	UPROPERTY()
		class UBlackboardData* BBAsset;

	//// 3초마다 폰에게 목적지로 이동하는 명령 함수
	//void OnRepeatTimer();

	//// 타이머
	//FTimerHandle RepeatTimerHandle;

	//// 타이머 간격
	//float RepeatInterval;
};
