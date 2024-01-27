// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	// 초기화
	CurrentPawnSpeed = 0.0f; 
	IsInAir = false;
	IsDead = false;

	// 연속 공격 애니메이션 몽타주 대입
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 폰에 접근
	auto Pawn = TryGetPawnOwner();

	// if(::IsValid(객체)는 객체가 유효한지 검사하는 역할 - 유효성 검사를 통과한 객체 사용 가능
	// 명시적으로 검사하여 안전한 접근을 보장, 객체가 null 포인터일 경우나 이미 파괴된 객체에 접근하는 것을 방지하는데 도움.

	// if(객체)
	// null포인터 또는 유효하지 않은 상태인 경우에만 false 반환
	// 유효성 검사를 수행하지 않고 객 체의 상태에 따라 조건을 평가

	// 전자 방식이 유효성 검사를 하기 때문에 더욱 더 안전함.
	// 언리얼 엔진은 객체의 수명 주기가 관리되는데 이러한 객체에 접근할 때 항상 유효성을 확인하는 것이 좋음.

	// 폰에 접근을 실패하면, 반환
	if (!::IsValid(Pawn)) return;

	// 죽지 않은 상태에서만 작동
	if(!IsDead)
	{
		// 폰의 속력 값을 대입
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// Pawn이 Character 클래스일 때만 진행
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// 현재 캐릭터가 떨어지고 있는지를 확인
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

// 연속 공격 몽타주 재생
void UABAnimInstance::PlayAttackMontage()
{
	// 델리게이트에 의해 공격의 시작과 감지되므로 아래 조건문은 사용하지 않아도 무방

	//// 연속 공격 몽타주가 재생 중이지 않으면, 해당 몽타주를 재생
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}

	// 죽지 않은 경우에만 실행
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// 죽지 않은 경우에만 실행
	ABCHECK(!IsDead);
	// 공격 몽타주가 현재 재생되고 있는지 확인
	ABCHECK(Montage_IsPlaying(AttackMontage));

	// 해당 몽타주 섹션으로 이동
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	// 멀티캐스트 델리게이트에 등록된 함수를 모두 호출
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	// Section이 범위 안에 존재한다면, 밑으로 이동하고 그렇지 않으면 NAME_None을 출력
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}