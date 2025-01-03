﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "ABGameInstance.generated.h"

// 구조체 선언
// 플레이어 정보에 대한 구조체
USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 생성자
	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;
};

/**
 * 
 */
UCLASS()
class CPP419_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();

	// 게임 생성될 때 호출
	virtual void Init() override;

	// 해당 레벨의 캐릭터 정보를 가져오기
	FABCharacterData* GetABCharacterData(int32 Level);

	// FStreamableManager : 게임 진행 중에도 비동기 방식으로 애셋을 로딩하도록 제공
	// 비동기 방식으로 애셋을 로딩하는 명령은 AsyncLoad이다.
	// FStreamableDelegate 형식의 델리게이트를 넘겨줄 경우, 애셋이 로딩을 완료하면 해당 델리게이트에 연결된 함수를 호출.
	// FStreamableDelegate 형식으로 델리게이트 멤버를 선언하고 넘겨줄 수 있지만, 델리게이트에서 제공하는 CreateUObject 명령을 사용해 즉석에서
	// 델리게이트를 생성함으로써 함수와 연동시킨 후, 넘겨주는 방식이 간편하다.
	FStreamableManager StreamableManager;
	

private:

	// 캐릭터 테이블 정보
	UPROPERTY()
		class UDataTable* ABCharacterTable;
};
