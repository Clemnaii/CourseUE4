// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("springArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("cameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//bOrientRotationToMovement = true: 只要角色有输入（比如 AddMovementInput），角色就会自动朝移动方向旋转。而不是保持原有的姿态进行平移
	bUseControllerRotationYaw = false;
	/**
	 * ✅ 你可以这么理解：
		属性名	含义
		bUseControllerRotationYaw = true	角色旋转跟着控制器转（常用于第一人称）
		bOrientRotationToMovement = true	角色自动面朝移动方向（适用于第三人称）
		而它们不能同时为 true， 否则控制权会冲突。
	 */

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::MoveForward(float value) {
	/**
	 * 要区分两个方向向量：	 
	 * 1.CameraComp摄像机的方向（也是控制器Controller的方向）
	 * 2.Pawn的方向（角色面朝的方向）
	 */

	//区分一下是往摄像机指向的方向前进，还是pawn人物面向的方向前进
	//正常来说应该要往摄像机指向的方向前进
	FRotator controlRot = GetControlRotation();
	/**
	* GetControlRotation() 返回的是 控制器（Controller）当前的朝向，也就是玩家输入设备（如鼠标、手柄）控制的方向。
		对于 PlayerController 来说，这个值等于摄像机的朝向（默认摄像机跟着 Controller ）。
		所以你取的 controlRot，本质上是当前摄像机的朝向。
	*/
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(controlRot.Vector(), value);
}

void AMyCharacter::MoveRight(float value) {
	/**
	 * 同理，当需要人物左右移动，很显然也需要让移动的向量变得摄像机向量旋转90°
	 * 而不是人物面朝向量的左右90°，否则人物旋转会让人物面朝的向量也实时旋转，那么就会导致人物一直在原地转圈
	 */

	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	//x = forward
	//y = right
	//z = up
	FVector RightVector = FRotationMatrix(controlRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	/**
	 * AActor
		└── APawn
			└── ACharacter
	 */
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

