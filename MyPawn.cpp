// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"

// Sets default Values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Add these 2 lines below
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(Mesh);

	PersonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PersonMesh Static Mesh"));
	SetRootComponent(PersonMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->AttachToComponent(PersonMesh, FAttachmentTransformRules::KeepWorldTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepWorldTransform);

	PhysicsThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Physics Thruster"));
	PhysicsThruster->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);

	//Not setting mesh asset in here, 
	//because don't want to deal with having mesh asset always in a predetermined location
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(1);
	Mesh->SetAngularDamping(1);

	//Not setting thruster location, because we don't know what mesh will be set
	PhysicsThruster->SetRelativeRotation(FRotator(-90, 0, 0));
	PhysicsThruster->bAutoActivate = true;

	SpringArm->TargetArmLength = 800;
	SpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	//Put in here just for testing to automatically possess pawn
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FVector ThrusterCenterOfMassDifference = PhysicsThruster->GetComponentLocation() - Mesh->GetCenterOfMass();
	FRotator InvertedMeshRotation = Mesh->GetComponentRotation().GetInverse();
	FVector CenterOfMassActualOffset = InvertedMeshRotation.RotateVector(ThrusterCenterOfMassDifference);
	FVector CenterOfMassWantedOffset = FVector(CenterOfMassActualOffset.X, CenterOfMassActualOffset.Y, 0);
	Mesh->SetCenterOfMass(CenterOfMassWantedOffset, FName());
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Mouse Right", this, &AMyPawn::MouseRight);
	PlayerInputComponent->BindAxis("Mouse Up", this, &AMyPawn::MouseUp);
	PlayerInputComponent->BindAxis("Move Up", this, &AMyPawn::MoveUp);
	PlayerInputComponent->BindAxis("Rotate Right", this, &AMyPawn::RotateRight);
	PlayerInputComponent->BindAxis("Tilt Forward", this, &AMyPawn::TiltForward);
	PlayerInputComponent->BindAxis("Tilt Right", this, &AMyPawn::TiltRight);

}

void AMyPawn::MouseRight(float Value)
{
	SpringArm->AddRelativeRotation(FRotator(0, Value, 0));
}

void AMyPawn::MouseUp(float Value)
{
	SpringArm->AddRelativeRotation(FRotator(Value, 0, 0));
}

void AMyPawn::MoveUp(float Value)
{
	float DesiredUpForce = Value * VariableUpForce + ConstantUpForce;
	PhysicsThruster->ThrustStrength = DesiredUpForce / GetActorUpVector().Z * Mesh->GetMass();
}

void AMyPawn::RotateRight(float Value)
{
	Mesh->AddTorqueInDegrees(GetActorUpVector() * Value * YawRotationSpeed, FName(), true);
}

void AMyPawn::TiltForward(float Value)
{
	float DesiredAngle = Value * DesiredTiltAngle + Mesh->GetComponentRotation().Pitch;
	float ClampedValue = FMath::Clamp(DesiredAngle, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorRightVector() * ClampedValue * TiltingSpeed, FName(), true);
}

void AMyPawn::TiltRight(float Value)
{
	float DesiredAngle = Value * -DesiredTiltAngle + Mesh->GetComponentRotation().Roll;
	float ClampedValue = FMath::Clamp(DesiredAngle, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorForwardVector() * ClampedValue * TiltingSpeed, FName(), true);
}

void AMyPawn::SetDesiredTiltAngle(float Value)
{
	DesiredTiltAngle = FMath::Clamp(Value, -60.f, 60.f);
}

float AMyPawn::GetDesiredTiltAngle()
{
	return DesiredTiltAngle;
}

void AMyPawn::SetTiltSpeedClampBound(float Value)
{
	TiltSpeedClampBound = Value > 0 ? Value : 0;
}

float AMyPawn::GetTiltSpeedClampBound()
{
	return TiltSpeedClampBound;
}

void AMyPawn::SetTiltingSpeed(float Value)
{
	TiltingSpeed = Value > 0 ? Value : 0;
}

float AMyPawn::GetTiltingSpeed()
{
	return TiltingSpeed;
}

