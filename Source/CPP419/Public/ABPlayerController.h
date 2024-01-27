// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPP419.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CPP419_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 생성자
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void Possess(APawn* aPawn) override;

	// HUD 가져오기
	class UABHUDWidget* GetHUDWidget() const;

	// NPC가 죽었을 때
	void NPCKill(class AABCharacter* KilledNPC) const;

	// 게임 점수 증가
	void AddGameScore() const;

	// 입력 모드 변경
	void ChangeInputMode(bool bGameMode = true);

	// 결과창 UI 보여주기
	void ShowResultUI();

protected:

	// 
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// 위젯 클래스 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABHUDWidget> HUDWidgetClass;

	// 메뉴 위젯 클래스 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABGameplayWidget> MenuWidgetClass;

	// 결과 위젯 클래스 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABGameplayWidget> ResultWidgetClass;

private:
	// HUD
	UPROPERTY()
		class UABHUDWidget* HUDWidget;

	// 플레이어 스테이트 연결할 클래스
	UPROPERTY()
		class AABPlayerState* ABPlayerState;

	// 메뉴 위젯
	UPROPERTY()
		class UABGameplayWidget* MenuWidget;

	// 결과 위젯
	UPROPERTY()
		class UABGameplayResultWidget* ResultWidget;

	// 게임 입력 모드
	FInputModeGameOnly GameInputMode;

	// UI 입력 모드
	FInputModeUIOnly UIInputMode;

	// 게임 중지
	void OnGamePuase();
};
