﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "ABGameStateBase.h"

// AddToViewport 함수를 호출하기 전에 UI 위젯이 미리 Game State의 정보를 읽어들일 수 있도록 바인딩하는 함수
void UABGameplayResultWidget::BindGameState(AABGameStateBase* GameState)
{
	ABCHECK(nullptr != GameState);
	CurrentGameState = GameState;
}


// UI 위젯을 초기화하는 시점에서 발생
void UABGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(CurrentGameState.IsValid());

	// 결과 텍스트 블록 연동
	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	ABCHECK(nullptr != Result);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Mission Complete") : TEXT("Mission Failed")));

	// 획득한 점수 텍스트 블록 연동
	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	ABCHECK(nullptr != TotalScore);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}