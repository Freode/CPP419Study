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
	// ������
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void Possess(APawn* aPawn) override;

	// HUD ��������
	class UABHUDWidget* GetHUDWidget() const;

	// NPC�� �׾��� ��
	void NPCKill(class AABCharacter* KilledNPC) const;

	// ���� ���� ����
	void AddGameScore() const;

	// �Է� ��� ����
	void ChangeInputMode(bool bGameMode = true);

	// ���â UI �����ֱ�
	void ShowResultUI();

protected:

	// 
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABHUDWidget> HUDWidgetClass;

	// �޴� ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABGameplayWidget> MenuWidgetClass;

	// ��� ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABGameplayWidget> ResultWidgetClass;

private:
	// HUD
	UPROPERTY()
		class UABHUDWidget* HUDWidget;

	// �÷��̾� ������Ʈ ������ Ŭ����
	UPROPERTY()
		class AABPlayerState* ABPlayerState;

	// �޴� ����
	UPROPERTY()
		class UABGameplayWidget* MenuWidget;

	// ��� ����
	UPROPERTY()
		class UABGameplayResultWidget* ResultWidget;

	// ���� �Է� ���
	FInputModeGameOnly GameInputMode;

	// UI �Է� ���
	FInputModeUIOnly UIInputMode;

	// ���� ����
	void OnGamePuase();
};
