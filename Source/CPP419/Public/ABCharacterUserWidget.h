﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CPP419.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API UABCharacterUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 다음 캐릭터로 설정
	UFUNCTION(BlueprintCallable)
		void NextCharacter(bool bForward = true);

	//
	virtual void NativeConstruct() override;

protected:
	// 현재 캐릭터 인덱스 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 CurrentIndex;

	// 최대 인덱스 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 MaxIndex;
	
	// 변경할 스켈레탈 메시 컴포넌트
	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	// 이전 버튼
	UPROPERTY()
		class UButton* PrevButton;

	// 다음 버튼
	UPROPERTY()
		class UButton* NextButton;

	// 입력 닉네임 박스
	UPROPERTY()
		class UEditableTextBox* TextBox;

	// 확인 버튼
	UPROPERTY()
		class UButton* ConfirmButton;

private:
	// 이전 버튼 클릭
	UFUNCTION()
		void OnPrevClicked();

	// 다음 버튼 클릭
	UFUNCTION()
		void OnNextClicked();

	// 확인 버튼 클릭
	UFUNCTION()
		void OnConfirmClicked();
	
};
