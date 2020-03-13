// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"

#include "Engine/DataTable.h"

#include "MyCharacter.generated.h"




USTRUCT(BlueprintType)
struct FPlayerAttackMontage : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
			UAnimMontage* Montage;

	/** amount of start sections within our montage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 AnimSectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Description;

		

};



USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Disabled;

	// default constructor
	FMeleeCollisionProfile()
	{
		Enabled = FName(TEXT("Weapon"));
		Disabled = FName(TEXT("NoCollision"));
	}
};









UCLASS()
class ON3DFILTERTEST_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
		int EnemiesKilled;

	UPROPERTY(Transient)
		bool bIsJumping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UDataTable* PlayerAttackDataTable;

	/*Components*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Assets, meta = (AllowPrivateAccess = "true"))
		class USkeletalMesh* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Assets, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* LeftFistCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* RightFistCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* MeleeFistAttackMontage;

public:
	// Sets default values for this character's properties
	AMyCharacter();

	bool bAttackStarted;
	bool bCanAttack;


protected:
	

//	TArray<AMyEnemy*> arrayOfMyEnemy;
	/*Delay till combo reset*/
	FTimerHandle ComboDelayTimerHandle;
	FTimerHandle AttackDelayTimerHandle;

	/*Resets timer*/

	void ResetAttackTimer();
	void ResetComboTimer();
	/*Movement*/
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void TurnAtRate(float AxisRate);

	void LookUpAtRate(float AxisRate);

	/*Combo Setup*/







	
protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AttackInput();

	void AttackStart();

	void AttackEnd();
	
	/*Enemy score*/
	void UpdateScore(int Score);

	

	UFUNCTION()
	void OnAttackHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

private:

	FMeleeCollisionProfile MeleeCollisionProfile;





};
