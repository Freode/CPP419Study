// User : Jeonghoe22 : Study by '�̵���� �𸮾� C++ ���Ӱ����� ����'

#pragma once

#include "CPP419.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// ���� ���� �Լ��� ��ϵǵ��� �ϴ� ���� ��Ƽĳ��Ʈ ��������Ʈ
// ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ���ϴ� ��Ƽĳ��Ʈ ��������Ʈ ����� Broadcast��.

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class CPP419_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// ������
	UABAnimInstance();

	// �ִϸ��̼� �ν��Ͻ������� Tick �̺�Ʈ ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// �ִϸ��̼� ��Ÿ�� ��� �Լ�
	void PlayAttackMontage();

	// ���� ���� ��Ÿ�� ����
	void JumpToAttackMontageSection(int32 NewSection);

public:
	// ��������Ʈ ��ü
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	// ĳ���͸� ���� ���·� ����
	void SetDeadAnim() { IsDead = true; }


private:

	// �Ʒ��� ��� �Լ��� �𸮾� ��Ÿ���� ã�� �� �ֵ��� UFUNCTION ��ũ�ΰ� �����Ǿ�� ��.
	// AnimMontage�� AnimSequence���� Notify�� ȣ��Ǹ�, �𸮾� ������ �̸� ���� �ڵ����� AnimInstance Class�� 'AnimNotify_��Ƽ���̸�'�̶�� �̸��� ��� �Լ��� ã�Ƽ� ȣ��.
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	// ���� ����
	UFUNCTION()
		void AnimNotify_NextAttackCheck();

private:
	// EditAnywehre : �ִϸ��̼� �������Ʈ�� �ִ� �׷����� ����� �� �ֵ���
	// BlueprintReadOnly : �������Ʈ���� ���� ����������, ���� �б⸸ ����
	// BlueprintReadWrite : �������Ʈ���� ���� �����ϰ� �б�� ���� ��� ����
	// Meta = (AllowPrivateAccess = true) :  ���� �����ڰ� private����, �ܺο��� ������ �� �ֵ��� ��. ������, ����ȭ�� �� ��.
	// DefaultsOnly : Ŭ������ �⺻���� ����ϴ� �������Ʈ ���� ȭ�鿡���� ������. <- VisibleDefaultsOnly
	// InstanceOnly : �ν��Ͻ��� �Ӽ��� �����ִ� ������ ����Ʈ������ ������.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	
	// ���߿� �ִ��� �Ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	// ���� ���� ��Ÿ�� �ּ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	
	// ���� ���� ��Ÿ�� ���� �̸� ��������
	FName GetAttackMontageSectionName(int32 Section);

	// ���� ĳ���Ͱ� �׾����� �Ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
};
