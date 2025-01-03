﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABGameStateBase.h"

// 생성자
AABGameStateBase::AABGameStateBase()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

// 게임 점수 가져오기
int32 AABGameStateBase::GetTotalGameScore() const
{
	return TotalGameScore;
}

// 게임 점수 추가
void AABGameStateBase::AddGameScore()
{
	TotalGameScore++;
}

// 게임 클리어
void AABGameStateBase::SetGameCleared()
{
	bGameCleared = true;
}

// 게임 클리어 상태 가져오기
bool AABGameStateBase::IsGameCleared() const
{
	return bGameCleared;
}