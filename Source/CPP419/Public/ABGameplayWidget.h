﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Blueprint/UserWidget.h"
#include "ABGameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UABGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// UI 위젯을 초기화하는 시점에서 발생
	virtual void NativeConstruct() override;
	
	// 게임 재개 버튼 클릭
	UFUNCTION()
		void OnResumeClicked();

	// Title로 이동 버튼 클릭
	UFUNCTION()
		void OnReturnToTitleClicked();

	// 재시작 버튼 클릭
	UFUNCTION()
		void OnRetryGameClicked();

protected:
	// 재개 버튼
	UPROPERTY()
		class UButton* ResumeButton;

	// Title로 이동 버튼
	UPROPERTY()
		class UButton* ReturnToTitleButton;

	// 게임 재시작 버튼
	UPROPERTY()
		class UButton* RetryGameButton;
};
