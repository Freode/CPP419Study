// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'
#pragma once

#include "CPP419.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
// 생성자 코드 실행
public:
	// 생성자
	AABGameMode();
	
	// 로그인한 직후
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 컴포넌트 초기화한 후
	virtual void PostInitializeComponents() override;

	// 게임 점수 증가
	void AddScore(class AABPlayerController *ScoredPlayer);

	// 게임 점수 가져오기
	int32 GetScore() const;

private:
	// Game State Class
	UPROPERTY()
		class AABGameStateBase* ABGameState;

	// 목표 점수
	UPROPERTY()
		int32 ScoreToClear;
};
