// Copyright Drop Games Inc.
#include "SRadarChart.h"
#include "SlateOptMacros.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "UICharts"

void SRadarChart::SetData(TArray<float> InData)
{
	float MaxX = -FLT_MAX;

	//Preprocess
	for (const float X : InData)
	{
		if (X > MaxX)
		{
			MaxX = X;
		}
	}

	DataBounds = MaxX;
	Data = InData;
}

void SRadarChart::Construct(const FArguments& InArgs)
{
	Brush = FInvalidatableBrushAttribute(InArgs._Brush);

#if WITH_EDITOR
	// const TArray<float> Mock = {2.f, 5.f, 5.4f, 6.4f, 7.8f, 5.4f, 6.4f, 7.8f};
	const TArray<float> Mock = {2.f, 5.f, 12.4f};
	const TArray<FText> MockLabels = {
		LOCTEXT("MockText", "Speed"), LOCTEXT("MockText", "Damage"), LOCTEXT("MockText", "Durability")
	};
	SetData(Mock);
#endif
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SRadarChart::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                           const FSlateRect& MyCullingRect,
                           FSlateWindowElementList& OutDrawElements, int32 LayerId,
                           const FWidgetStyle& InWidgetStyle,
                           bool bParentEnabled) const
{
	const int N = Data.Num();
	if (N < 3)
	{
		return LayerId;
	}


	TArray<FSlateVertex> Vertices;
	TArray<SlateIndex> Indices;
	
	TArray<FVector2D> FullLineSegments;
	TArray<FVector2D> PartLineSegments;

	const FSlateBrush* SlateBrush = Brush.GetImage().Get();
	FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*SlateBrush);
	FLinearColor LinearColor = GetColorAndOpacity() * InWidgetStyle.GetColorAndOpacityTint() * SlateBrush->
		GetTint(InWidgetStyle);
	FColor FinalColorAndOpacity = LinearColor.ToFColor(true);

	const FVector2D Pos = AllottedGeometry.GetAbsolutePosition();
	const FVector2D Size = AllottedGeometry.GetAbsoluteSize();
	const FVector2D Center = Pos + 0.5 * Size;

	Vertices.AddZeroed();
	FSlateVertex& CenterVertex = Vertices.Last();
	CenterVertex.Position = FVector2f(Center);
	CenterVertex.Color = FinalColorAndOpacity;
	CenterVertex.MaterialTexCoords[0] = 0.f;
	CenterVertex.MaterialTexCoords[1] = 0.5f;
	CenterVertex.TexCoords[0] = 0.f;
	CenterVertex.TexCoords[1] = 0.5f;
	CenterVertex.TexCoords[2] = 0.f;
	CenterVertex.TexCoords[3] = 0.5f;

	const float Radius = FMath::Min(Size.X, Size.Y) * 0.5f;
	const float ArcLen = (2 * PI) / N;

	for (int32 i = 0; i <= N; ++i)
	{
		// wrap
		const float Angle = (i < N ? i * ArcLen : 0.f) - (PI / 2);
		const FVector2D EdgeDirection = Radius * FVector2D(
			FMath::Cos(Angle), FMath::Sin(Angle));

		PartLineSegments.Add(
			AllottedGeometry.AbsoluteToLocal(Center + (Data[i < N ? i : 0] / DataBounds) * EdgeDirection));
		FullLineSegments.Add(AllottedGeometry.AbsoluteToLocal(Center + EdgeDirection));

		const FVector2D UV_Data{0.5f, (i % 2) * 1.f};


		FSlateVertex V0;
		V0.Position = FVector2f(Center + (Data[i < N ? i : 0] / DataBounds) * EdgeDirection);
		V0.Color = FinalColorAndOpacity;
		V0.MaterialTexCoords[0] = UV_Data.X;
		V0.MaterialTexCoords[1] = UV_Data.Y;
		V0.TexCoords[0] = UV_Data.X;
		V0.TexCoords[1] = UV_Data.Y;
		V0.TexCoords[2] = UV_Data.X;
		V0.TexCoords[3] = UV_Data.Y;
		Vertices.Add(V0);
	}

	//close loop
	for (int32 i = 1; i <= N; ++i)
	{
		Indices.Add(0);
		Indices.Add(i);
		Indices.Add(i + 1);
	}

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId + 2,
		AllottedGeometry.ToPaintGeometry(),
		FullLineSegments,
		ESlateDrawEffect::None,
		FLinearColor::White, // Line color
		true, // Anti-alias
		6.0f // Line thickness
	);

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId + 2,
		AllottedGeometry.ToPaintGeometry(),
		PartLineSegments,
		ESlateDrawEffect::None,
		FLinearColor::White, // Line color
		true, // Anti-alias
		2.5f // Line thickness
	);

	// Use MakeCustomVerts to draw the polygon and apply a material
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId,
		Handle,
		Vertices,
		Indices,
		nullptr, // No clipping
		0, // Layer
		0 // Render effects
	);

	// Use MakeCustomVerts to draw the polygon and apply a material
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId,
		Handle,
		Vertices,
		Indices,
		nullptr, // No clipping
		0, // Layer
		0 // Render effects
	);

	return LayerId;
}

void SRadarChart::SetBrush(FSlateBrush* InBrush)
{
	Brush.SetImage(*this, InBrush);
}
#undef LOCTEXT_NAMESPACE
