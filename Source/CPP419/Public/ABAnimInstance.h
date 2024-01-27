// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// 여러 개의 함수가 등록되도록 하는 것은 멀티캐스트 델리게이트
// 멀티캐스트 델리게이트에 등록된 모든 함수를 호출하는 멀티캐스트 델리게이트 명령은 Broadcast다.

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
	// 생성자
	UABAnimInstance();

	// 애니메이션 인스턴스에서의 Tick 이벤트 역할
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 애니메이션 몽타주 재생 함수
	void PlayAttackMontage();

	// 점프 공격 몽타주 섹션
	void JumpToAttackMontageSection(int32 NewSection);

public:
	// 델리게이트 객체
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	// 캐릭터를 죽음 상태로 설정
	void SetDeadAnim() { IsDead = true; }


private:

	// 아래의 멤버 함수를 언리얼 런타임이 찾을 수 있도록 UFUNCTION 매크로가 지정되어야 함.
	// AnimMontage나 AnimSequence에서 Notify가 호출되면, 언리얼 엔진은 이를 보고 자동으로 AnimInstance Class의 'AnimNotify_노티파이명'이라는 이름의 멤버 함수를 찾아서 호출.
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	// 위와 동일
	UFUNCTION()
		void AnimNotify_NextAttackCheck();

private:
	// EditAnywehre : 애니메이션 블루프린트의 애님 그래프를 사용할 수 있도록
	// BlueprintReadOnly : 블루프린트에서 접근 가능하지만, 오직 읽기만 가능
	// BlueprintReadWrite : 블루프린트에서 접근 가능하고 읽기와 쓰기 모두 가능
	// Meta = (AllowPrivateAccess = true) :  접근 지정자가 private지만, 외부에서 접근할 수 있도록 함. 하지만, 은닉화는 또 됨.
	// DefaultsOnly : 클래스의 기본값을 담당하는 블루프린트 편집 화면에서만 보여짐. <- VisibleDefaultsOnly
	// InstanceOnly : 인스턴스의 속성을 보여주는 에디터 뷰포트에서만 보여짐.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	
	// 공중에 있는지 판단
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	// 연속 공격 몽타주 애셋
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	
	// 현재 공격 몽타주 섹션 이름 가져오기
	FName GetAttackMontageSectionName(int32 Section);

	// 현재 캐릭터가 죽었는지 판단
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
};
