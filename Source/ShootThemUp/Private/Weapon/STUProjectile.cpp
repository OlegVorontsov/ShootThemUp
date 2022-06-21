// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(5.0f);

    //включаем коллизии
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    //указываем что хотиv получать материал куда попали
    SphereComponent->bReturnMaterialOnMove = true;

    SetRootComponent(SphereComponent);

    //создаем компонет движения снаряда
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    //задаем начальную скорость
    MovementComponent->InitialSpeed = 2000.0f;
    //отключаем притяжение к земле чтобы ракета летела по прямой
    MovementComponent->ProjectileGravityScale = 0.0f;

    //создаем компонент визуального эффекта
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
    check(MovementComponent);
    check(SphereComponent);
    check(WeaponFXComponent);

    //задаем скорость движения умножением направления движения на начальную скорость
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    //указываем игнорирование нашего персонажа во время движения ракеты
    SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);

    //вызываем функцию OnProjectileHit через делегат OnComponentHit
    SphereComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    //ограничиваем время жизни ракеты
    SetLifeSpan(LifeSeconds);
}

//функция столкновения с ракетой
void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld())
        return;

    //после столкновения останавливаем движения ракеты
    MovementComponent->StopMovementImmediately();

    //наносим ущерб всему что в сфере через получение всех акторов со сцены
    //с использованием функции TakeDamage всех акторов
    UGameplayStatics::ApplyRadialDamage(GetWorld(), //
        DamageAmount,                               //
        GetActorLocation(),                         //
        DamageRadius,                               //
        UDamageType::StaticClass(),                 //
        {GetOwner()},                               //массив акторов игнорирующихся при нанесении ущерба
        this,                                       //указатель на то кто нанес ущерб
        GetController(),                            //указатель на контроллер
        DoFullDamage);

    //каждый тик рисуем сферу
    // DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

    //вызываем визуальный эффект
    WeaponFXComponent->PlayImpactFX(Hit);

    //уничтожаем ракету
    Destroy();
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
