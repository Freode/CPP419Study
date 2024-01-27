// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// ĳ���� ���� ������
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};


// �ش� �α� ��ũ�θ� ����
DECLARE_LOG_CATEGORY_EXTERN(CPP419, Log, All);


// -------------------------------------------------
// �α� ��� ��� �߰�

// ��� ���� ���� : ���� ��ũ�θ� ȣ���� �Լ� �̸��� ��ġ(����)�� ���
// __FUNCTION__ : ���� ȣ���� �Լ� �̸��� ��ȯ�ϴ� ��ũ��
// __LINE__ : ���� ȣ���� ���� ��ȯ�ϴ� ��ũ��
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// �ڵ尡 ����ִ� �����̸��� �Լ�, �׸��� ���� ������ �߰��� CPP419 ī�װ��� �α׸� �����. �α׸� ����� �Լ��� ���� ������ �ľ��� ��, ������.
// Verbosity : �α� �޽����� �󼼵��� ��Ÿ���� �Ű� ���� ( �پ��� �α� ���� ǥ�� ���� - Warning, Error, Verbose �� )
// TEXT("%s") -> *ABLOG_CALLINFO�� FString �κ��� ȣ��
#define ABLOG_S(Verbosity) UE_LOG(CPP419, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)

// ABLOG_S ������ ���� ���ڿ��� �߰� ������ ������ ������ �����.
// Format : ���� ���ڿ� -> �α� �޽����� �⺻ ������ ����
// TEXT("%s %s") -> ù %s�� ���� ȣ���� �Լ��� �̸��� ������ ���
// �� ��° %s�� ���� ���ڿ��� ���� ���� �μ� �Ű������� �Է�
// ## : �ش� �����ڴ� ���� �Ű������� ����Ǵ� ���� ����. ����, ��ũ�ο� �ƹ��� �μ��� ���޵��� ���� �� ��ĭ���� ó��.
// ##__VA_ARGS_ : ���� �μ� ����� ��� �μ��� �ٸ� ��ũ�γ� �Լ��� ������ �� ����.
#define ABLOG(Verbosity, Format, ...) UE_LOG(CPP419, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// �𸮾� �������� �����ϴ� check�� ensure ��� ��� ���
// ABCHECK��� ��ũ�θ� �߰��� ��Ÿ�ӿ��� ������ �߻����� ��, ���������� ���� �α׸� �Ѹ��� �ٷ� �Լ��� ��ȯ�ϴ� ���.
// ABCHECK ��ũ�� ���� ������ ���� �ƴ� ���, �αװ� �Ӱ� ��µ�.

// Expr : �������� ����ϰ� ������, �ش� ������ ������ ��, ���� { ABLOG } ��ũ�θ� ����.
// #Expr -> ���ǽ��� ���ڿ��� ��ȯ
// %s�� #Expr�� ���ڿ� ������ �Էµ�.
// __VA_ARGS__ : ABCHECK(Expr, ...) �κп��� ... �κ����� �Էµ� ���� �Ű������� �ش�.
// return __VA_ARGS__ : ��, ��ũ�ΰ� ��Ÿ�ӿ��� ��ȯ�ؾ� �ϴ� ���� ��Ÿ����.

// ������ �����̸�, ������ ��ȯ���̳� �ڵ� ������ �����ϰ� �Լ��� ��� ��ȯ.
#define ABCHECK(Expr, ...) {if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s "), TEXT("'"#Expr"'")); return __VA_ARGS__; }}