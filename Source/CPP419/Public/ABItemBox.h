﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class CPP419_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialization before Beginplay
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 플레이어 감지
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* Trigger;

	// 아이템 상자 시각화
	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;

	// 아이템을 먹을 때, 작동하는 효과
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

	// 플레이어가 아이템 상자에서 가져갈 아이템 클래스
	// VisibleAnywhere : 어디서든 그냥 보이기만 함
	// EditInstanceOnly : 뷰포트에서 객체를 편집할 때, 설정 변경 가능
	UPROPERTY(EditInstanceOnly, Category = Box)
		TSubclassOf<class AABWeapon> WeaponItemClass;

private:
	// Overlap Event
	// 아래 함수 인자는 항상 저런 형식을 가져야 함.
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 이펙트 효과가 종료되었을 때, 호출
	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);
};
