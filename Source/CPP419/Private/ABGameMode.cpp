// User : Jeonghoe22 : Study by '�̵���� �𸮾� C++ ���Ӱ����� ����'

#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABGameStateBase.h"

AABGameMode::AABGameMode()
{
	// Default Pawn Class�� ABPawn Ŭ������ �����ϱ�
	// �𸮾� ������Ʈ Ŭ���� ������ Unreal Header Tool�� ���� �ڵ����� �����Ǹ�, �𸮾� ������Ʈ���� �ڵ����� ������
	// StaticClass��� ����ƽ �Լ��� ȣ���� ������ �� �ִ�.
	DefaultPawnClass = AABCharacter::StaticClass();

	// Player Controller Class�� Ŭ������ ����
	PlayerControllerClass = AABPlayerController::StaticClass();

	// Player State Class�� ����
	PlayerStateClass = AABPlayerState::StaticClass();

	// Game State Class�� ����
	GameStateClass = AABGameStateBase::StaticClass();

	// ��ǥ ���� ����
	ScoreToClear = 2;
}

// �α����� ����
void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));

	// PlayerState ������ �ʱ�ȭ
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();
}

// ������Ʈ �ʱ�ȭ ����
void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameStateBase>(GameState);
}

// ���� ���� ����
void AABGameMode::AddScore(AABPlayerController* ScoredPlayer)
{
	// �÷��̾� ��Ʈ�ѷ���ŭ ��� �ݺ�
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());

		// ������ �÷��̾� ��Ʈ�ѷ��� ��ġ�� ��
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}

	// ���� ������Ʈ���� ���� ����
	ABGameState->AddGameScore();

	// ��ǥ ���� �޼� ���� Ȯ��
	if (GetScore() >= ScoreToClear)
	{
		ABGameState->SetGameCleared();

		// Pawn Iterator�� ���� : ��� �� ���߱�
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		// �÷��̾� ��Ʈ�ѷ� ã��
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			// ��� UI ���
			const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
			if (nullptr != ABPlayerController)
			{
				ABPlayerController->ShowResultUI();
			}
		}

	}
}

// ���� ���� ��������
int32 AABGameMode::GetScore() const
{
	return ABGameState->GetTotalGameScore();
}

