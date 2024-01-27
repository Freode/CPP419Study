// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABWeapon.h"
#include "ABCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "ABAIController.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABHUDWidget.h"
#include "TimerManager.h"
#include "ABGameMode.h"


// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 정의
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	// ===== 계층 구조 설정 ======
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	// 체력 UI를 캐릭터에 붙이기
	HPBarWidget->SetupAttachment(GetMesh());

	// 기본값 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// 애니메이션 인스턴스 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimationBlueprint.WarriorAnimationBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);

	}

	// 컨트롤 모드를 0번으로 설정
	SetControlMode(EControlMode::DIABLO);

	// 부드럽게 움직일 때, SpringArm과 Camera 속도 설정
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// 점프 높이 조절
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	// 현재 공격 상태가 아니라고 설정
	IsAttacking = false;

	// 콤보 공격 최대치 및 콤보 종료 상태로 설정
	MaxCombo = 4;
	AttackEndComboState();

	// 캡슐 컴포넌트의 콜리전 프리셋 변경
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// 공격 사거리와 범위 설정
	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	// ===== 체력 UI =====
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// 화면을 항상 플레이어를 향해 바라보도록 하기 위해 Screen으로 설정.
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		// 체력 UI 클래스 설정 및 출력 영역 설정
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// AI Controller 연결
	AIControllerClass = AABAIController::StaticClass();
	// 월드에 있거나 스폰될 때, 자동으로 AI Controller가 활성화되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// CPP419Setting 모듈의 ABCharacterSetting.h로부터 정보를 가져올 수 있는지 확인
	//auto DefaultSetting = GetDefault<UABCharacterSetting>();

	//if (DefaultSetting->CharacterAssets.Num() > 0)
	//{
	//	for (auto CharacterAsset : DefaultSetting->CharacterAssets)
	//	{
	//		ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
	//	}
	//}

	// 캐릭터 스켈레탈 메시 번호
	AssetIndex = 4;
	// 캐릭터 상태 설정
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;

	// 사후 타이머 설정
	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//// 플레이어가 컨트롤하지 않을 경우, 애셋 경로를 config의 DefaultCPP419에서 가져와 인게임 도중 비동기로 애셋 로딩하기
	//if (!IsPlayerControlled())
	//{
	//	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	//	
	//	// 애셋 경로 중 랜덤으로 하나 선택
	//	int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	//	CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

	//	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	//	if (nullptr != ABGameInstance)
	//	{
	//		// 해당 애셋을 로드하고 완료된 함수를 호출
	//		AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	//	}
	//}

	// 현재 캐릭터를 플레이어가 조종하는지 확인
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		// 플레이어인 경우, 플레이어 컨트롤러를 캐스팅
		ABPlayerController = Cast<AABPlayerController>(GetController());
		ABCHECK(nullptr != ABPlayerController);
	}
	else
	{
		// AI인 경우, AI 컨트롤러를 캐스팅
		ABAIController = Cast<AABAIController>(GetController());
		ABCHECK(nullptr != ABAIController);
	}

	// config의 DefaultCPP419 파일을 가져옴.
	auto DefaultSetting = GetDefault<UABCharacterSetting>();

	// 플레이어인 경우, 본인이 선택한 캐릭터 번호로 캐릭터 설정
	if (bIsPlayer)
	{
		auto ABPlayerState = Cast<AABPlayerState>(PlayerState);
		ABCHECK(nullptr != ABPlayerState);
		AssetIndex = ABPlayerState->GetCharacterIndex();
	}
	// AI인 경우에는 config 파일의 애셋 경로 중 랜덤으로 하나 가져오기
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	// 캐릭터 스켈레탈 메시 설정
	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	// GameInstance 가져오기
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);
	// 비동기 로딩
	AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	// 캐릭터 상태를 Loading으로 변경
	SetCharacterState(ECharacterState::LOADING);
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SpringArm의 거리를 부드럽게 변경
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	// 모드 변경으로 인한 변경 기능
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		break;

	case EControlMode::DIABLO:
		// 모드 변경시, 카메라가 부드럽게 회전되도록 설정
		SpringArm->RelativeRotation = FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);
		break;
	}

	// 키보드를 통한 이동 관련 기능
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 다른 호출 함수에서 이미 정의
		break;

	case EControlMode::DIABLO:
		// 이동 방향 크기 제곱이 0보다 클때
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// 캐릭터 이동 방향 설정
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());

			// 캐릭터 움직이는 방향 주기
			AddMovementInput(DirectionToMove);
		}
		break;
	}

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 키보드 이벤트와 연결
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	// 마우스 이벤트와 연결
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);

	// 카메라 뷰 변경 이벤트
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);

	// 점프 추가 : Character 클래스에 이미 Jump 함수 추가되어 있음
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// 마우스 왼쪽 클릭 : 공격
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

// 위아래 방향키를 눌렀을 때, 해당 함수를 호출하고 캐릭터가 이동하도록 설정
void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 컨트롤 회전값 = 카메라가 바라보고 있는 값으로 이동
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

		// 정면으로 캐릭터 이동
		// AddMovementInput(GetActorForwardVector(), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// X축 이동 정도를 저장
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

// 왼쪽 오른쪽 방향키를 눌렀을 때, 해당 함수를 호출하고 캐릭터가 이동하도록 설정
void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 컨트롤 회전값 = 카메라가 바로보고 있는 우측 값으로 이동
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

		// 우측으로 캐릭터 이동
		// AddMovementInput(GetActorRightVector(), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// Y축 이동 정도를 저장
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

// 마우스를 위아래 방향으로 움직였을 때, 해당 함수를 호출하고 카메라 방향 변경
void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// Y축의 시점 변경
		AddControllerPitchInput(NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// 카메라 시점이 고정이여서 필요 없음
		break;
	}
}

// 마우스를 왼쪽 오른쪽 방향으로 움직였을 때, 해당 함수를 호출하고 카메라 방향 변경
void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// X축의 시점 변경
		AddControllerYawInput(NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// 카메라 시점이 고정이여서 필요 없음
		break;
	}
}

// 컨트롤 모드에 따라 설정 변경
void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	// 새롭게 변경된 컨트롤 모드 저장
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{

	// GTA 모드의 컨트롤 방식 적용
	case EControlMode::GTA:
		// SpringArm->TargetArmLength = 450.0f;
		// SpringArm->SetRelativeRotation(FRotator::ZeroRotator);

		// SpringArm 거리와 Camera 회전 목표값 설정
		ArmLengthTo = 450.0f;

		// SpringArm은 사용자의 마우스 움직임에 따라 회전하도록 설정
		SpringArm->bUsePawnControlRotation = true;

		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;

		SpringArm->bDoCollisionTest = true;

		// 마우스 좌우 움직임만 카메라에 영향이 가는 설정을 해제
		bUseControllerRotationYaw = false;

		// 캐릭터가 움직이는 방향으로 캐릭터를 자동으로 회전시킴 -> CharacterMovement로 설정
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// 캐릭터 부드럽게 회전하도록 설정
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	// 디아블로 모드의 컨트롤 방식 적용
	case EControlMode::DIABLO:
		// SpringArm->TargetArmLength = 800.0f;
		// SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

		// SpringArm 거리와 Camera 회전 목표값 설정
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		
		// SpringArm은 사용자의 마우스 움직임에 따라 회전하지 않도록 설정
		SpringArm->bUsePawnControlRotation = false;

		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;

		// 컬리전과 연동되지 않도록 설정
		SpringArm->bDoCollisionTest = false;

		// 마우스가 Z축으로 움직일 때, 카메라가 회전하도록 설정
		// 캐릭터 부드럽게 회전하도록 설정
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	// NPC 모드의 컨트롤 방식 적용
	case EControlMode::NPC:

		// 자연스럽게 회전할 수 있도록 설정
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// 카메라 뷰 모드를 변경하는 이벤트를 실행할 때, 호출되는 함수
void AABCharacter::ViewChange()
{
	// 서로 반대의 모드가 선택되도록 설정
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 카메라 회전을 현재 캐릭터의 회전값으로 설정
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;

	case EControlMode::DIABLO:
		// 카메라 회전을 SpringArm 회전값으로 설정
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

// 공격 키 입력 -> 밑에 함수의 else로 호출 -> 공격 키 또 입력 -> Notify_NextAttack? 함수 호출로 검사 -> 다음 공격 순으로 진행

// 마우스 클릭했을 때, 공격하는 함수 호출
void AABCharacter::Attack()
{
	//ABLOG_S(Warning);

	// 공격 중
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	
	// 공격 중이 아님.
	else
	{
		// 현재 콤보 번호가 0번일 때만, 실행
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

	//// 애니메이션 인스턴스 얻기
	//auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	//// 애니메이션 인스턴스가 정상적으로 캐스팅되고 null이 아닐 경우에만 진행
	//if (nullptr == AnimInstance)
	//{
	//	return;
	//}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 애니메이션 인스턴스 가져오기
	// auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	// AnimInstance가 있는지 확인하고 없으면, 바로 현재 함수를 종료
	ABCHECK(nullptr != ABAnim);

	// OnMontageEnded 델리게이트는 UAnimInstance에 선언되어 있음.
	// 블루프린트와 호환되는 성질 외에도 여러 개의 함수를 받을 수 있어 행동이 종료되면, 등록된 모든 함수들에게 모두 알려주는 기능도 포함.
	// 이를 멀티캐스트 델리게이트(Multicast Delegate)라고도 함.
	
	// DECLARE_DYNAMIC_MULTICAST_TwoParams(FOnMontageEndedMCDelegate, UAnimMontage* Montage, bool bIsInterrupted);

	// 시그니처(Signature) : 언리얼 엔진에서 델리게이트의 선언은 언리얼이 제공하는 매크로를 통해 정의. 이렇게 정의된 델리게이트 형식을 시그니처라고 함.

	// 두 가지 기능이 있는 OnMontageEnded 델리게이트 : 다이내믹 멀티캐스트 델리게이트
	// 다이내믹 멀티캐스트 델리게이트는 AddDynamic 함수를 사용할 수 있는데 이 함수는 C++ 인텔리센스에 검색되지 않는다.
	// 인텔리센스를 무시하고 타이핑해도 컴파일에 문제 없음.

	// 애님 인스턴스의 OnMontageEnded 델리게이트와 우리가 선언한 OnAttackMontageEnded를 연결
	// 델리게이트가 발동할 때까지 애니메이션 시스템에 몽타주 재생 명령을 내리지 못하게 폰 로직에서 방지
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	// ======= 람다식 =======

	// 람다 소개자(Lambda Introducer) : []로 표시, 람다 구문이 참조할 환경을 지정.
	// 람다 함수가 참조할 환경을 캡처(Capture)라고 함.
	// 여기서는 현재 람다 구문이 인스턴스의 관련 멤버 함수와 변수를 사용하므로 캡처 환경을 this로 지정.

	// 파라미터 리스트(Parameter List) : 람다 함수가 사용할 파라미터를 지정하는 구문.
	// 우리가 사용할 델리게이트는 함수 인자가 없으므로 빈 괄호를 사용.

	// 후행 반환 타입 (Trailing Return Type) : -> 기호를 사용한 후, 람다 함수가 반환할 타입을 지정.
	// 우리가 사용할 델리게이트는 반환값이 없으므로 void를 사용.

	// 람다 함수 구문(Lambda Body) : {}로 캡처 환경을 사용한 람다 함수의 로직을 넣어줌.
	// 앞서 람다 소개자의 캡처를 this로 지정했기 때문에 멤버 변수와 함수에 자유롭게 접근 가능.

	// 다음 공격 확인하는 Notify가 호출될 때, 사용할 람다식
	ABAnim->OnNextAttackCheck.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// 공격할 때, 탐지에 사용할 도형 추가
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	//// 캐릭터 체력이 0 됐을 때, 함수 호출
	//CharacterStat->OnHPIsZero.AddLambda([this]()->void {
	//	ABLOG(Warning, TEXT("OnHPIsZero"));
	//	// 죽는 애니메이션 재생
	//	ABAnim->SetDeadAnim();
	//	// 컬리전 비활성화
	//	SetActorEnableCollision(false);
	//});

	//// HP UI Casting 하기
	//auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	//if (nullptr != CharacterWidget)
	//{
	//	// HP UI에 새로운 캐릭터 상태 정보 추가
	//	CharacterWidget->BindCharacterStat(CharacterStat);
	//}
	
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrfupted)
{
	// 현재 공격중이지 않다면, return을 통해서 함수를 바로 종료
	ABCHECK(IsAttacking);

	// 이전에 공격 상태였다면
	ABCHECK(CurrentCombo > 0);
	// 애니메이션 몽타주가 끝났으므로, 공격 상태 종료
	IsAttacking = false;
	AttackEndComboState();

	// 공격이 종료됐으므로 해당 멀티캐스트 델리게이트에 등록된 함수를 모두 호출
	OnAttackEnd.Broadcast();
}

// 콤보 공격이 시작할 때, 관련 속성 지정
void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	// 현재 콤보 번호가 0 ~ MaxCombo - 1 사이에 존재한다면 다음으로 이동하고
	// 거짓이라면 해당 함수를 즉각 반환.
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	// 현재 콤보 번호를 설정하는데 1 ~ MaxCombo 사이의 값으로만 존재하도록 설정
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// 콤보 공격을 종료할 때, 사용
void AABCharacter::AttackEndComboState()
{
	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

// 공격 확인
void AABCharacter::AttackCheck()
{
	// 마지막 무기 사거리 가져오기
	float FinalAttackRange = GetFinalAttackRange();

	// HitReuslt 구조체
	FHitResult HitResult;
	// Params
	FCollisionQueryParams Params(NAME_None, false, this);

	// 캡슐 타입으로 충돌 확인
	// 끝 부분은 50cm 반구체, 가운데는 200cm의 원기둥
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

// 디버그 모드
#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	// 캐릭터가 바라보는 시선과 캡슐의 상단 방향(Z 방향)이 같음.
	// 따라서, 캐릭터 시선 벡터를 MakeFromZ로 해주면, 캡슐의 상단으로 향하는 벡터임.
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	// 충돌 물체가 있는 경우는 초록색, 없는 경우는 빨간색
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	// 충돌 물체가 감지되면, 이름 출력
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			// 충돌 물체에게 데미지를 줌.
			// 50.0의 데미지를 주고 데미지 전달 주체는 현재 입력을 내린 플레이어 컨트롤러,
			// 데미지 전달 도구는 현재 캐릭터 객체로 설정

			// 현재 캐릭터는 플레이어가 입력한 도구이므로 데미지를 가한 진정한 가해자는 폰이 아니라 폰에게 명령을 내릭 플레이어 컨트롤러
			FDamageEvent DamageEvent;
			// 현재 캐릭터 레벨에 따른 데미지를 상대에게 줌.
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}

	// Garbage Collection
	// 메모리에 있는 언리얼 오브젝트가 사용 중인지 아닌지를 주기적으로 검사.
	// 사용중이지 않은 물체를 발견하면 메모리에서 자동 제거.

	// 언리얼 오브젝트가 사용 중인지 여부는 다른 언리얼 오브젝트가 해당 오브젝트를
	// 참조하는지로 판단

	// FHitResult의 멤버 변수 Actor의 선언이 일반 참조로 선언된다면, 해당 함수에서의 참조로 인해
	// 제거되어야 할 액터가 메모리에 그대로 남아 있는 문제 발생.

	// 위 문제를 해결하기 위해 TWeakObjectPtr (약포인터) 방식으로 멤버 변수를 선언.
	// 약 포인터는 자유롭게 포인터 정보를 전달할 수 있음.
	// 약 포인터로 지정된 액터에 접근하려면, IsValid 함수를 사용해 사용하려는 액터가 유효한지 먼저 점검하고 사용.
}

// 데미지를 입었을 때 호출
float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	// 부모 클래스인 AActor에 기본적인 데미지 관련 로직이 구현되어 있어 Super 키워드를 사용해 부모 클래스의 로직을 먼저 실행해야 함.
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	// 캐릭터가 유효한 데미지를 입으면, 죽은 상태로 설정
	CharacterStat->SetDamage(FinalDamage);

	// 데미지를 받은 현재 캐릭터가 죽은 상태일 때
	if (CurrentState == ECharacterState::DEAD)
	{
		// 데미지 가해자가 플레이어인 경우
		if (EventInstigator->IsPlayerController())
		{
			auto ABPlayerController = Cast<AABPlayerController>(EventInstigator);
			ABCHECK(nullptr != ABPlayerController, 0.0f);
			// NPC의 경험치가 플레이어 컨트롤러를 통해서 전달되도록 실행
			ABPlayerController->NPCKill(this);
		}
	}

	return FinalDamage;
}

// 무기 설정할 수 있는지에 대한 여부
bool AABCharacter::CanSetWeapon()
{
	return true;
}

// 무기 설정
void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	// 새로운 무기가 존재할 경우에만 작동
	ABCHECK(nullptr != NewWeapon);

	// 기존 무기가 존재할 경우, 삭제
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	// 새로운 무기 추가
	FName WeaponSocket(TEXT("hand_rSocket"));
	
	if (nullptr != NewWeapon)
	{
		// 새로운 무기를 캐릭터의 소켓에 붙이고 Owner 설정
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

// 소유되었을 때, 호출하는 함수
void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//// 플레이어가 조종하는 캐릭터인 경우
	//if (IsPlayerControlled())
	//{
	//	SetControlMode(EControlMode::DIABLO);
	//	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//}
	//// NPC인 경우
	//else
	//{
	//	SetControlMode(EControlMode::NPC);
	//	// 플레이어보다 속도가 느리도록 설정
	//	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//}


}

// Config 파일로부터 애셋 경로를 가져와 로딩을 완료할 때, 호출할 함수
// 해당 함수에서 핸들러로부터 애셋을 캐스팅해 캐릭터 스켈레탈 메시로 등록
void AABCharacter::OnAssetLoadCompleted()
{
	// 비동기로 가져온 애셋을 캐스팅
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());

	// 핸들러 안에 있는 내용은 초기화
	AssetStreamingHandle.Reset();

	// 스켈레탈 메시를 정상적으로 가져온 경우 등록
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	// 캐릭터 상태를 준비 상태로 설정
	SetCharacterState(ECharacterState::READY);
}

// 캐릭터 상태 설정
void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	// 상태가 같으면 반환
	ABCHECK(CurrentState != NewState);
	
	CurrentState = NewState;

	switch (CurrentState)
	{
	// LOADING
	case ECharacterState::LOADING:
	{
		// 플레이어일 경우, 로딩 중이므로 Player Controller 입력받지 못하도록 설정
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);

			// HUD 위젯과 캐릭터 스텟 컴포넌트 연결
			// GetHUDWidget()으로 Widget을 가져오기 전에 해당 함수가 실행되어 null을 반환하는 문제가 발생해
			// 마지막에 Delay를 통해서 실행되도록 설정
			//ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			// TimerManger로 Delay 걸기
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("DelayBindCharacterStat"));

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);

			// null 반환됨.
			ABLOG(Warning, TEXT("Get HUD Widget : %s"), ABPlayerController->GetHUDWidget());

			// Player State 설정
			auto ABPlayerState = Cast<AABPlayerState>(PlayerState);
			ABCHECK(nullptr != ABPlayerState);
			// Actor Component에 플레이어 레벨 재설정
			CharacterStat->SetNewLevel(ABPlayerState->GetCharacterLevel());
		}
		// NPC인 경우, 현재 게임 스코어를 게임 모드에게 질의하고, 이를 기반으로 레벨 값 설정
		else
		{
			// 게임 모드 가져오기
			auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != ABGameMode);
			// 레벨 계산
			int32 TargetLevel = FMath::CeilToInt(((float)ABGameMode->GetScore() * 0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);

			ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			// NPC 캐릭터 레벨 설정
			CharacterStat->SetNewLevel(FinalLevel);
		}
		// 캐릭터를 숨기기
		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;

		break;
	}

	// READY
	case ECharacterState::READY:
	{
		// 캐릭터를 보이도록 활성화
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;
		// 체력이 0일 때, 델리게이트 함수 추가
		CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
			SetCharacterState(ECharacterState::DEAD);
		});

		// 캐릭터 체력 위젯 설정
		auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		// 플레이어 설정
		if (bIsPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(ABPlayerController);
		}
		// AI 캐릭터로 설정
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			ABAIController->RunAI();
		}
		break;
	}
	// DEAD
	case ECharacterState::DEAD:
	{
		// 체력 위젯만 숨기고 죽는 애니메이션 재생
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		ABAnim->SetDeadAnim();
		bCanBeDamaged = false;

		// 플레이어 입력 비활성화
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);
		}
		// AI인 경우, Behavior Tree 비활성화
		else
		{
			ABAIController->StopAI();
		}
		
		// 타이머 핸들러로 타이머실행
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void
		{
			// 플레이어인 경우, 결과 창 출력
			if (bIsPlayer)
			{
				ABPlayerController->ShowResultUI();
			}
			// AI인 경우, 제거
			else
			{
				Destroy();
			}
		}), DeadTimer, false);
		break;
	}
	}
}

// 캐릭터 상태 가져오기
ECharacterState AABCharacter::GetCharacterState() const
{
	return CurrentState;
}

// ABController의 GetHUDWidget을 BeginPlay로 바로 호출하지 않아 null값이 반환되는 것을 방지
// HUD 위젯과 캐릭터 스텟 컴포넌트 연결
void AABCharacter::DelayBindCharacterStat()
{
	ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);
	CharacterStat->OnHPChanged.Broadcast();
}

// 현재 플레이어 경험치량 가져오기
int32 AABCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

// 최종 공격 사거리 가져오기
float AABCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

// 최종 공격력 가져오기
float AABCharacter::GetFinalAttackDamage() const
{
	// 무기 공격력과 속성치 가져와서 계산 후, 반환
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	return AttackDamage * AttackModifier;
}