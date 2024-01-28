// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

// 공격 종료됐을 때, 사용할 멀티캐스트 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class CPP419_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

	// 캐릭터 상태 설정
	void SetCharacterState(ECharacterState NewState);

	// 캐릭터 상태 가져오기
	ECharacterState GetCharacterState() const;

	// 현재 플레이어 경험치량 가져오기
	int32 GetExp() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called establish components before InitializeComponentes.
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when object takes damamge by outer
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 소유되었을 때, 호출하는 함수
	virtual void PossessedBy(AController* NewController) override;

	// 무기 설정할 수 있는지 여부
	bool CanSetWeapon();

	// 무기 설정
	void SetWeapon(class AABWeapon* NewWeapon);

	// 최종 공격 사거리 가져오기
	float GetFinalAttackRange() const;

	// 최종 공격력 가져오기
	float GetFinalAttackDamage() const;

	// 무기 클래스
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AABWeapon* CurrentWeapon;

	// Use weapon
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	// 캐릭터 상태 Actor Component
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UABCharacterStatComponent* CharacterStat;

	// Components
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	// 체력 UI Component
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	// 마우스 왼쪽 클릭했을 때, 공격하는 함수 실행
	void Attack();
	
	// 공격이 종료됐을 때, 호출할 멀티캐스트 델리게이트 객체
	FOnAttackEndDelegate OnAttackEnd;
	
private:

	// 방향 키 이벤트에 대해서 호출할 함수
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);

	// 마우스 방향 이벤트에 대해서 호출할 함수
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	// View Change 입력 이벤트가 실행할 때, 호출할 함수
	void ViewChange();

	// 공격 몽타주가 끝났을 때, 호출할 함수.
	// 델리게이트로 사용하기 위해 UFUNCTION() 매크로를 사용.
	// 블루프린트와 관련된 C++ 함수는 모두 UFUNCTION() 매크로를 사용해야 함.
	// 블루프린트 객체와 연동하는 델리게이트를 언리얼 엔진에서는 다이나믹 델리게이트(Dynamic Delegate)라고 함.
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 콤보 공격이 시작할 때, 관련 속성 지정
	void AttackStartComboState();

	// 콤보 공격을 종료할 때, 사용
	void AttackEndComboState();

	// 공격 확인
	void AttackCheck();

	// Config 파일로부터 애셋 경로를 가져와 로딩을 완료할 때, 호출할 함수
	// 해당 함수에서 핸들러로부터 애셋을 캐스팅해 캐릭터 스켈레탈 메시로 등록
	void OnAssetLoadCompleted();

	// ABController의 GetHUDWidget을 BeginPlay로 바로 호출하지 않아 null값이 반환되는 것을 방지
	// HUD 위젯과 캐릭터 스텟 컴포넌트 연결
	UFUNCTION()
		void DelayBindCharacterStat();

protected:

	// 컨트롤 모드 구분 열거형
	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	// 모드에 따른 컨트롤 모드 변경
	void SetControlMode(EControlMode NewControlMode);

	// 현재 컨트롤 모드 정보
	EControlMode CurrentControlMode = EControlMode::GTA;

	// 각 축의 입력을 조합해 보관
	FVector DirectionToMove = FVector::ZeroVector;

	// ------------- 컨트롤 모드 전환으로 부드럽게 움직이는 기능에 관한 변수 --------------

	// 변경할 목표 SpringArm 거리
	float ArmLengthTo = 0.0f;

	// 변경할 목표 Camera 회전값
	FRotator ArmRotationTo = FRotator::ZeroRotator;

	// 컨트롤 모드 전환으로 부드럽게 움직일 때, SpringArm 변경 속도
	float ArmLengthSpeed = 0.0f;

	// 컨트롤 모드 전환으로 부드럽게 움직일 때, Camera 회전 속도
	float ArmRotationSpeed = 0.0f;

private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	// 공격 중의 상태 여부
		bool IsAttacking;

	// 전방 선언 : 헤더 파일에 선언하는 것
	// 같은 모듈에 있는 다른 헤더 파일을 참조하지 않아도 되므로 상호 참조를 방지하고 코드 구조 관리 용이

	// 현재 캐릭터의 애니메이션 인스턴스
	UPROPERTY()
		class UABAnimInstance* ABAnim;

	// 다음 콤보로의 이동 가능 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	// 콤보 입력 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	// 현재 실행 중인 콤보 카운터
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	// 콤보 카운터의 최대치
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	// 공격 사거리
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	// AI가 공격할 수 있는 유효 거리
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	// 애셋 경로
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);

	// 비동기로 애셋 가져올 수 있는 핸들러
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	// 애셋 번호
	int32 AssetIndex = 0;

	// 현재 캐릭터 상태
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	// 현재 캐릭터를 플레이어가 조종하는지
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;

	// AI Controller의 클래스
	UPROPERTY()
		class AABAIController* ABAIController;

	// Player Controller 클래스
	UPROPERTY()
		class AABPlayerController* ABPlayerController;

	// 타이머를 발동시켜 죽은 이후에도 작동할 수 있는 코드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;

	// 사후 타이머 핸들러
	FTimerHandle DeadTimerHandle = {};
};
