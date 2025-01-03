﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#include "ABCharacterUserWidget.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "ABSaveGame.h"
#include "ABPlayerState.h"

// 다음 캐릭터로 설정
void UABCharacterUserWidget::NextCharacter(bool bForward)
{
	// 방향에 따라 다음 인덱스 번호 설정
	bForward ? CurrentIndex++ : CurrentIndex--;

	// 맨 마지막으로 이동
	if (CurrentIndex == -1)
	{
		CurrentIndex = MaxIndex - 1;
	}
	// 맨 청므으로 이동
	if (CurrentIndex == MaxIndex)
	{
		CurrentIndex = 0;
	}

	// 캐릭터 세팅 객체 가져오기
	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	// 애셋 레퍼런스 가져오기
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	// 게임 인스턴스 객체 가져오기
	auto ABGameInstance = GetWorld()->GetGameInstance<UABGameInstance>();
	ABCHECK(nullptr != ABGameInstance);
	ABCHECK(TargetComponent.IsValid());

	// 스켈레탈 메시 애셋 가져와서 설정
	USkeletalMesh* Asset = ABGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

//
void UABCharacterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기화
	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	// 현재 월드에 있는 특정 타입을 상속받은 액터의 목록을 Iterator로 가져오기
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	// 이전 버튼 연결
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	// 다음 버튼 연결
	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	// 플레이어 이름 입력칸 연결
	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	// 확인 버튼 연결
	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	// 버튼을 각 함수에 바인딩
	PrevButton->OnClicked.AddDynamic(this, &UABCharacterUserWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UABCharacterUserWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UABCharacterUserWidget::OnConfirmClicked);
}

// 이전 버튼 클릭
void UABCharacterUserWidget::OnPrevClicked()
{
	NextCharacter(false);
}

// 다음 버튼 클릭
void UABCharacterUserWidget::OnNextClicked()
{
	NextCharacter(true);
}

// 확인 버튼 클릭
void UABCharacterUserWidget::OnConfirmClicked()
{
	// 플레이어 이름 가져오기
	FString CharacterName = TextBox->GetText().ToString();
	// 플레이어 이름 길이 검사
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10)
	{
		return;
	}

	// 새로운 플레이어 데이터 초기화
	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	// 플레이어 스테이트 객체 생성
	auto ABPlayerState = GetDefault<AABPlayerState>();
	// 슬롯에 저장하고 Gameplay 레벨로 이동
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, ABPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Gameplay"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}