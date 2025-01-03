﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABWeapon.h"


// Sets default values
AABWeapon::AABWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 무기 컴포넌트 생성
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	// 무기 추가
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	// 무기 컴포넌트에서 충돌 시스템 해제
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// 공격 값 초기 설정
	AttackRange = 150.0f;
	AttackDamageMin = -2.5f;
	AttackDamageMax = 10.0f;
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;

}

// Called when the game starts or when spawned
void AABWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// 무기 공격력과 효과치 속성값을 랜덤으로 설정하기
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);
	ABLOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), AttackDamage, AttackModifier);
}

// Called every frame
void AABWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 무기 공격 사거리 가져오기
float AABWeapon::GetAttackRange() const
{
	return AttackRange;
}

// 공격 데미지 가져오기
float AABWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

// 효과치 속성 가져오기
float AABWeapon::GetAttackModifier() const
{
	return AttackModifier;
}


