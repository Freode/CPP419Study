﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABGameInstance.h"

// 생성자
UABGameInstance::UABGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");
	
	// 캐릭터 정보 가져오기
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
	ABCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
	// 제대로 로딩됐는지 확인

}

// 게임 생성될 때 호출
void UABGameInstance::Init()
{
	Super::Init();
	// 데이터가 잘 로딩됐는지 20레벨 경험치를 출력
	//ABLOG(Warning, TEXT("DropExp of Level 20 ABCharacter : %d"), GetABCharacterData(20)->DropExp);
}

// 해당 레벨의 캐릭터 정보 가져오기
FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}

