// User : Jeonghoe22 : Study by '�̵���� �𸮾� C++ ���Ӱ����� ����'

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	// �ʱ�ȭ
	CurrentPawnSpeed = 0.0f; 
	IsInAir = false;
	IsDead = false;

	// ���� ���� �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ���� ����
	auto Pawn = TryGetPawnOwner();

	// if(::IsValid(��ü)�� ��ü�� ��ȿ���� �˻��ϴ� ���� - ��ȿ�� �˻縦 ����� ��ü ��� ����
	// ��������� �˻��Ͽ� ������ ������ ����, ��ü�� null �������� ��쳪 �̹� �ı��� ��ü�� �����ϴ� ���� �����ϴµ� ����.

	// if(��ü)
	// null������ �Ǵ� ��ȿ���� ���� ������ ��쿡�� false ��ȯ
	// ��ȿ�� �˻縦 �������� �ʰ� �� ü�� ���¿� ���� ������ ��

	// ���� ����� ��ȿ�� �˻縦 �ϱ� ������ ���� �� ������.
	// �𸮾� ������ ��ü�� ���� �ֱⰡ �����Ǵµ� �̷��� ��ü�� ������ �� �׻� ��ȿ���� Ȯ���ϴ� ���� ����.

	// ���� ������ �����ϸ�, ��ȯ
	if (!::IsValid(Pawn)) return;

	// ���� ���� ���¿����� �۵�
	if(!IsDead)
	{
		// ���� �ӷ� ���� ����
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// Pawn�� Character Ŭ������ ���� ����
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// ���� ĳ���Ͱ� �������� �ִ����� Ȯ��
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

// ���� ���� ��Ÿ�� ���
void UABAnimInstance::PlayAttackMontage()
{
	// ��������Ʈ�� ���� ������ ���۰� �����ǹǷ� �Ʒ� ���ǹ��� ������� �ʾƵ� ����

	//// ���� ���� ��Ÿ�ְ� ��� ������ ������, �ش� ��Ÿ�ָ� ���
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}

	// ���� ���� ��쿡�� ����
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// ���� ���� ��쿡�� ����
	ABCHECK(!IsDead);
	// ���� ��Ÿ�ְ� ���� ����ǰ� �ִ��� Ȯ��
	ABCHECK(Montage_IsPlaying(AttackMontage));

	// �ش� ��Ÿ�� �������� �̵�
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� �Լ��� ��� ȣ��
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	// Section�� ���� �ȿ� �����Ѵٸ�, ������ �̵��ϰ� �׷��� ������ NAME_None�� ���
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}