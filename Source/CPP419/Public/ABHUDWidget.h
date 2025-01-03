﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 캐릭터 스테이트의 체력 변경될 때, HUD에서도 변경되도록 바인딩
	void BindCharacterStat(class UABCharacterStatComponent* CharacterStat);
	// 플레이어 스테이트가 변경될 때, HUD에서도 변경되도록 바인딩
	void BindPlayerState(class AABPlayerState* PlayerState);
	
protected:
	virtual void NativeConstruct() override;

	// 업데이트 함수
	void UpdateCharacterStat();
	void UpdatePlayerState();
	
private:
	// 캐릭터 정보 약 포인터 클래스
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;
	// 플레이어 스테이트 약 포인터 클래스
	TWeakObjectPtr<class AABPlayerState> CurrentPlayerState;

	// 체력바
	UPROPERTY()
		class UProgressBar* HPBar;

	// 경험치바
	UPROPERTY()
		class UProgressBar* ExpBar;

	// 플레이어 이름 출력
	UPROPERTY()
		class UTextBlock* PlayerName;

	// 플레이어 레벨 출력
	UPROPERTY()
		class UTextBlock* PlayerLevel;

	// 현재 점수 출력
	UPROPERTY()
		class UTextBlock* CurrentScore;

	// 현재 최고점
	UPROPERTY()
		class UTextBlock* HighScore;

};
