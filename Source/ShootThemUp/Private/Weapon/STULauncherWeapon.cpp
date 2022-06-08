// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

//функции стрельбы
void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}
void ASTULauncherWeapon::MakeShot()
{
    //если нет игры и арсенал пуст в BaseWeapon не можем стрелять
    if (!GetWorld() || IsAmmoEmpty())
        return;

    //создаем переменные для начальной/конечной точек выстрела
    FVector TraceStart, TraceEnd;

    //вызываем функцию получения начальной и конечной точки стрельбы
    //из STUBaseWeapon без переопределения
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    //переменная с результатами столкновения
    FHitResult HitResult;

    //вызываем функцию выстрела
    MakeHit(HitResult, TraceStart, TraceEnd);

    //задаем конечную точку стрельбы. если попали то в точке пересечения, нет то TraceEnd
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

    //задаем вектор движения ракеты и нормализуем его
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    //локальная переменная начальной трансформации ракеты
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

    //спавним ракету с помощью функции отложенного BeginPlay и записываем в переменную
    // auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass,
    // SpawnTransform);
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

    //если указатель не нулевой завершаем спавн ракеты
    if (Projectile)
    {
        //назначаем направление движения ракеты. вызываем функцию в STUProjectile
        Projectile->SetShotDirection(Direction);

        //получаем нашего персонажа
        Projectile->SetOwner(GetOwner());

        Projectile->FinishSpawning(SpawnTransform);
    }
    //уменьшаем количество патрон в BaseWeapon
    DecreaseAmmo();
}
