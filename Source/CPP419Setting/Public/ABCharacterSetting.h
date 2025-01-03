﻿// User : Jeonghoe22 : Study by '이득우의 언리얼 C++ 게임개발의 정석'

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=CPP419)
class CPP419SETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	// 생성자
	UABCharacterSetting();

	// Character Asset에 대한 Reference 경로를 INI 파일에서 불러옴.
	// FSoftObjectPath은 Asset에 대한 Reference 경로만 있으면, 해당 애셋을 참조할 수 있도록 지원
	UPROPERTY(config)
		TArray<FSoftObjectPath> CharacterAssets;
	
	
};
