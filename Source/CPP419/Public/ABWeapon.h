﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/Actor.h"
#include "ABWeapon.generated.h"

UCLASS()
class CPP419_API AABWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 무기 스켈레탈 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	// 무기 공격 사거리 가져오기
	float GetAttackRange() const;

	// 공격 데미지 가져오기
	float GetAttackDamage() const;

	// 효과치 속성 가져오기
	float GetAttackModifier() const;

protected:
	// 무기 공격 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackRange;

	// 최소 무기 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackDamageMin;
	
	// 최대 무기 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackDamageMax;

	// 최소 무기 속성치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackModifierMin;

	// 최대 무기 속성치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackModifierMax;

	// 설정된 무기 공격력
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
		float AttackDamage;

	// 설정된 무기 속성치
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
		float AttackModifier;
};
