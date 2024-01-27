// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPP419.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class CPP419_API AFountain : public AActor
{
	GENERATED_BODY()

	// CPP419.h�� EngineMinimal.h�� ����.
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// VisibleAnywhere : ��ü�� �� �� ������, �ٸ� ��ü�� ������ �Ұ�����.
	// ��, StaticMeshComponent���� �ٸ� ������Ʈ�� ���� �Ұ���.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Water;

	UPROPERTY(VisibleAnywhere)
		UPointLightComponent *Light;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent *Splash;
	
	// UPROPERTY() ��ũ�� �������ε� �̸� ����� �⺻���� ������.
	// ���� ������ ��� ������ UPROPERTY ��ũ�� �����ϸ�, �ݵ�� 0�� �ʱ� ���� �������.

	// EditAnywhere : �𸮾� �����Ϳ��� �Ӽ��� ������ ����
	UPROPERTY(EditAnywhere, Category = ID)
		int32 ID;

	// ������ �ӵ��� ȸ�������ִ� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* Movement;

private:
	// Meta = AllowPrivateAccess : ���а� ���ÿ� �ܺο��� ������ �� �ֵ��� ����
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float RotateSpeed;
};
