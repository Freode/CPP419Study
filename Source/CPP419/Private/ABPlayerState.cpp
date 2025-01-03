﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABPlayerState.h"
#include "ABGameInstance.h"
#include "ABSaveGame.h"

// 생성자
AABPlayerState::AABPlayerState()
{
	// 초기화
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
	GameHighScore = 0;
	SaveSlotName = TEXT("Player1");
	CharacterIndex = 0;
}

// 게임 점수 얻기
int32 AABPlayerState::GetGameScore() const
{
	return GameScore;
}

// 캐릭터 레벨 가져오기
int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

// 플레이어 데이터 초기화
void AABPlayerState::InitPlayerData()
{
	// 세이브 게임 객체 가져오기
	auto ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == ABSaveGame)
	{
		// 없으면, 게임 객체 새롭게 생성
		ABSaveGame = GetMutableDefault<UABSaveGame>();
	}

	// 플레이어 데이터를 마지막으로 세이브한 결과값으로 초기화
	SetPlayerName(ABSaveGame->PlayerName);
	SetCharacterLevel(ABSaveGame->Level);
	GameScore = 0;
	GameHighScore = ABSaveGame->HighScore;
	Exp = ABSaveGame->Exp;
	CharacterIndex = ABSaveGame->CharacterIndex;

	// 데이터 저장
	SavePlayerData();
}

// Exp 비율 가져오기
float AABPlayerState::GetExpRatio() const
{
	// 다음 레벨을 달성하기 위해 필요한 경험치량이 0.0 이하
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	// 비율 구하고 반환
	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

// 경험치 추가
bool AABPlayerState::AddExp(int32 IncomeExp)
{
	// 만렙일 때
	if (CurrentStatData->NextExp == -1)
	{
		return false;
	}

	bool DidLevelUp = false;

	// 경험치 증가
	Exp = Exp + IncomeExp;

	// 레벨 업
	if (Exp >= CurrentStatData->NextExp)
	{
		// 현재 경험치를 다음 레벨 필요 경험치량만큼 감소
		Exp -= CurrentStatData->NextExp;
		// 레벨 업
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	// 경험치 추가에 따른 함수들 호출
	OnPlayerStateChanged.Broadcast();
	
	// 데이터 저장
	SavePlayerData();

	return DidLevelUp;
}

// 캐릭터 레벨 설정
void AABPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	// Game Instance 가져오기
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);

	// 새로운 레벨에 따른 데이터를 가져오기
	CurrentStatData = ABGameInstance->GetABCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	// 새로운 레벨 설정
	CharacterLevel = NewCharacterLevel;
}

// 게임 점수 증가
void AABPlayerState::AddGameScore()
{
	GameScore++;

	// 게임 최고 점수 초기화
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}

	OnPlayerStateChanged.Broadcast();

	// 게임 데이터 저장
	SavePlayerData();
}

// 게임 최고 점수 가져오기
int32 AABPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

// 플레이어 데이터 저장
void AABPlayerState::SavePlayerData()
{
	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	// 게임 슬롯 데이터에 저장
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		// 실패시
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}

// 선택한 캐릭터 인덱스 가져오기
int32 AABPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}