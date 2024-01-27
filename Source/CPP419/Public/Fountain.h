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

	// CPP419.h로 EngineMinimal.h를 포함.
	
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

	// VisibleAnywhere : 객체를 볼 수 있지만, 다른 객체로 변경은 불가능함.
	// 즉, StaticMeshComponent에서 다른 컴포넌트로 변경 불가능.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Water;

	UPROPERTY(VisibleAnywhere)
		UPointLightComponent *Light;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent *Splash;
	
	// UPROPERTY() 매크로 선언만으로도 미리 예약된 기본값이 지정됨.
	// 정수 유형의 멤버 변수에 UPROPERTY 매크로 선언하면, 반드시 0의 초기 값을 보장받음.

	// EditAnywhere : 언리얼 에디터에서 속성의 데이터 변경
	UPROPERTY(EditAnywhere, Category = ID)
		int32 ID;

	// 지정한 속도로 회전시켜주는 컴포넌트
	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* Movement;

private:
	// Meta = AllowPrivateAccess : 은닉과 동시에 외부에서 편집할 수 있도록 지원
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float RotateSpeed;
};
