// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "MyEnemy.generated.h"

UCLASS()
class ON3DFILTERTEST_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AMyEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemyBasics, meta = (AllowPrivateAccess = "true"))
	int HP;


	void RecieveDamage(int Damage);

	
	void CheckIfImDeath(int Life);


	void DeathEvent();

	void ResetHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FVector AxisVector;

	UPROPERTY(VisibleAnywhere)
	bool CanBeHit;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInstance*> MainMaterial;

//	UPROPERTY(EditAnywhere)
//	UMaterialInstanceDynamic* MaterialDynamic;

//	UPROPERTY(EditAnywhere)
//	UMaterial* MaterialDynamic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* EnemyAnimation;
	/*Damage System*/

	AMyCharacter* PlayerReference;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
