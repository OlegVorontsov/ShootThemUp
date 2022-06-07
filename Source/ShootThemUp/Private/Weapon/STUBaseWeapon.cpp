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

    //вызываем функцию пересечения с первым попавшимся объектом на сцене
    GetWorld()->LineTraceSingleByChannel(
        HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}
