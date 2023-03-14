// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class TESTPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default Values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Add these 2 lines below
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* PersonMesh;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UPhysicsThrusterComponent* PhysicsThruster;

	UFUNCTION()
		void MouseRight(float Value);

	UFUNCTION()
		void MouseUp(float Value);

	UFUNCTION()
		void MoveUp(float Value);

	UFUNCTION()
		void RotateRight(float Value);

	UFUNCTION()
		void TiltForward(float Value);

	UFUNCTION()
		void TiltRight(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
		float VariableUpForce = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
		float ConstantUpForce = 979.5;
		//위쪽 중력값 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
		float YawRotationSpeed = 60;

	UFUNCTION(BlueprintSetter)
		void SetDesiredTiltAngle(float Value);

	UFUNCTION(BlueprintGetter)
		float GetDesiredTiltAngle();

	UFUNCTION(BlueprintSetter)
		void SetTiltSpeedClampBound(float Value);

	UFUNCTION(BlueprintGetter)
		float GetTiltSpeedClampBound();

	UFUNCTION(BlueprintSetter)
		void SetTiltingSpeed(float Value);

	UFUNCTION(BlueprintGetter)
		float GetTiltingSpeed();

private:
	UPROPERTY(EditAnywhere, BlueprintSetter = SetDesiredTiltAngle, BlueprintGetter = GetDesiredTiltAngle,
		Category = "Drone Movement", meta = (ClampMin = -60, ClampMax = 60))
		float DesiredTiltAngle = 30;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltSpeedClampBound, BlueprintGetter = GetTiltSpeedClampBound,
		Category = "Drone Movement", meta = (ClampMin = 0))
		float TiltSpeedClampBound = 20;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltingSpeed, BlueprintGetter = GetTiltingSpeed,
		Category = "Drone Movement", meta = (ClampMin = 0))
		float TiltingSpeed = 5;



};
