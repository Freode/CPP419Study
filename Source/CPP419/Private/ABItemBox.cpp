﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"


// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 그릇 만들기
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	// 계층 설정
	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	// Transform 설정
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// 박스 스태틱 메시 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	// 파티클 이펙트 설정
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		// 자동 시작 해제
		Effect->bAutoActivate = false;
	}

	// 박스 스태틱 메시 위치 설정
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// 컬리전 설정
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// 해당 속성에 대한 기본 클래스 값 지정
	WeaponItemClass = AABWeapon::StaticClass();

}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Initialization before BeginPlay
void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 델리게이트에 호출될 함수 연결
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

// Overlap Event
void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	// 현재 컬리전과 부딪힌 액터가 AABCharacter인지 확인
	auto ABCharacter = Cast<AABCharacter>(OtherActor);
	ABCHECK(nullptr != ABCharacter);

	// 아이템을 주울 캐릭터와 캐릭터에게 줄 아이템 클래스가 모두 존재할 경우
	if (nullptr != ABCharacter && nullptr != WeaponItemClass)
	{
		// 해당 캐릭터가 아이템을 주울 수 있는 상태
		if (ABCharacter->CanSetWeapon())
		{
			// 새로운 무기 생성
			// SpawnActor는 월드에서 생성되는 것이므로 GetWorld()로 월드 포인터를 가져와서 진행
			// 생성자에서는 진행하지 못함.
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			ABCharacter->SetWeapon(NewWeapon);

			// 이펙트 효과 재생
			Effect->Activate(true);

			// 중복 실행 방지로 아이템 박스 비활성화와 숨기기 ( 이펙트 효과 재생완료되기 전까지 )
			// SetHiddenInGame -> 에디터에서는 보임, 플레이할 때 안보임.
			// SetVisibility -> 에디터와 플레이할 때, 모두 안보이게 설정
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);

			// 이펙트 재생 완료 시, 호출할 함수를 다이내믹 델리게이트에 등록
			Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
		}

		// 해당 캐릭터가 아이템을 주울 수 없는 상태
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
		}
	}
}

// 이펙트 효과 재생이 완료되어서 해당 함수가 호출되면, 아이템 상자가 삭제되도록 진행
void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}
