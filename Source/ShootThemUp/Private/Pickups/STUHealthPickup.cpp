// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(HealthPickupLog, All, All);

//�������������� ������� �� BasePickup
bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    //�������� ��������� �� STUHealthComponent � ������� ��������� ������� �� STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    //��������� �� ���������� HealthComponent
    if (!HealthComponent)
        return false;

    return HealthComponent->TryToAddHealth(HealthAmount);

    UE_LOG(HealthPickupLog, Display, TEXT("Health was added!"));

    return true;
}
