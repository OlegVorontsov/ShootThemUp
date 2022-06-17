// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(AmmoPickupLog, All, All);

//�������������� ������� �� BasePickup
bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    //�������� ��������� �� STUHealthComponent � ������� ��������� ������� �� STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    //��������� �� ���������� HealthComponent ��� �������� �����
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    //�������� ��������� �� STUWeaponComponent
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);

    UE_LOG(AmmoPickupLog, Display, TEXT("Ammo was added!"));

    return true;
}
