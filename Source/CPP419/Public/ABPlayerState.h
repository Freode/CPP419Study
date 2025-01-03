﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

// 플레이어 스테이트와 캐릭터 스탯 컴포넌트 정보를 모두 HUD에 연동하기 위해 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class CPP419_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// 생성자
	AABPlayerState();

	// 게임 점수 가져오기
	int32 GetGameScore() const;
	
	// 게임 최고 점수 가져오기
	int32 GetGameHighScore() const;

	// 캐릭터 레벨 가져오기
	int32 GetCharacterLevel() const;

	// 선택한 캐릭터 인덱스 가져오기
	int32 GetCharacterIndex() const;

	// Exp 비율 가져오기
	float GetExpRatio() const;

	// 경험치 추가
	bool AddExp(int32 IncomeExp);

	// 게임 점수 증가
	void AddGameScore();

	// 플레이어 데이터 초기화
	void InitPlayerData();

	// 플레이어 데이터 저장
	void SavePlayerData();

	// 플레이어 스테이트가 변경될 때 사용할 델리게이트 객체
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

	// 슬롯 저장 이름
	FString SaveSlotName;

protected:

	// 게임 점수
	UPROPERTY(Transient)
		int32 GameScore;

	// 캐릭터 레벨
	UPROPERTY(Transient)
		int32 CharacterLevel;

	// 경험치
	UPROPERTY(Transient)
		int32 Exp;

	// 최고 게임 점수
	UPROPERTY(Transient)
		int32 GameHighScore;

	// 선택한 캐릭터 인덱스
	UPROPERTY(Transient)
		int32 CharacterIndex;

private:
	// 캐릭터 레벨 설정
	void SetCharacterLevel(int32 NewCharacterLevel);

	// 현재 캐릭터 상태 객체
	struct FABCharacterData* CurrentStatData;
	
	
};
