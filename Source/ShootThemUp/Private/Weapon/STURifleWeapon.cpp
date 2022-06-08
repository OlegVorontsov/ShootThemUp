// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

//функции стрельбы
void ASTURifleWeapon::StartFire()
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"));

    //вызываем таймер по истечении которого опять вызовется MakeShot()
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);

    //вызов функции выстрела
    MakeShot();
}
void ASTURifleWeapon::StopFire()
{
    //останавливаем таймер
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
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

    //проверяем если пересечение произошло
    if (HitResult.bBlockingHit)
    {
        //вызов функции нанесения ущерба
        MakeDamage(HitResult);

        //рисуем линию выстрела от места расположения сокета в оружии до точки где произошло пересечение
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);

        //нарисуем сферу в точке пересечения
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        //выводим в консоль название кости с которой пересеклись
        // UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
    }
    else
    {
        //рисуем линию выстрела от места расположения сокета в оружии на расстояние TraceMaxDistance
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
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
