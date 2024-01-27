// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPawn.h"


// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// 계층 구조 설정
	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	// 기본값 설정
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// 애니메이션 블루프린트 인스턴스 설정
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// ClassFinder를 통해서 클래스 정보를 가져옴
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimationBlueprint.WarriorAnimationBlueprint_C"));
	
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();

	//// 게임 실행 중에 애셋을 로드하는 명령어 : LoadObject<Type>
	//Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/InfinityBladeWarriors/Animations/WarriorRun.WarriorRun"));

	//// 애셋 로딩이 완료되면 추가
	//if (AnimAsset != nullptr)
	//{
	//	Mesh->PlayAnimation(AnimAsset, true);
	//}
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 키보드 이벤트에 연결할 함수를 여기서 정의
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPawn::PossessedBy(AController* NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

// 위아래 움직일 때, 연결할 함수
void AABPawn::UpDown(float NewAxisValue)
{
	// 폰의 움직임을 주는 함수
	AddMovementInput(GetActorForwardVector(), NewAxisValue);

	// ABLOG(Warning, TEXT("%f"), NewAxisValue);
}

// 왼쪽오른쪽으로 움직일 때, 연결할 함수
void AABPawn::LeftRight(float NewAxisValue)
{
	// 폰의 움직임을 주는 함수
	AddMovementInput(GetActorRightVector(), NewAxisValue);

	// ABLOG(Warning, TEXT("%f"), NewAxisValue);
}