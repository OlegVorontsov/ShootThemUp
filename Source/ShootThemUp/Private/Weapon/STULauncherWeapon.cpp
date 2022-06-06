// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

//������� ��������
void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}
void ASTULauncherWeapon::MakeShot()
{
    //��������� ���������� ��������� ������������� ������
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

    //������� ������ � ������� ������� ����������� BeginPlay � ���������� � ����������
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);

    // set Projectile Params

    //�������� BeginPlay
    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
