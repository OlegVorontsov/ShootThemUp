// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    //создаем компонент визуального эффекта
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

//функции стрельбы
void ASTURifleWeapon::StartFire()
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"));

    //вызываем функция спавна эффекта
    InitMuzzleFX();

    //вызываем таймер по истечении которого опять вызовется MakeShot()
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);

    //вызов функции выстрела
    MakeShot();
}
void ASTURifleWeapon::StopFire()
{
    //останавливаем таймер
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

    //отключаем видимость эффекта
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
    //если нет игры и арсенал пуст в BaseWeapon не можем стрелять. останавливаем стрельбу
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    //создаем переменные для начальной/конечной точек выстрела
    FVector TraceStart, TraceEnd;

    //вызываем функцию получения начальной и конечной точки стрельбы
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    //переменная с результатами столкновения
    FHitResult HitResult;

    //вызываем функцию выстрела
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;
    //проверяем если пересечение произошло
    if (HitResult.bBlockingHit)
    {
        //переопределяем конечную точку трейса
        TraceFXEnd = HitResult.ImpactPoint;

        //вызов функции нанесения ущерба
        MakeDamage(HitResult);

        //рисуем линию выстрела от места расположения сокета в оружии до точки где произошло пересечение
        // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f,
        // 0, 3.0f);

        //нарисуем сферу в точке пересечения
        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        //выводим в консоль название кости с которой пересеклись
        // UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());

        //вызываем визуальный эффект
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    //вызываем функцию спавна трейса
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);

    //уменьшаем количество патрон в BaseWeapon
    DecreaseAmmo();
}

//функция получения начальной и конечной точки стрельбы
bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    //создаем переменные куда запишем параметры камеры персонажа
    FVector ViewLocation;
    FRotator ViewRotation;

    //получаем доступ к параметрам камеры персонажа
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    //переменная для точки начала выстрела
    TraceStart = ViewLocation;

    //создаем переменную для ширины конуса разброса
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);

    //переменная для направления выстрела. создаем с помощью рандомного направления в пределах конуса разброса
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);

    //переменная для конечной точки выстрела
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

//функция нанесения ущерба
void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
    //получаем актора в которого попали и записываем в переменную
    const auto DamagedActor = HitResult.GetActor();

    //наносим урон если у казатель не нулевой. FDamageEvent() - конструктор по умолчанию
    if (!DamagedActor)
        return;
    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

//функция спавна эффета
void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

//вкл видимости
void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!Visible);
        MuzzleFXComponent->SetVisibility(Visible, true);
    }
}

//функция спавна трейса пули
void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    //создаем переменную компонента
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
