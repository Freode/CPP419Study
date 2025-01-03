﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// 캐릭터 상태가 변경될 때, 호출할 함수
	void BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat);

protected:
	// UI 초기화 시점
	// UI 시스템이 준비되면 NativeConstruct 함수가 호출.
	// UI 생성은 플레이어 컨트롤러의 BeginPlay에서 호출되므로 
	// BeginPlay 전에 호출된 PostInitializeComponents 함수에서 발생한 명령은 UI에 반영되지 않는다.
	// 따라서, 현재 구조에서는 NativeConstruct 함수에서 위젯 내용을 업데이트하는 로직을 구현
	virtual void NativeConstruct() override;

	// 체력 상태가 변경됐을 때, 호출할 함수
	void UpdateHPWidget();

private:
	// 약 포인터를 이용해 캐릭터 상태 컴포넌트를 참조
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;
	
	// 체력 UI
	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
