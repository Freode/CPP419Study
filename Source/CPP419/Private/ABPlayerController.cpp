// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"
#include "ABCharacter.h"
#include "ABGameplayWidget.h"
#include "ABGameplayResultWidget.h"
#include "ABGameStateBase.h"

// 생성자
AABPlayerController::AABPlayerController()
{
	// HUD UI 클래스 설정
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	// 메뉴 UI 클래스 설정
	static ConstructorHelpers::FClassFinder<UABGameplayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	// 결과 UI 클래스 설정
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

	// 게임에게만 입력이 되도록 설정
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// HUD 설정
	ABLOG(Warning, TEXT("Player Controller Beginplay"));
	
	// 입력 모드 변경
	ChangeInputMode(true);

	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	ABCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	// 결과 위젯 설정
	ResultWidget = CreateWidget<UABGameplayResultWidget>(this, ResultWidgetClass);
	ABCHECK(nullptr != ResultWidget);

	// HUD 위젯과 플레이어 스테이트 연결
	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	// 연결했으므로 정보 업데이트
	ABPlayerState->OnPlayerStateChanged.Broadcast();
}

// HUD 위젯 가져오기
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

// NPC가 죽었을 때
void AABPlayerController::NPCKill(AABCharacter* KilledNPC) const
{
	// 현재 플레이어에게 해당 NPC가 제공하는 경험치량을 추가
	ABPlayerState->AddExp(KilledNPC->GetExp());
}

// 게임 점수 증가
void AABPlayerController::AddGameScore() const
{
	ABPlayerState->AddGameScore();
}

// 게임 중지
void AABPlayerController::OnGamePuase()
{
	// 메뉴 위젯 생성
	MenuWidget = CreateWidget<UABGameplayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	// 게임 중지 및 UI 입력 모드로 변경
	SetPause(true);
	ChangeInputMode(false);
}

// 입력 모드 변경
void AABPlayerController::ChangeInputMode(bool bGameMode)
{
	// 게임 입력 모드
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	// UI 입력 모드
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

// 결과 UI 출력
void AABPlayerController::ShowResultUI()
{
	// Game State를 가져와서 결과창에 바인딩
	auto ABGameState = Cast<AABGameStateBase>(UGameplayStatics::GetGameState(this));
	ABCHECK(nullptr != ABGameState);
	ResultWidget->BindGameState(ABGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}