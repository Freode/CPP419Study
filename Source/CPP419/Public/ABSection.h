﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/Actor.h"
#include "ABSection.generated.h"

UCLASS()
class CPP419_API AABSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// OnConstruction
	virtual void OnConstruction(const FTransform& Transfrom) override;

	// 맵 트리거에 도달할 때, 호출할 함수
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// 철문 출입문 트리거에 도달할 때, 호출할 함수
	UFUNCTION()
		void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// NPC 제거
	UFUNCTION()
		void OnKeyNPCDestroyed(AActor* DestroyedActor);

	// AI 적 생성
	void OnNPCSpawn();

private:
	// 섹션 상태
	enum class ESectionState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};

	// 섹션 상태 설정
	void SetState(ESectionState NewState);
	// 현재 섹션 상태
	ESectionState CurrentState = ESectionState::READY;

	// 철문(출입문) 작동
	void OperateGates(bool bOpen = true);

private:
	
	// 현재 섹션의 주요 배경
	UPROPERTY(VisibleAnywhere, Category = Math, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* Mesh;

	// 각 소켓에 붙어있는 철문 목록들
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		TArray<UStaticMeshComponent*> GateMeshes;

	// ABCharacter만 감지하는 철문 트리거들
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		TArray<UBoxComponent*> GateTriggers;

	// 맵에 대한 Trigger
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		UBoxComponent* Trigger;

	// 현재 섹션이 NoBattle 상태인지 확인
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
		bool bNoBattle;

	// 적 AI 스폰 시간
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
		float EnemySpawnTime;

	// 아이템 스폰 시간
	UPROPERTY(EditAnywhere, Category = SPawn, Meta = (AllowPrivateAccess = true))
		float ItemBoxSpawnTime;

	// 스폰 시간에 따른 핸들러
	FTimerHandle SpawnNPCTimerHandle = {};
	FTimerHandle SpawnItemBoxTimerHandle = {};
	
};
