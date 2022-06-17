// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

//создаем собственную категорию логирования
DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    //отключаем функцию тик
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

    //задаем текущий арсенал через начальный
    CurrentAmmo = DefaultAmmo;
}

//виртуальные функции стрельбы
void ASTUBaseWeapon::StartFire() {}
void ASTUBaseWeapon::StopFire() {}
void ASTUBaseWeapon::MakeShot() {}

//функция получения нашего персонажа
APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    //получаем нашего персонажа и записываем в переменную
    const auto Player = Cast<ACharacter>(GetOwner());

    //если указатель не нулевой возвращаем его
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

//функция получения доступа к камере персонажа
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    //получаем доступ к камере с помощью функции получения нашего персонажа
    const auto Controller = GetPlayerController();
    if (!Controller)
        return false;

    //получаем доступ к камере персонажа
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

//функция получения расположения сокета в оружии
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

//функция получения начальной и конечной точки стрельбы
bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    //создаем переменные куда запишем параметры камеры персонажа
    FVector ViewLocation;
    FRotator ViewRotation;

    //получаем доступ к параметрам камеры персонажа
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    //переменная для точки начала выстрела
    TraceStart = ViewLocation;

    //переменная для направления выстрела
    const FVector ShootDirection = ViewRotation.Vector();

    //переменная для конечной точки выстрела
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

//функция выстрела
void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld())
        return;

    //создаем переменную для записи параметров пересечения
    FCollisionQueryParams CollisionParams;

    //указываем чтобы игнорировались пересечения с самим собой
    CollisionParams.AddIgnoredActor(GetOwner());

    //указываем что хоти получать материал куда попали
    CollisionParams.bReturnPhysicalMaterial = true;

    //вызываем функцию пересечения с первым попавшимся объектом на сцене
    GetWorld()->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

//функция уменьшающая количество патрон при выстреле
void ASTUBaseWeapon::DecreaseAmmo()
{
    //проверяем остались ли патроны
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty!"));
    }
    //уменьшаем количество патрон
    CurrentAmmo.Bullets--;

    // LogAmmo();

    //проверяем нужна ли перезарядка. магазин пуст и арсенал не пустой
    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        //останавливаем стрельбу
        StopFire();

        //подписываемся на делегат
        OnClipEmpty.Broadcast(this);
    }
}

//функция вернет true когда арсенал пуст
bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    //если арсенал конечен и магазины 0 и патроны 0
    return !CurrentAmmo.Infinite && CurrentAmmo.CLips == 0 && IsClipEmpty();
}

//функция вернет true когда магазин пуст
bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

//функция смены магазина
void ASTUBaseWeapon::ChangeClip()
{
    //если арсенал не бесконечен
    if (!CurrentAmmo.Infinite)
    {
        //проверяем остались ли магазины
        if (CurrentAmmo.CLips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips!"));
        }
        //убавляем мазазины
        CurrentAmmo.CLips--;
    }
    //восстанавливаем количество патрон в начальное значение
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;

    UE_LOG(LogBaseWeapon, Display, TEXT("----------Change Clip-----------"));
}

//функция определяет возможна ли перезарядка
bool ASTUBaseWeapon::CanReload() const
{
    //вернет true если не осталось патрон и если есть магазины
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.CLips > 0;
}

//функция выведения информации об арсенале
void ASTUBaseWeapon::LogAmmo()
{
    //локальная переменная с информацией о патронах
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";

    //добавляем в строку либо кол-во магазинов если арсенал конечен либо текст Infinite если арсенал бесконечен
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.CLips);

    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

//функция вернет true когда арсенал полон
bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.CLips == DefaultAmmo.CLips && //
           CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

//функция добавления арсенала через пикап
bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    //условие выхода из функции: арсенал бесконечен или арсенал полон или кол-во магазинов отриц.
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
        return false;

    //если арсенал пуст
    if (IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));

        CurrentAmmo.CLips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.CLips + 1);
        //подписываемся на делегат
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
