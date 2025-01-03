﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABGameplayWidget.h"
#include "Components/Button.h"
#include "ABPlayerController.h"

// UI 위젯을 초기화하는 시점에서 발생
void UABGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 재개 버튼 연동
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UABGameplayWidget::OnResumeClicked);
	}

	// Title로 이동 버튼 연동
	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UABGameplayWidget::OnReturnToTitleClicked);
	}

	// 재시작 버튼 연동
	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UABGameplayWidget::OnRetryGameClicked);
	}
}

// 재개 버튼 클릭
void UABGameplayWidget::OnResumeClicked()
{
	// Player Controller 가져오기
	auto ABPlayerController = Cast<AABPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != ABPlayerController);

	// 뷰포트로부터 제거
	RemoveFromParent();

	ABPlayerController->ChangeInputMode(true);
	ABPlayerController->SetPause(false);
}

// Title로 이동 버튼 클릭
void UABGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

// 재시작 버튼 클릭
void UABGameplayWidget::OnRetryGameClicked()
{
	auto ABPlayerController = Cast<AABPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != ABPlayerController);
	ABPlayerController->RestartLevel();
}


