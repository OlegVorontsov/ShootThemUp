// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    //объ€вл€ем переопределение стандартной функции
    virtual void DrawHUD() override;

protected:
    //объ€вл€ем класс дл€ виджета
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    virtual void BeginPlay() override;

private:
    void DrawCrossHair();
};
