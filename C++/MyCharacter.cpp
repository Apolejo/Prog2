
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyEnemy.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "Engine.h"


// Sets default values
AMyCharacter::AMyCharacter()
{

	// Variables Setup

	EnemiesKilled = 0;
	bAttackStarted = false;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed= 375.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;

	/*TurnRates*/
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	/*CameraSetup*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	//Create Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	/*ComboSetup*/

	bCanAttack = true;

	// load the attack montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> 
		MeleeFistAttackMontageObject(TEXT("AnimMontage'/Game/UE4_Mannequin/Animations/MeleeFistAttackMontage.MeleeFistAttackMontage'"));
	if (MeleeFistAttackMontageObject.Succeeded())
	{
		MeleeFistAttackMontage = MeleeFistAttackMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> 
		PlayerAttackMontageDataObject(TEXT("DataTable'/Game/UE4_Mannequin/DataTable/PlayerAttackMontageDataTable.PlayerAttackMontageDataTable'"));
	if (PlayerAttackMontageDataObject.Succeeded())
	{
		PlayerAttackDataTable = PlayerAttackMontageDataObject.Object;
	}

	LeftFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistCollisionBox"));
	LeftFistCollisionBox->SetupAttachment(RootComponent);
	LeftFistCollisionBox->SetHiddenInGame(false);

	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"));
	RightFistCollisionBox->SetupAttachment(RootComponent);
	RightFistCollisionBox->SetHiddenInGame(false);

}	


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()

{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	LeftFistCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "Fist_L_Collision");
	RightFistCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "Fist_R_Collision");
	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
	LeftFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);


	

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::AttackInput);
	PlayerInputComponent->BindAction("Attack", IE_Released, this,&AMyCharacter::AttackEnd);

}

void AMyCharacter::MoveForward(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f)) 
	{
		FRotator Rotation = Controller->GetControlRotation();
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())

		{
			Rotation.Pitch = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyCharacter::MoveRight(float AxisValue)
{
	if (Controller && AxisValue != 0.0f)
	{


		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyCharacter::LookUpAtRate(float AxisRate)
{
	AddControllerPitchInput(AxisRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}



void AMyCharacter::TurnAtRate(float AxisRate)
{
	AddControllerYawInput(AxisRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}
/*Delay*/
void AMyCharacter::ResetComboTimer()
{
	bCanAttack = true;
	GetWorld()->GetTimerManager().ClearTimer(ComboDelayTimerHandle);
}

void AMyCharacter::AttackInput()
{

	if (bCanAttack)
	{

		bCanAttack = false;
		bAttackStarted = true;
		if (PlayerAttackDataTable)
		{
			static const FString ContextString(TEXT("Player Attack Montage Context"));
			FPlayerAttackMontage* AttackMontage = PlayerAttackDataTable->FindRow<FPlayerAttackMontage>(FName(TEXT("Punch1")), ContextString, true);
			if (AttackMontage)
			{
				int MontageSectionIndex = rand() % AttackMontage->AnimSectionCount + 1;

				FString MontageSection = "Start_" + FString::FromInt(MontageSectionIndex);


				PlayAnimMontage(AttackMontage->Montage, 1.f, FName(*MontageSection));

			}

			

			
		}
		
	}
	
	
}

void AMyCharacter::AttackStart()
{
	LeftFistCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Enabled);
	LeftFistCollisionBox->SetNotifyRigidBodyCollision(true);

	RightFistCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Enabled);
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AMyCharacter::AttackEnd()
{
	GetWorld()->GetTimerManager().SetTimer(AttackDelayTimerHandle, this, &AMyCharacter::ResetComboTimer, .5f, false);


}

void AMyCharacter::OnAttackHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, TEXT("Golpe"));


}


void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (AMyEnemy* Enemy = Cast<AMyEnemy>(OtherActor)) // This will fail if OtherActor is null, or the cast fails. So we know we're not going to put any nullptrs in our array.
		{
			if (bAttackStarted==true)
			{


				GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, TEXT("Golpe"));

				Enemy->PlayerReference = this;
				Enemy->RecieveDamage(1);

				bAttackStarted = false;

			}
		}
	}


}




void AMyCharacter::UpdateScore(int Score)
{

	EnemiesKilled = Score + EnemiesKilled;
	UE_LOG(LogTemp, Warning, TEXT("%d"), EnemiesKilled);
	if (EnemiesKilled >= 8)
	{
		FVector2D TextSize = FVector2D(10.f, 10.f);

		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Black, TEXT("Winner"), true, TextSize);
	}
}