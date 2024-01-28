﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	// 생성자
	UBTService_Detect();

protected:
	// Tick
	virtual void TickNode(UBehaviorTreeComponent& OwenrComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
