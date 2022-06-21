// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    //функция возвращает кол-во % оставшегося здоровья
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    //функция возвращает данные об иконках оружия
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    //функция возвращает данные о текущем арсенале оружия
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    //функция возвращает жив/мертв персонаж
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    //функция возвращает в режиме спектаитинга или нет
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

    //функция запустит анимацию в блюпринте
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();

    //функция подписания на делегат
    virtual bool Initialize() override;

private:
    //функция для биндинга
    void OnHealthChanged(float Health, float HealthDelta);
};
