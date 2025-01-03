﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"


// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeComponent 함수 활성화
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Actor의 PostInitializeComponents() 함수 전에 호출
void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 레벨 설정
	SetNewLevel(Level);
}

// 플레이어의 레벨을 새롭게 설정
void UABCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	// 게임 인스턴스 가져오기
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != ABGameInstance);

	// 새로운 레벨에 따른 플레이어 정보 가져오기
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);

	// 새로운 레벨에 따른 플레이어 정보 저장
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		// 체력 설정
		SetHP(CurrentStatData->MaxHP);
	}
	// 가져오지 못할 경우, 에러 출력
	else
	{
		ABLOG(Error, TEXT("Level (%d) data doesn't exit"), NewLevel);
	}
}

// 데미지 입었을 때, 호출할 함수
void UABCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(nullptr != CurrentStatData);

	// 체력이 0과 최대 체력 사이의 값으로 존재하도록 조정
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

// 현재 플레이어 데미지 가져오기
float UABCharacterStatComponent::GetAttack() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

// 현재 체력 설정
void UABCharacterStatComponent::SetHP(float NewHP)
{
	// 체력 설정
	CurrentHP = NewHP;
	// 체력이 변경됐을 때, 해당 델리게이트에 등록된 모든 함수를 호출
	OnHPChanged.Broadcast();

	// KINDA_SMALL_NUMBER
	// float의 값을 0과 비교할 때, 미세한 오차 범위 내에 있는지를 보고 판단하는 것이 좋음.
	// 무시 가능한 오차를 측정할 때, 사용하는 것이 KINDA_SMALL_NUMBER임. 

	// 죽었을 때
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

// 현재 체력이 최대 체력 대비 몇 %인지 가져옴
float UABCharacterStatComponent::GetHPRatio() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	// 비중 반환
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

// 죽었을 때, 감소할 경험치량 가져오기
int32 UABCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}