﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "ABGameplayWidget.h"
#include "ABGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UABGameplayResultWidget : public UABGameplayWidget
{
	GENERATED_BODY()
	
public:
	// AddToViewport 함수를 호출하기 전에 UI 위젯이 미리 Game State의 정보를 읽어들일 수 있도록 바인딩하는 함수
	void BindGameState(class AABGameStateBase* GameState);

protected:
	// UI 위젯을 초기화하는 시점에서 발생
	virtual void NativeConstruct() override;
	
private:
	// 현재 Game State 객체
	TWeakObjectPtr<class AABGameStateBase> CurrentGameState;
	
};
