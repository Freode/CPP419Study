﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/SaveGame.h"
#include "ABSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UABSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// 생성자
	UABSaveGame();

	// 레벨
	UPROPERTY()
		int32 Level;

	// 경험치
	UPROPERTY()
		int32 Exp;

	// 플레이어 이름
	UPROPERTY()
		FString PlayerName;

	// 최고 점수
	UPROPERTY()
		int32 HighScore;
	
	// 선택한 캐릭터 인덱스
	UPROPERTY()
		int32 CharacterIndex;
	
};
