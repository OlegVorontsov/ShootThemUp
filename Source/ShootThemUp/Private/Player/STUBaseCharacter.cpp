// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

//������� ����������� ��������� �����������
DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

//������� ��������� � ����������� ������������ �������� STUCharacterMovementComponent
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(
          ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    //������� ������ ������
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    //������� ������ ������ � ����
    SpringArm->SetupAttachment(GetRootComponent());
    //��������� �������� ��� ��������� ���������
    SpringArm->bUsePawnControlRotation = true;
    //���������� ������ ������� � ������� ����� ���� ������� ��������
    SpringArm->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    //������� ������
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    //������� ������ � �������
    CameraComponent->SetupAttachment(SpringArm);

    //������� ��������� �������� ���������
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("HealthComponent"));

    //������� ��������� ������������ ��������� ����� �������� ���������
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
    //������� ��������� � root
    HealthTextComponent->SetupAttachment(GetRootComponent());
    //������ �� ������� ��������� � ��������� ������� ���������
    HealthTextComponent->SetOwnerNoSee(true);

    //������� ��������� ��� ������
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>(TEXT("WeaponComponent"));
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    //������� �������� �������� �����������
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());

    //������������� �� ������� �� HealthComponent � �������� ������� OnDeath ���������
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);

    //������������� �� ������� �� HealthComponent � �������� ������� OnHealthChanged ���������
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    //������������� �� ������� �� ACharacter � �������� ������� ���������
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//������� ������ ���������
void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());
    //����������� �������� ������ ���������
    PlayAnimMontage(DeathAnimMontage);

    //������������� �������� ��������� ����� ��� ������
    GetCharacterMovement()->DisableMovement();

    //���������� ��������� ����� ������
    SetLifeSpan(LifeSpanOnDeath);

    //����������� pawn ��� ������ ���������
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    //��������� ������� �� �������� � CapsuleCollision �������� ���������
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

//������� ��������� �������� ���������
void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    //�������� � HealthTextComponent �������� �� ���������� ���������� � ��������� ���������
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

//������� ������� �� �����
void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    //��������� ���������� ���� ������� �������� ��� ���������� �������� ���������
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

    //���� �������� ������ ������ ������� �� ����������� ���� � ������� �� �������
    if (FallVelocityZ < LandedDamageVelocity.X)
        return;

    //����������� ���� �������� ���������
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);

    //������� ���� ���������
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

//����� ���������
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    //����� ������� �������� ��������� ������ ����� ������ �����
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

    //����� ������� �������� ������ ������ ���������
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);

    //����� ������� ������ ���������
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);

    //����� ������� ���� ���������
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);

    //����� ������� ��������
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
}

//������� �������� ��������� ������ �����
void ASTUBaseCharacter::MoveForward(float Amount)
{
    //��������� ���������� � true ������ ���� ������ ������ �������� ��������� ������
    IsMovingForward = Amount > 0.0f;

    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

//������� �������� ��������� ������ �����
void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorRightVector(), Amount);
}

//������� ���� ���������
void ASTUBaseCharacter::OnStartRunning()
{
    WantsToRun = true;
}
void ASTUBaseCharacter::OnStopRunning()
{
    WantsToRun = false;
}
bool ASTUBaseCharacter::IsRunning() const
{
    //��������� �������� ��� ������� ������� ������ true ����
    //������ ������ ���� � �������� ��������� ������ � �������� �������� �� 0
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

//������� ���������� ������ ����������� �������� ��������� ��� ����������� �������� �������� ��������
float ASTUBaseCharacter::GetMovementDirection() const
{
    //���� �������� ����� 0 ������� ������ 0
    if (GetVelocity().IsZero())
        return 0.0f;

    //��������� ������� ������� ��������
    const auto VelocityNormal = GetVelocity().GetSafeNormal();

    //��������� ���� ����� ��������� �������� � ����������� ��������
    //����� arc ������� ���������� ������������ �������� �������� � ����������� ��������
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));

    //��������� ��������� ������������ �������� �������� � ����������� ��������
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    //��������� �������� ���� �� ������ � ������� � ���������� � ����������
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

    //��������� �������� ��� ����������� � ���������� ���
    //��� ���� ���������� ��������� ������������ �� 0 � ����
    //�� - ���������� �������, ��� - �������� ���� �� ���� Z ���������� ������������
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
