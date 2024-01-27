// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// 캐릭터 상태 열거형
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};


// 해당 로그 매크로를 선언
DECLARE_LOG_CATEGORY_EXTERN(CPP419, Log, All);


// -------------------------------------------------
// 로그 출력 방식 추가

// 출력 형식 지정 : 현재 매크로를 호출한 함수 이름과 위치(라인)을 출력
// __FUNCTION__ : 현재 호출한 함수 이름을 반환하는 매크로
// __LINE__ : 현재 호출한 줄을 반환하는 매크로
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// 코드가 들어있는 파일이름과 함수, 그리고 라인 정보를 추가해 CPP419 카테고리로 로그를 남긴다. 로그를 사용한 함수의 실행 시점을 파악할 때, 유용함.
// Verbosity : 로그 메시지의 상세도를 나타내는 매개 변수 ( 다양한 로그 레벨 표현 가능 - Warning, Error, Verbose 등 )
// TEXT("%s") -> *ABLOG_CALLINFO의 FString 부분을 호출
#define ABLOG_S(Verbosity) UE_LOG(CPP419, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)

// ABLOG_S 정보에 형식 문자열로 추가 정보를 지정해 정보를 남긴다.
// Format : 서식 문자열 -> 로그 메시지의 기본 형식을 정의
// TEXT("%s %s") -> 첫 %s에 현재 호출한 함수의 이름과 라인을 출력
// 두 번째 %s에 서식 문자열에 대한 가변 인수 매개변수를 입력
// ## : 해당 연산자는 이전 매개변수와 연결되는 것을 방지. 따라서, 매크로에 아무런 인수가 전달되지 않을 때 빈칸으로 처리.
// ##__VA_ARGS_ : 가변 인수 목록의 모든 인수를 다른 매크로나 함수에 전달할 수 있음.
#define ABLOG(Verbosity, Format, ...) UE_LOG(CPP419, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// 언리얼 엔진에서 제공하는 check나 ensure 어설션 대신 사용
// ABCHECK라는 매크로를 추가해 런타임에서 문제가 발생했을 때, 붉은색으로 에러 로그를 뿌리고 바로 함수를 반환하는 기능.
// ABCHECK 매크로 안의 조건이 참이 아닌 경우, 로그가 붉게 출력됨.

// Expr : 조건으로 사용하고 있으며, 해당 조건이 거짓일 때, 뒤의 { ABLOG } 매크로를 실행.
// #Expr -> 조건식을 문자열로 변환
// %s에 #Expr의 문자열 내용이 입력됨.
// __VA_ARGS__ : ABCHECK(Expr, ...) 부분에서 ... 부분으로 입력된 가변 매개변수에 해당.
// return __VA_ARGS__ : 즉, 매크로가 런타임에서 반환해야 하는 값을 나타낸다.

// 조건이 거짓이면, 지정된 반환값이나 코드 조각을 실행하고 함수를 즉시 반환.
#define ABCHECK(Expr, ...) {if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s "), TEXT("'"#Expr"'")); return __VA_ARGS__; }}