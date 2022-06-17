// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(AmmoPickupLog, All, All);

//переопределяем функцию из BasePickup
bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    //получаем указатель на STUHealthComponent с помощью шаблонной функции из STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    //проверяем на валидность HealthComponent или персонаж мертв
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    //получаем указатель на STUWeaponComponent
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);

    UE_LOG(AmmoPickupLog, Display, TEXT("Ammo was added!"));

    return true;
}
