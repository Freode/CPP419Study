﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

// ===== 델리게이트 =====
// 체력이 0 이하 됐을 때, 사용할 멀티캐스트 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
// 체력이 변경되었을 때, 사용할 멀티캐스트 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP419_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Actor의 PostInitializeComponents() 전에 호출
	virtual void InitializeComponent() override;

public:

	// 플레이어의 레벨을 새롭게 설정
	void SetNewLevel(int32 NewLevel);

	// 데미지 입었을 때, 호출할 함수
	void SetDamage(float NewDamage);

	// 현재 체력 설정
	void SetHP(float NewHP);

	// 현재 플레이어 데미지 가져오기
	float GetAttack() const;

	// 현재 체력이 최대 체력 대비 몇 %인지 가져옴
	float GetHPRatio() const;

	// 죽었을 때, 감소할 경험치량 가져오기
	int32 GetDropExp() const;

	// 체력이 0 이하 됐을 때, 사용할 멀티캐스트 델리게이트 객체
	FOnHPIsZeroDelegate OnHPIsZero;
	// 현재 체력이 변경됐을 때, 사용할 멀티캐스트 델리게이트 객체
	FOnHPChangedDelegate OnHPChanged;

private:

	// 현재 레벨에 따른 플레이어 상태 정보
	struct FABCharacterData* CurrentStatData = nullptr;

	// 현재 플레이어 레벨
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	
	// UPROPERTY(Transient) : 직렬화 비활성화
	// 자동 초기화, 잠재적 접근, 가비지 컬렉션으로 들어가는 것을 방지
	
	// 오브젝트의 UPROPERTY() 속성을 저장하고 로딩할 수 있음.
	// 하지만, CurrentHP는 게임을 시작할 때마다 변경되므로 이 값을 보관하는 것은 의미가 없고 오히려 오브젝트를 저장할 때, 필요 없는 디스크 공간만 차지.
	// 그러므로 Transient 키워드를 추가해 해당 속성을 직렬화에서 제외

	// 현재 플레이저 체력
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;
};
