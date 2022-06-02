// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

//переопределяем стандартную функцию
void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
    //создаем объект типа TInterval для вычисления центра экрана
    //с помощью переменных содержащих координаты центра экрана
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    //функция нанесения линий в центре экрана для прицела
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
