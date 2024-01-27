// User : Jeonghoe22 : Study by '�̵���� �𸮾� C++ ���Ӱ����� ����'

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

	// ������Ʈ ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	// ===== ���� ���� ���� ======
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	// ü�� UI�� ĳ���Ϳ� ���̱�
	HPBarWidget->SetupAttachment(GetMesh());

	// �⺻�� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// �ִϸ��̼� �ν��Ͻ� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimationBlueprint.WarriorAnimationBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);

	}

	// ��Ʈ�� ��带 0������ ����
	SetControlMode(EControlMode::DIABLO);

	// �ε巴�� ������ ��, SpringArm�� Camera �ӵ� ����
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// ���� ���� ����
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	// ���� ���� ���°� �ƴ϶�� ����
	IsAttacking = false;

	// �޺� ���� �ִ�ġ �� �޺� ���� ���·� ����
	MaxCombo = 4;
	AttackEndComboState();

	// ĸ�� ������Ʈ�� �ݸ��� ������ ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// ���� ��Ÿ��� ���� ����
	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	// ===== ü�� UI =====
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// ȭ���� �׻� �÷��̾ ���� �ٶ󺸵��� �ϱ� ���� Screen���� ����.
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		// ü�� UI Ŭ���� ���� �� ��� ���� ����
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// AI Controller ����
	AIControllerClass = AABAIController::StaticClass();
	// ���忡 �ְų� ������ ��, �ڵ����� AI Controller�� Ȱ��ȭ�ǵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// CPP419Setting ����� ABCharacterSetting.h�κ��� ������ ������ �� �ִ��� Ȯ��
	//auto DefaultSetting = GetDefault<UABCharacterSetting>();

	//if (DefaultSetting->CharacterAssets.Num() > 0)
	//{
	//	for (auto CharacterAsset : DefaultSetting->CharacterAssets)
	//	{
	//		ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
	//	}
	//}

	// ĳ���� ���̷�Ż �޽� ��ȣ
	AssetIndex = 4;
	// ĳ���� ���� ����
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;

	// ���� Ÿ�̸� ����
	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//// �÷��̾ ��Ʈ������ ���� ���, �ּ� ��θ� config�� DefaultCPP419���� ������ �ΰ��� ���� �񵿱�� �ּ� �ε��ϱ�
	//if (!IsPlayerControlled())
	//{
	//	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	//	
	//	// �ּ� ��� �� �������� �ϳ� ����
	//	int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	//	CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

	//	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	//	if (nullptr != ABGameInstance)
	//	{
	//		// �ش� �ּ��� �ε��ϰ� �Ϸ�� �Լ��� ȣ��
	//		AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	//	}
	//}

	// ���� ĳ���͸� �÷��̾ �����ϴ��� Ȯ��
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		// �÷��̾��� ���, �÷��̾� ��Ʈ�ѷ��� ĳ����
		ABPlayerController = Cast<AABPlayerController>(GetController());
		ABCHECK(nullptr != ABPlayerController);
	}
	else
	{
		// AI�� ���, AI ��Ʈ�ѷ��� ĳ����
		ABAIController = Cast<AABAIController>(GetController());
		ABCHECK(nullptr != ABAIController);
	}

	// config�� DefaultCPP419 ������ ������.
	auto DefaultSetting = GetDefault<UABCharacterSetting>();

	// �÷��̾��� ���, ������ ������ ĳ���� ��ȣ�� ĳ���� ����
	if (bIsPlayer)
	{
		auto ABPlayerState = Cast<AABPlayerState>(PlayerState);
		ABCHECK(nullptr != ABPlayerState);
		AssetIndex = ABPlayerState->GetCharacterIndex();
	}
	// AI�� ��쿡�� config ������ �ּ� ��� �� �������� �ϳ� ��������
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	// ĳ���� ���̷�Ż �޽� ����
	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	// GameInstance ��������
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);
	// �񵿱� �ε�
	AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	// ĳ���� ���¸� Loading���� ����
	SetCharacterState(ECharacterState::LOADING);
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SpringArm�� �Ÿ��� �ε巴�� ����
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	// ��� �������� ���� ���� ���
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		break;

	case EControlMode::DIABLO:
		// ��� �����, ī�޶� �ε巴�� ȸ���ǵ��� ����
		SpringArm->RelativeRotation = FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);
		break;
	}

	// Ű���带 ���� �̵� ���� ���
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// �ٸ� ȣ�� �Լ����� �̹� ����
		break;

	case EControlMode::DIABLO:
		// �̵� ���� ũ�� ������ 0���� Ŭ��
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// ĳ���� �̵� ���� ����
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());

			// ĳ���� �����̴� ���� �ֱ�
			AddMovementInput(DirectionToMove);
		}
		break;
	}

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Ű���� �̺�Ʈ�� ����
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	// ���콺 �̺�Ʈ�� ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);

	// ī�޶� �� ���� �̺�Ʈ
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);

	// ���� �߰� : Character Ŭ������ �̹� Jump �Լ� �߰��Ǿ� ����
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// ���콺 ���� Ŭ�� : ����
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

// ���Ʒ� ����Ű�� ������ ��, �ش� �Լ��� ȣ���ϰ� ĳ���Ͱ� �̵��ϵ��� ����
void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// ��Ʈ�� ȸ���� = ī�޶� �ٶ󺸰� �ִ� ������ �̵�
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

		// �������� ĳ���� �̵�
		// AddMovementInput(GetActorForwardVector(), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// X�� �̵� ������ ����
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

// ���� ������ ����Ű�� ������ ��, �ش� �Լ��� ȣ���ϰ� ĳ���Ͱ� �̵��ϵ��� ����
void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// ��Ʈ�� ȸ���� = ī�޶� �ٷκ��� �ִ� ���� ������ �̵�
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

		// �������� ĳ���� �̵�
		// AddMovementInput(GetActorRightVector(), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// Y�� �̵� ������ ����
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

// ���콺�� ���Ʒ� �������� �������� ��, �ش� �Լ��� ȣ���ϰ� ī�޶� ���� ����
void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// Y���� ���� ����
		AddControllerPitchInput(NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// ī�޶� ������ �����̿��� �ʿ� ����
		break;
	}
}

// ���콺�� ���� ������ �������� �������� ��, �ش� �Լ��� ȣ���ϰ� ī�޶� ���� ����
void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// X���� ���� ����
		AddControllerYawInput(NewAxisValue);
		break;

	case EControlMode::DIABLO:
		// ī�޶� ������ �����̿��� �ʿ� ����
		break;
	}
}

// ��Ʈ�� ��忡 ���� ���� ����
void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	// ���Ӱ� ����� ��Ʈ�� ��� ����
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{

	// GTA ����� ��Ʈ�� ��� ����
	case EControlMode::GTA:
		// SpringArm->TargetArmLength = 450.0f;
		// SpringArm->SetRelativeRotation(FRotator::ZeroRotator);

		// SpringArm �Ÿ��� Camera ȸ�� ��ǥ�� ����
		ArmLengthTo = 450.0f;

		// SpringArm�� ������� ���콺 �����ӿ� ���� ȸ���ϵ��� ����
		SpringArm->bUsePawnControlRotation = true;

		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;

		SpringArm->bDoCollisionTest = true;

		// ���콺 �¿� �����Ӹ� ī�޶� ������ ���� ������ ����
		bUseControllerRotationYaw = false;

		// ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ����� ȸ����Ŵ -> CharacterMovement�� ����
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// ĳ���� �ε巴�� ȸ���ϵ��� ����
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	// ��ƺ�� ����� ��Ʈ�� ��� ����
	case EControlMode::DIABLO:
		// SpringArm->TargetArmLength = 800.0f;
		// SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

		// SpringArm �Ÿ��� Camera ȸ�� ��ǥ�� ����
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		
		// SpringArm�� ������� ���콺 �����ӿ� ���� ȸ������ �ʵ��� ����
		SpringArm->bUsePawnControlRotation = false;

		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;

		// �ø����� �������� �ʵ��� ����
		SpringArm->bDoCollisionTest = false;

		// ���콺�� Z������ ������ ��, ī�޶� ȸ���ϵ��� ����
		// ĳ���� �ε巴�� ȸ���ϵ��� ����
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	// NPC ����� ��Ʈ�� ��� ����
	case EControlMode::NPC:

		// �ڿ������� ȸ���� �� �ֵ��� ����
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// ī�޶� �� ��带 �����ϴ� �̺�Ʈ�� ������ ��, ȣ��Ǵ� �Լ�
void AABCharacter::ViewChange()
{
	// ���� �ݴ��� ��尡 ���õǵ��� ����
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// ī�޶� ȸ���� ���� ĳ������ ȸ�������� ����
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;

	case EControlMode::DIABLO:
		// ī�޶� ȸ���� SpringArm ȸ�������� ����
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

// ���� Ű �Է� -> �ؿ� �Լ��� else�� ȣ�� -> ���� Ű �� �Է� -> Notify_NextAttack? �Լ� ȣ��� �˻� -> ���� ���� ������ ����

// ���콺 Ŭ������ ��, �����ϴ� �Լ� ȣ��
void AABCharacter::Attack()
{
	//ABLOG_S(Warning);

	// ���� ��
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	
	// ���� ���� �ƴ�.
	else
	{
		// ���� �޺� ��ȣ�� 0���� ����, ����
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

	//// �ִϸ��̼� �ν��Ͻ� ���
	//auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	//// �ִϸ��̼� �ν��Ͻ��� ���������� ĳ���õǰ� null�� �ƴ� ��쿡�� ����
	//if (nullptr == AnimInstance)
	//{
	//	return;
	//}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// �ִϸ��̼� �ν��Ͻ� ��������
	// auto AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	// AnimInstance�� �ִ��� Ȯ���ϰ� ������, �ٷ� ���� �Լ��� ����
	ABCHECK(nullptr != ABAnim);

	// OnMontageEnded ��������Ʈ�� UAnimInstance�� ����Ǿ� ����.
	// �������Ʈ�� ȣȯ�Ǵ� ���� �ܿ��� ���� ���� �Լ��� ���� �� �־� �ൿ�� ����Ǹ�, ��ϵ� ��� �Լ��鿡�� ��� �˷��ִ� ��ɵ� ����.
	// �̸� ��Ƽĳ��Ʈ ��������Ʈ(Multicast Delegate)��� ��.
	
	// DECLARE_DYNAMIC_MULTICAST_TwoParams(FOnMontageEndedMCDelegate, UAnimMontage* Montage, bool bIsInterrupted);

	// �ñ״�ó(Signature) : �𸮾� �������� ��������Ʈ�� ������ �𸮾��� �����ϴ� ��ũ�θ� ���� ����. �̷��� ���ǵ� ��������Ʈ ������ �ñ״�ó��� ��.

	// �� ���� ����� �ִ� OnMontageEnded ��������Ʈ : ���̳��� ��Ƽĳ��Ʈ ��������Ʈ
	// ���̳��� ��Ƽĳ��Ʈ ��������Ʈ�� AddDynamic �Լ��� ����� �� �ִµ� �� �Լ��� C++ ���ڸ������� �˻����� �ʴ´�.
	// ���ڸ������� �����ϰ� Ÿ�����ص� �����Ͽ� ���� ����.

	// �ִ� �ν��Ͻ��� OnMontageEnded ��������Ʈ�� �츮�� ������ OnAttackMontageEnded�� ����
	// ��������Ʈ�� �ߵ��� ������ �ִϸ��̼� �ý��ۿ� ��Ÿ�� ��� ����� ������ ���ϰ� �� �������� ����
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	// ======= ���ٽ� =======

	// ���� �Ұ���(Lambda Introducer) : []�� ǥ��, ���� ������ ������ ȯ���� ����.
	// ���� �Լ��� ������ ȯ���� ĸó(Capture)��� ��.
	// ���⼭�� ���� ���� ������ �ν��Ͻ��� ���� ��� �Լ��� ������ ����ϹǷ� ĸó ȯ���� this�� ����.

	// �Ķ���� ����Ʈ(Parameter List) : ���� �Լ��� ����� �Ķ���͸� �����ϴ� ����.
	// �츮�� ����� ��������Ʈ�� �Լ� ���ڰ� �����Ƿ� �� ��ȣ�� ���.

	// ���� ��ȯ Ÿ�� (Trailing Return Type) : -> ��ȣ�� ����� ��, ���� �Լ��� ��ȯ�� Ÿ���� ����.
	// �츮�� ����� ��������Ʈ�� ��ȯ���� �����Ƿ� void�� ���.

	// ���� �Լ� ����(Lambda Body) : {}�� ĸó ȯ���� ����� ���� �Լ��� ������ �־���.
	// �ռ� ���� �Ұ����� ĸó�� this�� �����߱� ������ ��� ������ �Լ��� �����Ӱ� ���� ����.

	// ���� ���� Ȯ���ϴ� Notify�� ȣ��� ��, ����� ���ٽ�
	ABAnim->OnNextAttackCheck.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// ������ ��, Ž���� ����� ���� �߰�
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	//// ĳ���� ü���� 0 ���� ��, �Լ� ȣ��
	//CharacterStat->OnHPIsZero.AddLambda([this]()->void {
	//	ABLOG(Warning, TEXT("OnHPIsZero"));
	//	// �״� �ִϸ��̼� ���
	//	ABAnim->SetDeadAnim();
	//	// �ø��� ��Ȱ��ȭ
	//	SetActorEnableCollision(false);
	//});

	//// HP UI Casting �ϱ�
	//auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	//if (nullptr != CharacterWidget)
	//{
	//	// HP UI�� ���ο� ĳ���� ���� ���� �߰�
	//	CharacterWidget->BindCharacterStat(CharacterStat);
	//}
	
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrfupted)
{
	// ���� ���������� �ʴٸ�, return�� ���ؼ� �Լ��� �ٷ� ����
	ABCHECK(IsAttacking);

	// ������ ���� ���¿��ٸ�
	ABCHECK(CurrentCombo > 0);
	// �ִϸ��̼� ��Ÿ�ְ� �������Ƿ�, ���� ���� ����
	IsAttacking = false;
	AttackEndComboState();

	// ������ ��������Ƿ� �ش� ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� �Լ��� ��� ȣ��
	OnAttackEnd.Broadcast();
}

// �޺� ������ ������ ��, ���� �Ӽ� ����
void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	// ���� �޺� ��ȣ�� 0 ~ MaxCombo - 1 ���̿� �����Ѵٸ� �������� �̵��ϰ�
	// �����̶�� �ش� �Լ��� �ﰢ ��ȯ.
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	// ���� �޺� ��ȣ�� �����ϴµ� 1 ~ MaxCombo ������ �����θ� �����ϵ��� ����
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// �޺� ������ ������ ��, ���
void AABCharacter::AttackEndComboState()
{
	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

// ���� Ȯ��
void AABCharacter::AttackCheck()
{
	// ������ ���� ��Ÿ� ��������
	float FinalAttackRange = GetFinalAttackRange();

	// HitReuslt ����ü
	FHitResult HitResult;
	// Params
	FCollisionQueryParams Params(NAME_None, false, this);

	// ĸ�� Ÿ������ �浹 Ȯ��
	// �� �κ��� 50cm �ݱ�ü, ����� 200cm�� �����
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

// ����� ���
#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	// ĳ���Ͱ� �ٶ󺸴� �ü��� ĸ���� ��� ����(Z ����)�� ����.
	// ����, ĳ���� �ü� ���͸� MakeFromZ�� ���ָ�, ĸ���� ������� ���ϴ� ������.
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	// �浹 ��ü�� �ִ� ���� �ʷϻ�, ���� ���� ������
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

	// �浹 ��ü�� �����Ǹ�, �̸� ���
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			// �浹 ��ü���� �������� ��.
			// 50.0�� �������� �ְ� ������ ���� ��ü�� ���� �Է��� ���� �÷��̾� ��Ʈ�ѷ�,
			// ������ ���� ������ ���� ĳ���� ��ü�� ����

			// ���� ĳ���ʹ� �÷��̾ �Է��� �����̹Ƿ� �������� ���� ������ �����ڴ� ���� �ƴ϶� ������ ����� ���� �÷��̾� ��Ʈ�ѷ�
			FDamageEvent DamageEvent;
			// ���� ĳ���� ������ ���� �������� ��뿡�� ��.
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}

	// Garbage Collection
	// �޸𸮿� �ִ� �𸮾� ������Ʈ�� ��� ������ �ƴ����� �ֱ������� �˻�.
	// ��������� ���� ��ü�� �߰��ϸ� �޸𸮿��� �ڵ� ����.

	// �𸮾� ������Ʈ�� ��� ������ ���δ� �ٸ� �𸮾� ������Ʈ�� �ش� ������Ʈ��
	// �����ϴ����� �Ǵ�

	// FHitResult�� ��� ���� Actor�� ������ �Ϲ� ������ ����ȴٸ�, �ش� �Լ������� ������ ����
	// ���ŵǾ�� �� ���Ͱ� �޸𸮿� �״�� ���� �ִ� ���� �߻�.

	// �� ������ �ذ��ϱ� ���� TWeakObjectPtr (��������) ������� ��� ������ ����.
	// �� �����ʹ� �����Ӱ� ������ ������ ������ �� ����.
	// �� �����ͷ� ������ ���Ϳ� �����Ϸ���, IsValid �Լ��� ����� ����Ϸ��� ���Ͱ� ��ȿ���� ���� �����ϰ� ���.
}

// �������� �Ծ��� �� ȣ��
float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	// �θ� Ŭ������ AActor�� �⺻���� ������ ���� ������ �����Ǿ� �־� Super Ű���带 ����� �θ� Ŭ������ ������ ���� �����ؾ� ��.
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	// ĳ���Ͱ� ��ȿ�� �������� ������, ���� ���·� ����
	CharacterStat->SetDamage(FinalDamage);

	// �������� ���� ���� ĳ���Ͱ� ���� ������ ��
	if (CurrentState == ECharacterState::DEAD)
	{
		// ������ �����ڰ� �÷��̾��� ���
		if (EventInstigator->IsPlayerController())
		{
			auto ABPlayerController = Cast<AABPlayerController>(EventInstigator);
			ABCHECK(nullptr != ABPlayerController, 0.0f);
			// NPC�� ����ġ�� �÷��̾� ��Ʈ�ѷ��� ���ؼ� ���޵ǵ��� ����
			ABPlayerController->NPCKill(this);
		}
	}

	return FinalDamage;
}

// ���� ������ �� �ִ����� ���� ����
bool AABCharacter::CanSetWeapon()
{
	return true;
}

// ���� ����
void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	// ���ο� ���Ⱑ ������ ��쿡�� �۵�
	ABCHECK(nullptr != NewWeapon);

	// ���� ���Ⱑ ������ ���, ����
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	// ���ο� ���� �߰�
	FName WeaponSocket(TEXT("hand_rSocket"));
	
	if (nullptr != NewWeapon)
	{
		// ���ο� ���⸦ ĳ������ ���Ͽ� ���̰� Owner ����
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

// �����Ǿ��� ��, ȣ���ϴ� �Լ�
void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//// �÷��̾ �����ϴ� ĳ������ ���
	//if (IsPlayerControlled())
	//{
	//	SetControlMode(EControlMode::DIABLO);
	//	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//}
	//// NPC�� ���
	//else
	//{
	//	SetControlMode(EControlMode::NPC);
	//	// �÷��̾�� �ӵ��� �������� ����
	//	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//}


}

// Config ���Ϸκ��� �ּ� ��θ� ������ �ε��� �Ϸ��� ��, ȣ���� �Լ�
// �ش� �Լ����� �ڵ鷯�κ��� �ּ��� ĳ������ ĳ���� ���̷�Ż �޽÷� ���
void AABCharacter::OnAssetLoadCompleted()
{
	// �񵿱�� ������ �ּ��� ĳ����
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());

	// �ڵ鷯 �ȿ� �ִ� ������ �ʱ�ȭ
	AssetStreamingHandle.Reset();

	// ���̷�Ż �޽ø� ���������� ������ ��� ���
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	// ĳ���� ���¸� �غ� ���·� ����
	SetCharacterState(ECharacterState::READY);
}

// ĳ���� ���� ����
void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	// ���°� ������ ��ȯ
	ABCHECK(CurrentState != NewState);
	
	CurrentState = NewState;

	switch (CurrentState)
	{
	// LOADING
	case ECharacterState::LOADING:
	{
		// �÷��̾��� ���, �ε� ���̹Ƿ� Player Controller �Է¹��� ���ϵ��� ����
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);

			// HUD ������ ĳ���� ���� ������Ʈ ����
			// GetHUDWidget()���� Widget�� �������� ���� �ش� �Լ��� ����Ǿ� null�� ��ȯ�ϴ� ������ �߻���
			// �������� Delay�� ���ؼ� ����ǵ��� ����
			//ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			// TimerManger�� Delay �ɱ�
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("DelayBindCharacterStat"));

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);

			// null ��ȯ��.
			ABLOG(Warning, TEXT("Get HUD Widget : %s"), ABPlayerController->GetHUDWidget());

			// Player State ����
			auto ABPlayerState = Cast<AABPlayerState>(PlayerState);
			ABCHECK(nullptr != ABPlayerState);
			// Actor Component�� �÷��̾� ���� �缳��
			CharacterStat->SetNewLevel(ABPlayerState->GetCharacterLevel());
		}
		// NPC�� ���, ���� ���� ���ھ ���� ��忡�� �����ϰ�, �̸� ������� ���� �� ����
		else
		{
			// ���� ��� ��������
			auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != ABGameMode);
			// ���� ���
			int32 TargetLevel = FMath::CeilToInt(((float)ABGameMode->GetScore() * 0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);

			ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			// NPC ĳ���� ���� ����
			CharacterStat->SetNewLevel(FinalLevel);
		}
		// ĳ���͸� �����
		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;

		break;
	}

	// READY
	case ECharacterState::READY:
	{
		// ĳ���͸� ���̵��� Ȱ��ȭ
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;
		// ü���� 0�� ��, ��������Ʈ �Լ� �߰�
		CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
			SetCharacterState(ECharacterState::DEAD);
		});

		// ĳ���� ü�� ���� ����
		auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		// �÷��̾� ����
		if (bIsPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(ABPlayerController);
		}
		// AI ĳ���ͷ� ����
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
		// ü�� ������ ����� �״� �ִϸ��̼� ���
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		ABAnim->SetDeadAnim();
		bCanBeDamaged = false;

		// �÷��̾� �Է� ��Ȱ��ȭ
		if (bIsPlayer)
		{
			DisableInput(ABPlayerController);
		}
		// AI�� ���, Behavior Tree ��Ȱ��ȭ
		else
		{
			ABAIController->StopAI();
		}
		
		// Ÿ�̸� �ڵ鷯�� Ÿ�̸ӽ���
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void
		{
			// �÷��̾��� ���, ��� â ���
			if (bIsPlayer)
			{
				ABPlayerController->ShowResultUI();
			}
			// AI�� ���, ����
			else
			{
				Destroy();
			}
		}), DeadTimer, false);
		break;
	}
	}
}

// ĳ���� ���� ��������
ECharacterState AABCharacter::GetCharacterState() const
{
	return CurrentState;
}

// ABController�� GetHUDWidget�� BeginPlay�� �ٷ� ȣ������ �ʾ� null���� ��ȯ�Ǵ� ���� ����
// HUD ������ ĳ���� ���� ������Ʈ ����
void AABCharacter::DelayBindCharacterStat()
{
	ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);
	CharacterStat->OnHPChanged.Broadcast();
}

// ���� �÷��̾� ����ġ�� ��������
int32 AABCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

// ���� ���� ��Ÿ� ��������
float AABCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

// ���� ���ݷ� ��������
float AABCharacter::GetFinalAttackDamage() const
{
	// ���� ���ݷ°� �Ӽ�ġ �����ͼ� ��� ��, ��ȯ
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	return AttackDamage * AttackModifier;
}