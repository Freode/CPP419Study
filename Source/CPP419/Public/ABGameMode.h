// User : Jeonghoe22 : Study by '�̵���� �𸮾� C++ ���Ӱ����� ����'
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
	
// ������ �ڵ� ����
public:
	// ������
	AABGameMode();
	
	// �α����� ����
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// ������Ʈ �ʱ�ȭ�� ��
	virtual void PostInitializeComponents() override;

	// ���� ���� ����
	void AddScore(class AABPlayerController *ScoredPlayer);

	// ���� ���� ��������
	int32 GetScore() const;

private:
	// Game State Class
	UPROPERTY()
		class AABGameStateBase* ABGameState;

	// ��ǥ ����
	UPROPERTY()
		int32 ScoreToClear;
};
