// User : Jeonghoe22 : Study by '�̵���� �𸮾� C++ ���Ӱ����� ����'

#pragma once

#include "CPP419.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

// ���� ������� ��, ����� ��Ƽĳ��Ʈ ��������Ʈ
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class CPP419_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

	// ĳ���� ���� ����
	void SetCharacterState(ECharacterState NewState);

	// ĳ���� ���� ��������
	ECharacterState GetCharacterState() const;

	// ���� �÷��̾� ����ġ�� ��������
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

	// �����Ǿ��� ��, ȣ���ϴ� �Լ�
	virtual void PossessedBy(AController* NewController) override;

	// ���� ������ �� �ִ��� ����
	bool CanSetWeapon();

	// ���� ����
	void SetWeapon(class AABWeapon* NewWeapon);

	// ���� ���� ��Ÿ� ��������
	float GetFinalAttackRange() const;

	// ���� ���ݷ� ��������
	float GetFinalAttackDamage() const;

	// ���� Ŭ����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AABWeapon* CurrentWeapon;

	// Use weapon
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	// ĳ���� ���� Actor Component
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UABCharacterStatComponent* CharacterStat;

	// Components
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	// ü�� UI Component
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	// ���콺 ���� Ŭ������ ��, �����ϴ� �Լ� ����
	void Attack();
	
	// ������ ������� ��, ȣ���� ��Ƽĳ��Ʈ ��������Ʈ ��ü
	FOnAttackEndDelegate OnAttackEnd;
	
private:

	// ���� Ű �̺�Ʈ�� ���ؼ� ȣ���� �Լ�
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);

	// ���콺 ���� �̺�Ʈ�� ���ؼ� ȣ���� �Լ�
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	// View Change �Է� �̺�Ʈ�� ������ ��, ȣ���� �Լ�
	void ViewChange();

	// ���� ��Ÿ�ְ� ������ ��, ȣ���� �Լ�.
	// ��������Ʈ�� ����ϱ� ���� UFUNCTION() ��ũ�θ� ���.
	// �������Ʈ�� ���õ� C++ �Լ��� ��� UFUNCTION() ��ũ�θ� ����ؾ� ��.
	// �������Ʈ ��ü�� �����ϴ� ��������Ʈ�� �𸮾� ���������� ���̳��� ��������Ʈ(Dynamic Delegate)��� ��.
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// �޺� ������ ������ ��, ���� �Ӽ� ����
	void AttackStartComboState();

	// �޺� ������ ������ ��, ���
	void AttackEndComboState();

	// ���� Ȯ��
	void AttackCheck();

	// Config ���Ϸκ��� �ּ� ��θ� ������ �ε��� �Ϸ��� ��, ȣ���� �Լ�
	// �ش� �Լ����� �ڵ鷯�κ��� �ּ��� ĳ������ ĳ���� ���̷�Ż �޽÷� ���
	void OnAssetLoadCompleted();

	// ABController�� GetHUDWidget�� BeginPlay�� �ٷ� ȣ������ �ʾ� null���� ��ȯ�Ǵ� ���� ����
	// HUD ������ ĳ���� ���� ������Ʈ ����
	UFUNCTION()
		void DelayBindCharacterStat();

protected:

	// ��Ʈ�� ��� ���� ������
	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	// ��忡 ���� ��Ʈ�� ��� ����
	void SetControlMode(EControlMode NewControlMode);

	// ���� ��Ʈ�� ��� ����
	EControlMode CurrentControlMode = EControlMode::GTA;

	// �� ���� �Է��� ������ ����
	FVector DirectionToMove = FVector::ZeroVector;

	// ------------- ��Ʈ�� ��� ��ȯ���� �ε巴�� �����̴� ��ɿ� ���� ���� --------------

	// ������ ��ǥ SpringArm �Ÿ�
	float ArmLengthTo = 0.0f;

	// ������ ��ǥ Camera ȸ����
	FRotator ArmRotationTo = FRotator::ZeroRotator;

	// ��Ʈ�� ��� ��ȯ���� �ε巴�� ������ ��, SpringArm ���� �ӵ�
	float ArmLengthSpeed = 0.0f;

	// ��Ʈ�� ��� ��ȯ���� �ε巴�� ������ ��, Camera ȸ�� �ӵ�
	float ArmRotationSpeed = 0.0f;

private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	// ���� ���� ���� ����
		bool IsAttacking;

	// ���� ���� : ��� ���Ͽ� �����ϴ� ��
	// ���� ��⿡ �ִ� �ٸ� ��� ������ �������� �ʾƵ� �ǹǷ� ��ȣ ������ �����ϰ� �ڵ� ���� ���� ����

	// ���� ĳ������ �ִϸ��̼� �ν��Ͻ�
	UPROPERTY()
		class UABAnimInstance* ABAnim;

	// ���� �޺����� �̵� ���� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	// �޺� �Է� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	// ���� ���� ���� �޺� ī����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	// �޺� ī������ �ִ�ġ
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	// ���� ��Ÿ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	// AI�� ������ �� �ִ� ��ȿ �Ÿ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	// �ּ� ���
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);

	// �񵿱�� �ּ� ������ �� �ִ� �ڵ鷯
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	// �ּ� ��ȣ
	int32 AssetIndex = 0;

	// ���� ĳ���� ����
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	// ���� ĳ���͸� �÷��̾ �����ϴ���
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;

	// AI Controller�� Ŭ����
	UPROPERTY()
		class AABAIController* ABAIController;

	// Player Controller Ŭ����
	UPROPERTY()
		class AABPlayerController* ABPlayerController;

	// Ÿ�̸Ӹ� �ߵ����� ���� ���Ŀ��� �۵��� �� �ִ� �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;

	// ���� Ÿ�̸� �ڵ鷯
	FTimerHandle DeadTimerHandle = {};
};
