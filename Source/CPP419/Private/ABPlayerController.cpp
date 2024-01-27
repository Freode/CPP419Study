// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"
#include "ABCharacter.h"
#include "ABGameplayWidget.h"
#include "ABGameplayResultWidget.h"
#include "ABGameStateBase.h"

// ������
AABPlayerController::AABPlayerController()
{
	// HUD UI Ŭ���� ����
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	// �޴� UI Ŭ���� ����
	static ConstructorHelpers::FClassFinder<UABGameplayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	// ��� UI Ŭ���� ����
	static ConstructorHelpers::FClassFinder<UABGameplayResultWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::Possess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::Possess(aPawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���ӿ��Ը� �Է��� �ǵ��� ����
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// HUD ����
	ABLOG(Warning, TEXT("Player Controller Beginplay"));
	
	// �Է� ��� ����
	ChangeInputMode(true);

	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	ABCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	// ��� ���� ����
	ResultWidget = CreateWidget<UABGameplayResultWidget>(this, ResultWidgetClass);
	ABCHECK(nullptr != ResultWidget);

	// HUD ������ �÷��̾� ������Ʈ ����
	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	// ���������Ƿ� ���� ������Ʈ
	ABPlayerState->OnPlayerStateChanged.Broadcast();
}

// HUD ���� ��������
UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

// 
void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AABPlayerController::OnGamePuase);
}

// NPC�� �׾��� ��
void AABPlayerController::NPCKill(AABCharacter* KilledNPC) const
{
	// ���� �÷��̾�� �ش� NPC�� �����ϴ� ����ġ���� �߰�
	ABPlayerState->AddExp(KilledNPC->GetExp());
}

// ���� ���� ����
void AABPlayerController::AddGameScore() const
{
	ABPlayerState->AddGameScore();
}

// ���� ����
void AABPlayerController::OnGamePuase()
{
	// �޴� ���� ����
	MenuWidget = CreateWidget<UABGameplayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	// ���� ���� �� UI �Է� ���� ����
	SetPause(true);
	ChangeInputMode(false);
}

// �Է� ��� ����
void AABPlayerController::ChangeInputMode(bool bGameMode)
{
	// ���� �Է� ���
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	// UI �Է� ���
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

// ��� UI ���
void AABPlayerController::ShowResultUI()
{
	// Game State�� �����ͼ� ���â�� ���ε�
	auto ABGameState = Cast<AABGameStateBase>(UGameplayStatics::GetGameState(this));
	ABCHECK(nullptr != ABGameState);
	ResultWidget->BindGameState(ABGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}