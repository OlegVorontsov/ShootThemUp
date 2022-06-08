// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

//������� ��������
void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}
void ASTULauncherWeapon::MakeShot()
{
    //���� ��� ���� � ������� ���� � BaseWeapon �� ����� ��������
    if (!GetWorld() || IsAmmoEmpty())
        return;

    //������� ���������� ��� ���������/�������� ����� ��������
    FVector TraceStart, TraceEnd;

    //�������� ������� ��������� ��������� � �������� ����� ��������
    //�� STUBaseWeapon ��� ���������������
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    //���������� � ������������ ������������
    FHitResult HitResult;

    //�������� ������� ��������
    MakeHit(HitResult, TraceStart, TraceEnd);

    //������ �������� ����� ��������. ���� ������ �� � ����� �����������, ��� �� TraceEnd
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

    //������ ������ �������� ������ � ����������� ���
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    //��������� ���������� ��������� ������������� ������
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

    //������� ������ � ������� ������� ����������� BeginPlay � ���������� � ����������
    // auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass,
    // SpawnTransform);
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

    //���� ��������� �� ������� ��������� ����� ������
    if (Projectile)
    {
        //��������� ����������� �������� ������. �������� ������� � STUProjectile
        Projectile->SetShotDirection(Direction);

        //�������� ������ ���������
        Projectile->SetOwner(GetOwner());

        Projectile->FinishSpawning(SpawnTransform);
    }
    //��������� ���������� ������ � BaseWeapon
    DecreaseAmmo();
}
