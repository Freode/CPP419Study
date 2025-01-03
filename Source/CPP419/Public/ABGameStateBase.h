﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/GameStateBase.h"
#include "ABGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API AABGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// 생성자
	AABGameStateBase();

	// 게임 클리어
	void SetGameCleared();

	// 게임 클리어 상태 가져오기
	bool IsGameCleared() const;

public:
	// 게임 점수 가져오기
	int32 GetTotalGameScore() const;

	// 게임 점수 추가
	void AddGameScore();

private:
	// 게임 점수
	UPROPERTY(Transient)
		int32 TotalGameScore;

	// 게임 클리어했는지 여부
	UPROPERTY(Transient)
		bool bGameCleared;
	
};
