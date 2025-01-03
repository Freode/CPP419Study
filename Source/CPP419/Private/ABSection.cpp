﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABSection.h"
#include "ABCharacter.h"
#include "ABItemBox.h"
#include "ABPlayerController.h"
#include "ABGameMode.h"


// Sets default values
AABSection::AABSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 만들기
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	
	// 계층 설정
	RootComponent = Mesh;

	// 배경 애셋 가져와서 등록하기
	FString AssetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *AssetPath);
	}

	// Trigger 설정
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	Trigger->SetCollisionProfileName(TEXT("ABTrigger"));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnTriggerBeginOverlap);

	// 게이트 철문 애셋 가져와서 등록하기
	FString GateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *GateAssetPath);
	}

	// 철문 소켓들
	static FName GateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (FName GateSocket : GateSockets)
	{
		// 해당 소켓이 존재하는 경우에만 진행
		ABCHECK(Mesh->DoesSocketExist(GateSocket));
		// 철문 소켓에 대해 스켈레탈 메시 컴포넌트를 만들고 스켈레탈 메시를 넣어주기
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		NewGate->SetStaticMesh(SM_GATE.Object);

		// 해당 소켓에 붙이기
		NewGate->SetupAttachment(RootComponent, GateSocket);
		// 철문 피벗이 왼쪽에 있어 그거에 따른 보정
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		// 철문 목록에 등록
		GateMeshes.Add(NewGate);

		// Box Component 설정
		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnGateTriggerBeginOverlap);
		// 소켓 이름으로 태그 설정
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	// NoBattle 상태 설정
	bNoBattle = false;

	// 스폰 시간 설정
	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;
}

// Called when the game starts or when spawned
void AABSection::BeginPlay()
{
	Super::BeginPlay();

	// bNoState 상태에 따라 현재 섹션 상태를 설정
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
	
}

// Called every frame
void AABSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 섹션 상태 설정
void AABSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	// 준비 상태
	case ESectionState::READY:
	{
		// 구역 컬리전 설정
		Trigger->SetCollisionProfileName(TEXT("ABTrigger"));
		// 철문 컬리전 비활성화
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		// 출입문 활성화
		OperateGates(true);
		break;
	}

	case ESectionState::BATTLE:
	{
		// 구역 컬리전 비활성화
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		// 철문 컬리전 비활성화
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		// 출입문 비활성화
		OperateGates(false);

		// AI 적과 아이템 타이머 활성화
		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &AABSection::OnNPCSpawn), EnemySpawnTime, false);
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
			// 아이템은 해당 섹션에서 랜덤한 위치에 생성되도록 설정
			FVector2D RandXY = FMath::RandPointInCircle(600.0f);
			GetWorld()->SpawnActor<AABItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
		}), ItemBoxSpawnTime, false);
		break;
	}
	case ESectionState::COMPLETE:
	{
		// 구역 컬리전 비활성화
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		// 철문 컬리전 활성화
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		}

		// 출입문 활성화
		OperateGates(true);
		break;
	}
	}
}

// 철문(출입문) 작동
void AABSection::OperateGates(bool bOpen)
{
	// 출입문 회전 설정
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

// OnConstruction
void AABSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// No Battle에 따라 섹션 상태 설정
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// 맵 트리거에 도달할 때, 호출할 함수
void AABSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// 전투 모드로 변경
	if (CurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

// 철문 출입문 트리거에 도달할 때, 호출할 함수
void AABSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// 태그가 없으면 바로 반환
	ABCHECK(OverlappedComponent->ComponentTags.Num() == 1);

	// 태그 가져오기
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// 두 번째 단어가 소켓 이름?
	FName SocketName = FName(*ComponentTag.ToString().Left(2));

	if (!Mesh->DoesSocketExist(SocketName))
	{
		return;
	}

	// 소켓 위치 가져오기
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	// 충돌확인
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		// 소켓 위치에 새로운 섹션 생성
		auto NewSection = GetWorld()->SpawnActor<AABSection>(NewLocation, FRotator::ZeroRotator);
	}

	// 소켓 위치에 이미 새로운 섹션이 있는 경우
	else
	{
		ABLOG(Warning, TEXT("New section area is not empty."));
	}
}

// AI 적 생성
void AABSection::OnNPCSpawn()
{
	// Z 위치 조정해서 스폰
	auto KeyNPC = GetWorld()->SpawnActor<AABCharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);

	// 스폰한 NPC한테 죽을 때, 호출할 함수를 연결
	if (nullptr != KeyNPC)
	{
		KeyNPC->OnDestroyed.AddDynamic(this, &AABSection::OnKeyNPCDestroyed);
	}
	
}

// NPC 삭제
void AABSection::OnKeyNPCDestroyed(AActor* DestroyedActor)
{
	// 삭제할 NPC 객체
	auto ABCharacter = Cast<AABCharacter>(DestroyedActor);
	ABCHECK(nullptr != ABCharacter);

	// NPC 객체를 마지막으로 피격한 플레이어 컨트롤러 객체
	auto ABPlayerContorller = Cast<AABPlayerController>(ABCharacter->LastHitBy);
	ABCHECK(nullptr != ABPlayerContorller);

	// 게임 모드 가져오기
	auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != ABGameMode);

	// 게임 점수 증가
	ABGameMode->AddScore(ABPlayerContorller);

	// 현재 구역 클리어 상태로 설정
	SetState(ESectionState::COMPLETE);
}