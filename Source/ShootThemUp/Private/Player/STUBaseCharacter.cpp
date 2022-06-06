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

//создаем собственную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

//создаем персонажа с собственным конмпонентом движения STUCharacterMovementComponent
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(
          ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    //создаем штатив камеры
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    //аттачим штатив камеры к роот
    SpringArm->SetupAttachment(GetRootComponent());
    //разрешаем контроль над вращением персонажа
    SpringArm->bUsePawnControlRotation = true;
    //отодвигаем камеру немного в сторону чтобы было удобнее целиться
    SpringArm->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    //создаем камеру
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    //аттачим камеру к штативу
    CameraComponent->SetupAttachment(SpringArm);

    //создаем компонент здоровья персонажа
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("HealthComponent"));

    //создаем компонент показывающий плавающий текст здоровья персонажа
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
    //аттачим компонент к root
    HealthTextComponent->SetupAttachment(GetRootComponent());
    //делаем не видимым компонент у персонажа которым управляем
    HealthTextComponent->SetOwnerNoSee(true);

    //создаем компонент для оружия
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>(TEXT("WeaponComponent"));
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    //макросы проверки создания компонентов
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());

    //подписываемся на делегат из HealthComponent и вызываем функцию OnDeath персонажа
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);

    //подписываемся на делегат из HealthComponent и вызываем функцию OnHealthChanged персонажа
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    //подписываемся на делегат из ACharacter и вызываем функцию персонажа
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//функция смерти персонажа
void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());
    //проигрываем анимацию смерти персонажа
    PlayAnimMontage(DeathAnimMontage);

    //останавливаем движение персонажа после его смерти
    GetCharacterMovement()->DisableMovement();

    //уничтожаем персонажа после смерти
    SetLifeSpan(LifeSpanOnDeath);

    //переключаем pawn при смерти персонажа
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    //отключаем реакцию на коллизии у CapsuleCollision умершего персонажа
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

//функция изменения здоровья персонажа
void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    //передаем в HealthTextComponent значение из локальтной переменной с указанной точностью
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

//функция падения на землю
void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    //объявляем переменную куда заносим значение зэд переменной скорости персонажа
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

    //если скорость меньше нижней границы не высчитываем урон и выходим из функции
    if (FallVelocityZ < LandedDamageVelocity.X)
        return;

    //высчитываем урон здоровью персонажа
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);

    //наносим урон персонажу
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

//инпут персонажа
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    //вызов функций движения персонажа вперед назад вправо влево
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

    //вызов функций движения камеры вокруг персонажа
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);

    //вызов функции прыжка персонажа
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);

    //вызов функций бега персонажа
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);

    //вызов функций стрельбы
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
}

//функция движения персонажа вперед назад
void ASTUBaseCharacter::MoveForward(float Amount)
{
    //переводим переменную в true только если нажата кнопка движения персонажа вперед
    IsMovingForward = Amount > 0.0f;

    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

//функция движения персонажа вправо влево
void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorRightVector(), Amount);
}

//функции бега персонажа
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
    //вычисляем значение при котором функция вернет true если
    //нажата кнопка бега и персонаж двигается вперед и скорость движения не 0
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

//функция возвращает вектор направления движения персонажа для определения анимации бокового движения
float ASTUBaseCharacter::GetMovementDirection() const
{
    //если скорость равна 0 функция вернет 0
    if (GetVelocity().IsZero())
        return 0.0f;

    //вычисляем нормаль вектора скорости
    const auto VelocityNormal = GetVelocity().GetSafeNormal();

    //вычисляем угол между векторами скорости и направления движения
    //равна arc косинус скалярного произведения векторов скорости и направления движения
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));

    //вычисляем векторное произведение векторов скорости и направления движения
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    //переводим значение угла из радиан в градусы и записываем в переменную
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

    //вычисляем значение для направления и возвращаем его
    //при этом проеверяем векторное произведение на 0 и если
    //да - возвращаем градусы, нет - умножаем угол на знак Z векторного произведения
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
