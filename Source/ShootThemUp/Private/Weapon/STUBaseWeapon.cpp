// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

//������� ����������� ��������� �����������
DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    //��������� ������� ���
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero!"));
    checkf(DefaultAmmo.CLips > 0, TEXT("CLips count couldn't be less or equal zero!"));

    //������ ������� ������� ����� ���������
    CurrentAmmo = DefaultAmmo;
}

//����������� ������� ��������
void ASTUBaseWeapon::StartFire() {}
void ASTUBaseWeapon::StopFire() {}
void ASTUBaseWeapon::MakeShot() {}

//������� ��������� ������ ���������
APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    //�������� ������ ��������� � ���������� � ����������
    const auto Player = Cast<ACharacter>(GetOwner());

    //���� ��������� �� ������� ���������� ���
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

//������� ��������� ������� � ������ ���������
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    //�������� ������ � ������ � ������� ������� ��������� ������ ���������
    const auto Controller = GetPlayerController();
    if (!Controller)
        return false;

    //�������� ������ � ������ ���������
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

//������� ��������� ������������ ������ � ������
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

//������� ��������� ��������� � �������� ����� ��������
bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    //������� ���������� ���� ������� ��������� ������ ���������
    FVector ViewLocation;
    FRotator ViewRotation;

    //�������� ������ � ���������� ������ ���������
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    //���������� ��� ����� ������ ��������
    TraceStart = ViewLocation;

    //���������� ��� ����������� ��������
    const FVector ShootDirection = ViewRotation.Vector();

    //���������� ��� �������� ����� ��������
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

//������� ��������
void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld())
        return;

    //������� ���������� ��� ������ ���������� �����������
    FCollisionQueryParams CollisionParams;

    //��������� ����� �������������� ����������� � ����� �����
    CollisionParams.AddIgnoredActor(GetOwner());

    //��������� ��� ���� �������� �������� ���� ������
    CollisionParams.bReturnPhysicalMaterial = true;

    //�������� ������� ����������� � ������ ���������� �������� �� �����
    GetWorld()->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

//������� ����������� ���������� ������ ��� ��������
void ASTUBaseWeapon::DecreaseAmmo()
{
    //��������� �������� �� �������
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty!"));
    }
    //��������� ���������� ������
    CurrentAmmo.Bullets--;

    // LogAmmo();

    //��������� ����� �� �����������. ������� ���� � ������� �� ������
    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        //������������� ��������
        StopFire();

        //������������� �� �������
        OnClipEmpty.Broadcast(this);
    }
}

//������� ������ true ����� ������� ����
bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    //���� ������� ������� � �������� 0 � ������� 0
    return !CurrentAmmo.Infinite && CurrentAmmo.CLips == 0 && IsClipEmpty();
}

//������� ������ true ����� ������� ����
bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

//������� ����� ��������
void ASTUBaseWeapon::ChangeClip()
{
    //���� ������� �� ����������
    if (!CurrentAmmo.Infinite)
    {
        //��������� �������� �� ��������
        if (CurrentAmmo.CLips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips!"));
        }
        //�������� ��������
        CurrentAmmo.CLips--;
    }
    //��������������� ���������� ������ � ��������� ��������
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;

    UE_LOG(LogBaseWeapon, Display, TEXT("----------Change Clip-----------"));
}

//������� ���������� �������� �� �����������
bool ASTUBaseWeapon::CanReload() const
{
    //������ true ���� �� �������� ������ � ���� ���� ��������
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.CLips > 0;
}

//������� ��������� ���������� �� ��������
void ASTUBaseWeapon::LogAmmo()
{
    //��������� ���������� � ����������� � ��������
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";

    //��������� � ������ ���� ���-�� ��������� ���� ������� ������� ���� ����� Infinite ���� ������� ����������
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.CLips);

    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

//������� ������ true ����� ������� �����
bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.CLips == DefaultAmmo.CLips && //
           CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

//������� ���������� �������� ����� �����
bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    //������� ������ �� �������: ������� ���������� ��� ������� ����� ��� ���-�� ��������� �����.
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
        return false;

    //���� ������� ����
    if (IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));

        CurrentAmmo.CLips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.CLips + 1);
        //������������� �� �������
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.CLips < DefaultAmmo.CLips)
    {
        const auto NextClipsAmount = CurrentAmmo.CLips + ClipsAmount;
        if (DefaultAmmo.CLips - NextClipsAmount >= 0)
        {
            CurrentAmmo.CLips = NextClipsAmount;

            UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added!"));
        }
        else
        {
            CurrentAmmo.CLips = DefaultAmmo.CLips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;

            UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now!"));
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;

        UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added!"));
    }
    return true;
}
