// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(HealthPickupLog, All, All);

//переопределяем функцию из BasePickup
bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    //получаем указатель на STUHealthComponent с помощью шаблонной функции из STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    //проверяем на валидность HealthComponent
    if (!HealthComponent)
        return false;

    return HealthComponent->TryToAddHealth(HealthAmount);

    UE_LOG(HealthPickupLog, Display, TEXT("Health was added!"));

    return true;
}
