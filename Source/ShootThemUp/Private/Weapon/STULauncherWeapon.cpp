// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

//функции стрельбы
void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}
void ASTULauncherWeapon::MakeShot()
{
    //локальная переменная начальной трансформации ракеты
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

    //спавним ракету с помощью функции отложенного BeginPlay и записываем в переменную
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);

    // set Projectile Params

    //вызываем BeginPlay
    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
