// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;
};


USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Tranform;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FGoKartMove LastMove;
};

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector GetAirResistance();
	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime);

	void UpdateLocationFromVelocity(float DeltaTime);

	// Mass of the car (kg).
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Force applied to the car when the throttle is fully down (N).
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// Minimum radius of the car turning circle at full lock (m).
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	// Higher means more drag (kg/m).
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	// Higher means more rolling resistance.
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015; // car tires on concrete.

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION (Server, Reliable, WithValidation) // RPC
	void Server_MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation) // RPC
	void Server_MoveRight(float Value);

	UPROPERTY(Replicated)
	FVector Velocity;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTranform)
	FTransform ReplicatedTranform;

	UFUNCTION()
	void OnRep_ReplicatedTranform();

	UPROPERTY(Replicated)
	float Throttle;
	UPROPERTY(Replicated)
	float SteeringThrow;
};
