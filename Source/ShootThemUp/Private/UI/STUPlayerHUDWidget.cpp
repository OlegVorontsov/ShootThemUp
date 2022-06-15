// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

//������� ���������� ���-�� % ����������� ��������
float USTUPlayerHUDWidget::GetHealthPercent() const
{
    //�������� ��������� �� STUHealthComponent � ������� ��������� ������� �� STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    if (!HealthComponent)
        return 0.0f;

    //���������� �������� ������� STUHealthComponent
    return HealthComponent->GetHealthPercent();
}

//������� ���������� ������ �� ������� ������
bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    //�������� ��������� �� STUWeaponComponent � ������� ��������� ������� �� STUUtils
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (!WeaponComponent)
        return false;

    //���������� �������� ������� STUWeponComponent
    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

//������� ���������� ������ � ������� �������� ������
bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    //�������� ��������� �� STUWeaponComponent � ������� ��������� ������� �� STUUtils
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (!WeaponComponent)
        return false;

    //���������� �������� ������� STUWeponComponent
    return WeaponComponent->GetCurrentAmmoData(AmmoData);
}

//������� ���������� ���/����� ��������
bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    //�������� ��������� �� STUHealthComponent � ������� ��������� ������� �� STUUtils
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    //�������� ��� ����� ������� HealthComponent � �������� �� �����
    return HealthComponent && !HealthComponent->IsDead();
}

//������� ���������� � ������ ������������ ��� ���
bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    //�������� ��������� �� ����������
    const auto Controller = GetOwningPlayer();

    //� ������ ������������ ����� ���������� ������� � �� ��������� �������������
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
