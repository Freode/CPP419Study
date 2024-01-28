// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABGameStateBase.h"

AABGameMode::AABGameMode()
{
	// Default Pawn Class를 ABPawn 클래스로 지정하기
	// 언리얼 오브젝트 클래스 정보는 Unreal Header Tool에 의해 자동으로 생성되며, 언리얼 오브젝트마다 자동으로 생성된
	// StaticClass라는 스태틱 함수를 호출해 가져올 수 있다.
	DefaultPawnClass = AABCharacter::StaticClass();

	// Player Controller Class의 클래스를 변경
	PlayerControllerClass = AABPlayerController::StaticClass();

	// Player State Class를 변경
	PlayerStateClass = AABPlayerState::StaticClass();

	// Game State Class를 변경
	GameStateClass = AABGameStateBase::StaticClass();

	// 목표 점수 설정
	ScoreToClear = 2;
}

// 로그인한 직후
void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));

	// PlayerState 데이터 초기화
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();
}

// 컴포넌트 초기화 직후
void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameStateBase>(GameState);
}

// 게임 점수 증가
void AABGameMode::AddScore(AABPlayerController* ScoredPlayer)
{
	// 플레이어 컨트롤러만큼 모두 반복
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());

		// 증가할 플레이어 컨트롤러와 일치할 때
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}

	// 게임 스테이트에서 점수 증가
	ABGameState->AddGameScore();

	// 목표 점수 달성 여부 확인
	if (GetScore() >= ScoreToClear)
	{
		ABGameState->SetGameCleared();

		// Pawn Iterator로 돌기 : 모든 폰 멈추기
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		// 플레이어 컨트롤러 찾기
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			// 결과 UI 출력
			const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
			if (nullptr != ABPlayerController)
			{
				ABPlayerController->ShowResultUI();
			}
		}

	}
}

// 게임 점수 가져오기
int32 AABGameMode::GetScore() const
{
	return ABGameState->GetTotalGameScore();
}

