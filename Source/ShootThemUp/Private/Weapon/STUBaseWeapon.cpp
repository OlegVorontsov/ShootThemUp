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

//функция стрельбы
void ASTUBaseWeapon::Fire()
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"));

    //вызов функции выстрела
    MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld())
        return;

    //создаем переменные для начальной/конечной точек выстрела
    FVector TraceStart, TraceEnd;

    //вызываем функцию получения начальной и конечной точки стрельбы
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    //переменная с результатами столкновения
    FHitResult HitResult;

    //вызываем функцию выстрела
    MakeHit(HitResult, TraceStart, TraceEnd);

    //проверяем если пересечение произошло
    if (HitResult.bBlockingHit)
    {
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
}

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
