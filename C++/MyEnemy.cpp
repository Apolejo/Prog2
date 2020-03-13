// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"



// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 4;
	CanBeHit = true;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		EnemyAnimationObject(TEXT("AnimMontage'/Game/UE4_Mannequin/Animations/EnemiesAnimationMontage.EnemiesAnimationMontage'"));
	if (EnemyAnimationObject.Succeeded())
	{
		EnemyAnimation = EnemyAnimationObject.Object;
	}

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	int32 countMaterial = MainMaterial.Num();
	if (countMaterial == 0)
		return;

	int32 randomNumber = FMath::RandRange(0, countMaterial - 1);

	UMaterialInstance* Mat = MainMaterial[randomNumber];
	GetMesh()->SetMaterial(0, Mat);

}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMyEnemy::ResetHit()
{
	CanBeHit = true;
}

void AMyEnemy::DeathEvent()
{
//	FVector Location = FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 100);
//	FRotator Rotation = GetActorRotation();
	
//	GetWorld()->SpawnActor(ActorToSpawn, &Location, &Rotation);
	PlayerReference->UpdateScore(1);
	this->Destroy();


}

void AMyEnemy::CheckIfImDeath(int Life)
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), Life);

	if (Life <= 0)
	{
		PlayAnimMontage(EnemyAnimation, 1.0f, FName("Fist_3"));
		FTimerHandle Dying;
		GetWorldTimerManager().SetTimer(Dying, this, &AMyEnemy::DeathEvent, 2.0f);
	}
	else
	{
		PlayAnimMontage(EnemyAnimation, 1.0f, FName("Fist_Right"));
	}

}


void AMyEnemy::RecieveDamage(int Damage)
{

	if (CanBeHit)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerReference->GetActorLocation()), ETeleportType::None);
		// 		AxisVector = FVector(PlayerReference->GetActorRotation().Pitch, PlayerReference->GetActorRotation().Yaw, PlayerReference->GetActorRotation().Roll);
		// 		FRotator NewRotation = FRotator(0, AxisVector.Y , 0);
		// 		FQuat QuatRotation = FQuat(NewRotation);
		// 		AddActorLocalRotation(QuatRotation);
		HP = HP - Damage;
		CheckIfImDeath(HP);
		CanBeHit = false;
	}
	FTimerHandle ResetTimeByHit;
	GetWorldTimerManager().SetTimer(ResetTimeByHit, this, &AMyEnemy::ResetHit, .25f);

}

