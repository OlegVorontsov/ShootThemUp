// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

//функция возвращает кол-во % оставшегося здоровья
float USTUPlayerHUDWidget::GetHealthPercent() const
{
    //получаем указатель на STUHealthComponent с помощью шаблонной функции из STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    if (!HealthComponent)
        return 0.0f;

    //возвращаем значение функции STUHealthComponent
    return HealthComponent->GetHealthPercent();
}

//функция возвращает данные об иконках оружия
bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    //получаем указатель на STUWeaponComponent с помощью шаблонной функции из STUUtils
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (!WeaponComponent)
        return false;

    //возвращаем значение функции STUWeponComponent
    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

//функция возвращает данные о текущем арсенале оружия
bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    //получаем указатель на STUWeaponComponent с помощью шаблонной функции из STUUtils
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (!WeaponComponent)
        return false;

    //возвращаем значение функции STUWeponComponent
    return WeaponComponent->GetCurrentAmmoData(AmmoData);
}

//функция возвращает жив/мертв персонаж
bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    //получаем указатель на STUHealthComponent с помощью шаблонной функции из STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    //персонаж жив когда валиден HealthComponent и персонаж не мертв
    return HealthComponent && !HealthComponent->IsDead();
}

//функция возвращает в режиме спектаитинга или нет
bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    //получаем указатель на контроллер
    const auto Controller = GetOwningPlayer();

    //в режиме спектаитинга когда контроллер валиден и мы управляем спектаитингом
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
