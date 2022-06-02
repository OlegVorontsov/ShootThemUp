// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUDevDamageActor();

    //объвляем компонент для root актора
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;

    //перременная радиуса сферы
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 300.0f;

    //переменная цвета сферы
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;

    //переменная размера ущерба
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10.0f;

    //переменная true - полный ущерб, false - ущерб уменьшается от центра
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool DoFullDamage = false;

    //объявляем класс которым будем наносить урон персонажу
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDamageType> DamageType;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
